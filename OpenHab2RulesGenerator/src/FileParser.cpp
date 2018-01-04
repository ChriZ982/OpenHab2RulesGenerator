#include "FileParser.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include "StringUtils.h"
#include <set>
#include <chrono>

namespace zindach_openhab_rules_generator {

std::vector<std::string> read_file(const std::string &filename) {
    std::wifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: File \"" + filename + "\" could not be opened for reading!");
    }

    std::vector<std::string> result;

    for (std::wstring line; getline(file, line);) {
#ifdef PLATFORM_UNIX
        result.emplace_back(erase_all(std::string(line.begin(), line.end()), {"\r"}));
#else
        result.emplace_back(line.begin(), line.end());
#endif
    }

    return result;
}

void write_file(const std::string &filename, const std::vector<std::string> &lines) {
    std::wofstream file(filename, std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Error: File \"" + filename + "\" could not be opened for writing!");
    }

    for (const auto &line : lines) {
#ifdef PLATFORM_UNIX
        file << std::wstring(line.begin(), line.end()) << "\r\n";
#else
        file << std::wstring(line.begin(), line.end()) << "\n";
#endif
    }
}

std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines) {
    std::vector<Rule> result;
    const size_t table_index = line_index_starts_with(lines, "Tabelle", 0);

    std::vector<std::string> keys = split(lines[table_index + 1], ";");

    for (size_t i = table_index + 2; i < lines.size(); ++i) {
        result.emplace_back(keys, lines[i]);
    }

    return result;
}

std::map<std::string, Template> read_templates_from_file(const std::vector<std::string> &lines) {
    std::map<std::string, Template> result;

    size_t template_index = line_index_starts_with(lines, "#template:", 0);
    while (template_index != std::string::npos) {
        size_t next_template_index = line_index_starts_with(lines, "#template:", template_index + 1);
        if (next_template_index == std::string::npos) {
            next_template_index = lines.size();
        }

        std::vector<std::string> template_lines;
        for (size_t i = template_index + 1; i < next_template_index; ++i) {
            template_lines.emplace_back(lines[i]);
        }
        result[trim(erase_all(lines[template_index], {"#template:"}))] = Template(template_lines);

        template_index = line_index_starts_with(lines, "#template:", template_index + 1);
    }
    return result;
}

std::vector<std::string> create_rules_file(std::vector<Rule> &rules,
                                           std::map<std::string, Template> &templates) {
    std::vector<std::string> result;

    for (auto &rule : rules) {
        const Template temp = templates[rule.value_map[std::string("Aktion")]];
        std::vector<std::string> template_lines = temp.replace(rule.value_map);

        result.emplace_back(std::string("rule \"") + join_values(rule.value_map) + std::string("\""));
        for (auto template_line : template_lines) {
            result.emplace_back(template_line);
        }
    }

    return result;
}

std::vector<std::string> group_by_condition(std::vector<std::string> lines) {
    std::map<std::string, std::vector<size_t>> groups;

    size_t when_line = line_index_equals(lines, "when", 0);
    while (when_line != std::string::npos) {
        const size_t then_line = line_index_equals(lines, "then", when_line + 1);
        if (then_line - when_line == 2) {
            groups[trim(lines[when_line + 1])].emplace_back(when_line + 1);
        }

        when_line = line_index_equals(lines, "when", then_line + 2);
    }

    for (auto iterator = groups.begin(); iterator != groups.end();) {
        if (iterator->second.size() <= 1) {
            iterator = groups.erase(iterator);
        } else {
            ++iterator;
        }
    }
    std::cout << "(2/6) Grouping " << groups.size() << " rules by condition\n";

    for (auto pair = groups.begin(); pair != groups.end();) {
        const size_t rule_line = line_index_starts_with_reverse(lines, "rule", pair->second[0]);
        lines[rule_line] = "rule \"Group " + erase_all(pair->first, {"*", "\"", "?", ":", "#"}) + "\"";

        std::vector<std::string> additional_lines;

        for (size_t i = 1; i < pair->second.size(); ++i) {
            const size_t rule_line_idx = line_index_starts_with_reverse(lines, "rule", pair->second[i]);
            const size_t then_line_idx = line_index_equals(lines, "then", pair->second[i]);
            const size_t end_line_idx = line_index_equals(lines, "end", pair->second[i]);

            additional_lines.insert(additional_lines.end(), lines.begin() + then_line_idx + 1,
                                    lines.begin() + end_line_idx);

            for (auto &values : groups) {
                for (auto &value : values.second) {
                    if (value >= rule_line_idx) {
                        value -= end_line_idx - rule_line_idx + 1;
                    }
                }
            }

            lines.erase(lines.begin() + rule_line_idx, lines.begin() + end_line_idx + 1);
        }

        const size_t end_line_idx = line_index_equals(lines, "end", pair->second[0]);
        lines.insert(lines.begin() + end_line_idx, additional_lines.begin(), additional_lines.end());

        pair = groups.erase(groups.begin());
        for (auto &values : groups) {
            for (auto &value : values.second) {
                if (value >= end_line_idx) {
                    value += additional_lines.size();
                }
            }
        }
    }
    return lines;
}

std::vector<std::string> group_by_if_condition(const std::vector<std::string> &lines) {
    std::vector<std::string> result = lines;
    std::map<size_t, std::map<std::string, std::vector<size_t>>> if_conditions;

    size_t then_line_idx = std::string::npos;
    for (size_t i = 0; i < result.size(); ++i) {
        std::string trim_line = trim(result[i]);
        if (trim_line == "then") {
            then_line_idx = i;
        } else if (trim_line.find("if") == 0 && trim_line.find_last_of("{") == trim_line.size() - 1) {
            if_conditions[then_line_idx][trim_line].emplace_back(i);
            i = find_closing_brace(result, i);
        }
    }

    auto rule_iterator = if_conditions.begin();
    while (rule_iterator != if_conditions.end()) {
        bool optimizable = false;

        auto if_iterator = rule_iterator->second.begin();
        while (if_iterator != rule_iterator->second.end()) {
            if (if_iterator->second.size() <= 1) {
                if_iterator = rule_iterator->second.erase(if_iterator);
            } else {
                optimizable = true;
                ++if_iterator;
            }
        }

        if (!optimizable) {
            rule_iterator = if_conditions.erase(rule_iterator);
        } else {
            ++rule_iterator;
        }
    }
    std::cout << "(3/6) Grouping " << if_conditions.size() << " if statements by condition\n";

    rule_iterator = if_conditions.begin();
    for (size_t k = 0; k < if_conditions.size(); ++k) {
        auto if_iterator = rule_iterator->second.begin();
        for (size_t l = 0; l < rule_iterator->second.size(); ++l) {
            std::vector<std::string> additional_lines;
            for (size_t i = 1; i < if_iterator->second.size(); ++i) {
                const size_t end_if_line_idx = find_closing_brace(result, if_iterator->second[i]);

                additional_lines.emplace_back("\t\tThread::sleep(2000)");
                for (size_t j = if_iterator->second[i] + 1; j < end_if_line_idx; ++j) {
                    additional_lines.emplace_back(result[j]);
                }

                for (size_t j = 0; j < if_iterator->second.size(); ++j) {
                    if (if_iterator->second[j] > if_iterator->second[i]) {
                        if_iterator->second[j] -= end_if_line_idx - if_iterator->second[i] + 1;
                    }
                }

                auto iterator = result.begin();
                advance(iterator, if_iterator->second[i]);
                for (size_t j = if_iterator->second[i]; j <= end_if_line_idx; ++j) {
                    iterator = result.erase(iterator);
                }
            }

            const size_t end_if_line_idx = find_closing_brace(result, if_iterator->second[0]);
            auto iterator = result.begin();
            advance(iterator, end_if_line_idx);
            for (const auto &additional_line : additional_lines) {
                iterator = result.emplace(iterator, additional_line);
                ++iterator;
            }

            const size_t removed_lines_count = if_iterator->second.size() - 1;
            std::map<size_t, std::map<std::string, std::vector<size_t>>> new_if_conditions;
            for (auto &rule_pair : if_conditions) {
                if (rule_pair.first > end_if_line_idx) {
                    new_if_conditions[rule_pair.first - removed_lines_count] = rule_pair.second;
                } else {
                    new_if_conditions[rule_pair.first] = rule_pair.second;
                }
            }

            for (auto &rule_pair : new_if_conditions) {
                for (auto &if_condition_pair : rule_pair.second) {
                    for (auto &line_idx : if_condition_pair.second) {
                        if (line_idx > end_if_line_idx) {
                            line_idx -= removed_lines_count;
                        }
                    }
                }
            }
            if_conditions = new_if_conditions;
            rule_iterator = if_conditions.begin();
            advance(rule_iterator, k);
            if_iterator = rule_iterator->second.begin();
            advance(if_iterator, l + 1);
        }
        ++rule_iterator;
    }

    return result;
}
}

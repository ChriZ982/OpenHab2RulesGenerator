#include "Optimizer.h"
#include <iostream>
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

std::map<std::string, std::vector<size_t>> optimizable_rules(const std::vector<std::string> &lines) {
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

    return groups;
}

std::vector<std::string> group_by_condition(std::vector<std::string> lines) {
    auto groups = optimizable_rules(lines);
    std::cout << "\toptimizing " << groups.size() << " rules\n";

    for (auto pair = groups.begin(); pair != groups.end(); pair = groups.erase(groups.begin())) {
        const size_t first_rule_line_index = line_index_starts_with_reverse(lines, "rule", pair->second[0]);

        lines[first_rule_line_index] = "rule \"Group " + erase_all(pair->first, {"*", "\"", "?", ":", "#"}) + "\"";

        for (size_t i = 1; i < pair->second.size(); ++i) {
            const size_t rule_line_index = line_index_starts_with_reverse(lines, "rule", pair->second[i]);
            const size_t then_line_index = line_index_equals(lines, "then", rule_line_index);
            const size_t end_line_index = line_index_equals(lines, "end", then_line_index);

            const std::vector<std::string> additional_lines(lines.begin() + then_line_index + 1,
                                                            lines.begin() + end_line_index);

            const size_t first_end_line_index = line_index_equals(lines, "end", first_rule_line_index);

            lines.erase(lines.begin() + rule_line_index, lines.begin() + end_line_index + 1);
            lines.insert(lines.begin() + first_end_line_index, additional_lines.begin(), additional_lines.end());

            for (auto &values : groups) {
                for (auto &value : values.second) {
                    if (value > first_end_line_index) {
                        value += additional_lines.size();
                    }
                    if (value > end_line_index) {
                        value -= end_line_index - rule_line_index + 1;
                    }
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
    std::cout << "\toptimizing " << if_conditions.size() << " if statements\n";

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

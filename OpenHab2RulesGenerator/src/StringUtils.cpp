#include "StringUtils.h"
#include <locale>

namespace zindach_openhab_rules_generator {

std::string StringUtils::convert_template_key(const std::string &text) {
    std::string result = text;
    for (auto &value : result) {
        value = std::toupper(value, std::locale());
    }
    return std::string("$") + result + std::string("$");
}

std::string StringUtils::replace_all(const std::string &text, const std::string &from, const std::string &to) {
    std::string result = text;
    size_t last_replace = 0;

    while ((last_replace = result.find(from, last_replace)) != std::string::npos) {
        result.replace(last_replace, from.length(), to);
        last_replace += to.length();
    }

    return result;
}

std::string StringUtils::erase_all(const std::string &text, const std::vector<std::string> &values) {
    std::string result = text;

    for (const auto &val : values) {
        result = replace_all(result, val, "");
    }

    return result;
}

std::vector<std::string> StringUtils::split(const std::string &text, const std::string &delimiter) {
    std::vector<std::string> result;

    size_t start_index = 0;
    size_t end_index = text.find(delimiter, start_index);

    while (end_index != std::string::npos) {
        result.emplace_back(text.substr(start_index, end_index - start_index));

        start_index = end_index + delimiter.length();
        end_index = text.find(delimiter, start_index);

        if (end_index == std::string::npos && start_index != text.length() + 1) {
            end_index = text.length();
        }
    }

    return result;
}

size_t StringUtils::get_index_line_begins(const std::vector<std::string> &lines, const std::string &begin,
                                          const size_t start) {
    size_t idx = std::string::npos;
    for (size_t i = start; i < lines.size(); ++i) {
        if (trim(lines[i]).find(begin) == 0) {
            idx = i;
            break;
        }
    }
    return idx;
}

size_t StringUtils::get_index_line_equals(const std::vector<std::string> &lines, const std::string &text,
                                          const size_t start) {
    size_t idx = std::string::npos;
    for (size_t i = start; i < lines.size(); ++i) {
        if (trim(lines[i]) == text) {
            idx = i;
            break;
        }
    }
    return idx;
}

size_t StringUtils::get_index_line_begins_reverse(const std::vector<std::string> &lines, const std::string &begin,
                                                  const size_t start) {
    size_t idx = std::string::npos;
    for (size_t i = 0; i <= start; ++i) {
        if (trim(lines[start - i]).find(begin) == 0) {
            idx = start - i;
            break;
        }
    }
    return idx;
}

std::string StringUtils::get_config_value(const std::vector<std::string> &lines, const std::string &name,
                                          const std::string &delimiter) {
    return split(lines[get_index_line_begins(lines, name, 0)], delimiter)[1];
}

std::string StringUtils::join_values(const std::map<std::string, std::string> &map) {
    std::string result;
    for (const auto &pair : map) {
        if (!pair.second.empty()) {
            result += pair.second + std::string(" ");
        }
    }
    return result.substr(0, result.length() - 1);
}

std::string StringUtils::trim(const std::string &text) {
    std::string result = text;
    while ((result.find("\t") == 0 || result.find("\r") == 0 || result.find("\n") == 0 || result.find(" ") == 0) && !result.
        empty()) {
        result = result.substr(1, result.length() - 1);
    }
    while ((result.find("\t") == result.length() - 1 || result.find("\r") == result.length() - 1 || result.find("\n") ==
        result.length() - 1 || result.find(" ") == result.length() - 1) && !result.empty()) {
        result = result.substr(0, result.length() - 1);
    }
    return result;
}

std::vector<std::string> StringUtils::erase_unnecessary_whitespace(const std::vector<std::string> &lines) {
    std::vector<std::string> result = lines;

    bool empty_line_before = false;
    auto iterator = result.begin();
    while (iterator != result.end()) {
        if (empty_line_before && trim(*iterator).empty()) {
            iterator = result.erase(iterator);
        } else if (!empty_line_before && trim(*iterator).empty()) {
            empty_line_before = true;
            ++iterator;
        } else {
            bool space_before = false;
            auto ch_iterator = iterator->begin();
            while (ch_iterator != iterator->end()) {
                if (space_before && *ch_iterator == ' ') {
                    ch_iterator = iterator->erase(ch_iterator);
                } else if (!space_before && *ch_iterator == ' ') {
                    space_before = true;
                    ++ch_iterator;
                } else {
                    space_before = false;
                    ++ch_iterator;
                }
            }

            empty_line_before = false;
            ++iterator;
        }
    }

    return result;
}

size_t StringUtils::find_closing_brace(const std::vector<std::string> &lines, const size_t if_idx) {
    size_t end_if_line_idx = std::string::npos;
    int opened_if_statements = 1;
    for (size_t j = if_idx + 1; j < lines.size(); ++j) {
        std::string trim_line = trim(lines[j]);
        if (trim_line == "}") {
            --opened_if_statements;
        } else if (trim_line.find("if") == 0 && trim_line.find_last_of("{") == trim_line.size() - 1) {
            ++opened_if_statements;
        }
        if (opened_if_statements == 0) {
            end_if_line_idx = j;
            break;
        }
    }
    return end_if_line_idx;
}
}

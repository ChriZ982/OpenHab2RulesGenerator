#include "StringUtils.h"
#include <locale>

namespace zindach_openhab_rules_generator {

std::string replace_all(std::string text, const std::string &from, const std::string &to) {
    size_t last_replace = text.find(from);

    while (last_replace != std::string::npos) {
        text.replace(last_replace, from.length(), to);

        last_replace = text.find(from, last_replace + to.length());
    }

    return text;
}

std::string erase_all(std::string text, const std::vector<std::string> &values) {
    for (const auto &value : values) {
        text = replace_all(text, value, "");
    }

    return text;
}

std::vector<std::string> erase_unnecessary_whitespace(std::vector<std::string> lines) {
    bool empty_line_before = false;
    auto it = lines.begin();

    while (it != lines.end()) {
        if (empty_line_before && left_trim(*it).empty()) {
            it = lines.erase(it);
        } else if (!empty_line_before && left_trim(*it).empty()) {
            empty_line_before = true;
            ++it;
        } else {
            bool space_before = false;
            auto char_it = it->begin();

            while (char_it != it->end()) {
                if (space_before && *char_it == ' ') {
                    char_it = it->erase(char_it);
                } else if (!space_before && *char_it == ' ') {
                    space_before = true;
                    ++char_it;
                } else {
                    space_before = false;
                    ++char_it;
                }
            }

            empty_line_before = false;
            ++it;
        }
    }

    return lines;
}

std::string join_values(const std::map<std::string, std::string> &map) {
    std::string result;

    for (const auto &pair : map) {
        result += pair.second + " ";
    }

    return trim(result);
}

std::vector<std::string> split(const std::string &text, const std::string &delimiter) {
    std::vector<std::string> result;
    size_t end_index;
    size_t start_index = 0;

    do {
        end_index = text.find(delimiter, start_index);
        if (end_index == std::string::npos) {
            end_index = text.length();
        }

        result.emplace_back(trim(text.substr(start_index, end_index - start_index)));

        start_index = end_index + delimiter.length();
    } while (end_index != text.length());

    return result;
}

size_t line_index_equals(const std::vector<std::string> &lines, const std::string &text, const size_t start = 0) {
    for (size_t i = start; i < lines.size(); ++i) {
        if (trim(lines[i]) == text) {
            return i;
        }
    }

    return std::string::npos;
}

size_t line_index_equals_reverse(const std::vector<std::string> &lines, const std::string &text, const size_t start) {
    for (size_t i = 0; i <= start; ++i) {
        if (trim(lines[start - i]) == text) {
            return start - i;
        }
    }

    return std::string::npos;
}

size_t line_index_starts_with(const std::vector<std::string> &lines, const std::string &begin, const size_t start = 0) {
    for (size_t i = start; i < lines.size(); ++i) {
        if (left_trim(lines[i]).find(begin) == 0) {
            return i;
        }
    }

    return std::string::npos;
}

size_t line_index_starts_with_reverse(const std::vector<std::string> &lines, const std::string &begin, const size_t start) {
    for (size_t i = 0; i <= start; ++i) {
        if (left_trim(lines[start - i]).find(begin) == 0) {
            return start - i;
        }
    }

    return std::string::npos;
}

std::string left_trim(std::string text) {
    for (auto it = text.begin(); it != text.end(); ++it) {
        if (*it != '\n' && *it != '\r' && *it != '\t' && *it != ' ') {
            text.erase(text.begin(), it);

            return text;
        }
    }

    return "";
}

std::string right_trim(std::string text) {
    for (auto it = text.rbegin(); it != text.rend(); ++it) {
        if (*it != '\n' && *it != '\r' && *it != '\t' && *it != ' ') {
            text.erase(it.base(), text.end());

            return text;
        }
    }

    return "";
}

std::string trim(std::string text) {
    for (auto left_it = text.begin(); left_it != text.end(); ++left_it) {
        if (*left_it != '\n' && *left_it != '\r' && *left_it != '\t' && *left_it != ' ') {
            text.erase(text.begin(), left_it);

            for (auto right_it = text.rbegin(); right_it != text.rend(); ++right_it) {
                if (*right_it != '\n' && *right_it != '\r' && *right_it != '\t' && *right_it != ' ') {
                    text.erase(right_it.base(), text.end());

                    return text;
                }
            }
        }
    }

    return "";
}

std::string read_config_value(const std::vector<std::string> &lines, const std::string &name) {
    return split(lines[line_index_starts_with(lines, name)], ";")[1];
}

std::string convert_to_template_key(std::string text) {
    for (auto &ch : text) {
        ch = std::toupper(ch, std::locale());
    }

    return "$" + text + "$";
}

size_t find_closing_brace(const std::vector<std::string> &lines, const size_t if_line_index) {
    int opened_if_statements = 1;

    for (size_t j = if_line_index + 1; j < lines.size(); ++j) {
        const std::string trim_line = trim(lines[j]);

        if (trim_line == "}") {
            --opened_if_statements;
        } else if (trim_line.find("if") == 0 && trim_line.find_last_of("{") == trim_line.size() - 1) {
            ++opened_if_statements;
        }

        if (opened_if_statements == 0) {
            return j;
        }
    }

    return std::string::npos;
}
}

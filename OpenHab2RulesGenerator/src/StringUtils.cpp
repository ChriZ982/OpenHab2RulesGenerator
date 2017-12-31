#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

std::string StringUtils::convert_template_key(const std::string &text) {
    std::string result = text;
    for (auto &value : result) {
        value = toupper(value);
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
        if (lines[i].find(begin) == 0) {
            idx = i;
            break;
        }
    }
    return idx;
}

std::string StringUtils::get_config_value(const std::vector<std::string> &lines, const std::string &name,
                                          const std::string &delimiter) {
    return split(lines[get_index_line_begins(lines, name, 0)], delimiter)[1];
}
}

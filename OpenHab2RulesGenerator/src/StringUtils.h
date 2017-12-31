#pragma once
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

class StringUtils {
public:
    static std::string convert_template_key(const std::string &text);
    static std::string replace_all(const std::string &text, const std::string &from, const std::string &to);
    static std::string erase_all(const std::string &text, const std::vector<std::string> &values);
    static std::vector<std::string> split(const std::string &text, const std::string &delimiter);
    static size_t get_index_line_begins(const std::vector<std::string> &lines, const std::string &begin,
                                        const size_t start);
    static std::string get_config_value(const std::vector<std::string> &lines, const std::string &name,
                                        const std::string &delimiter);
};
}

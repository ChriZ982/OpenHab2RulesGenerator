#pragma once
#include <map>
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
    static size_t get_index_line_equals(const std::vector<std::string> &lines, const std::string &text,
                                        const size_t start);
    static size_t get_index_line_begins_reverse(const std::vector<std::string> &lines, const std::string &begin,
                                                const size_t start);
    static std::string get_config_value(const std::vector<std::string> &lines, const std::string &name,
                                        const std::string &delimiter);
    static std::string join_values(const std::map<std::string, std::string> &map);
    static std::string trim(const std::string &text);
    static std::vector<std::string> erase_unnecessary_whitespace(const std::vector<std::string> &lines);
    static size_t find_closing_brace(const std::vector<std::string> &lines, const size_t if_idx);
};
}

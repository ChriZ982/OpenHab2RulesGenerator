#pragma once
#include <map>
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

std::string replace_all(std::string text, const std::string &from, const std::string &to);
std::string erase_all(std::string text, const std::vector<std::string> &values);
std::vector<std::string> erase_unnecessary_whitespace(std::vector<std::string> lines);

std::string join_values(const std::map<std::string, std::string> &map);
std::vector<std::string> split(const std::string &text, const std::string &delimiter);

size_t line_index_equals(const std::vector<std::string> &lines, const std::string &text, const size_t start = 0);
size_t line_index_equals_reverse(const std::vector<std::string> &lines, const std::string &text, const size_t start);
size_t line_index_starts_with(const std::vector<std::string> &lines, const std::string &begin, const size_t start = 0);
size_t line_index_starts_with_reverse(const std::vector<std::string> &lines, const std::string &begin, const size_t start);

std::string left_trim(std::string text);
std::string right_trim(std::string text);
std::string trim(std::string text);

std::string read_config_value(const std::vector<std::string> &lines, const std::string &name);
std::string convert_to_template_key(std::string text);
size_t find_closing_brace(const std::vector<std::string> &lines, const size_t if_line_index);
}

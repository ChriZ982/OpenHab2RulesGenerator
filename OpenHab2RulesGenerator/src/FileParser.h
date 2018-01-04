#pragma once
#include <string>
#include <vector>
#include "Rule.h"
#include "Template.h"

namespace zindach_openhab_rules_generator {

std::vector<std::string> read_file(const std::string &filename);
void write_file(const std::string &filename, const std::vector<std::string> &lines);
std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines);
std::map<std::string, Template> read_templates_from_file(const std::vector<std::string> &lines);
std::vector<std::string> create_rules_file(std::vector<Rule> &rules, std::map<std::string, Template> &templates);
std::vector<std::string> group_by_condition(const std::vector<std::string> lines);
std::vector<std::string> group_by_if_condition(const std::vector<std::string> &lines);
}

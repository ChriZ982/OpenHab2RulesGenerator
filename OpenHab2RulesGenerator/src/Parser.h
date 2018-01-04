#pragma once
#include <string>
#include <vector>
#include "Rule.h"
#include "Template.h"

namespace zindach_openhab_rules_generator {

std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines);
std::map<std::string, Template> read_templates_from_file(const std::vector<std::string> &lines);
std::vector<std::string> create_rules_file(std::vector<Rule> &rules, std::map<std::string, Template> &templates);
}

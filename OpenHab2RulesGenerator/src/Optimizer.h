#pragma once
#include <map>
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

std::map<std::string, std::vector<size_t>> optimizable_rules(const std::vector<std::string> &lines);
std::vector<std::string> group_by_condition(std::vector<std::string> lines);
std::vector<std::string> group_by_if_condition(const std::vector<std::string> &lines);
}

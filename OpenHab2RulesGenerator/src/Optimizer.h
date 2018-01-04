#pragma once
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

std::vector<std::string> group_by_condition(const std::vector<std::string> lines);
std::vector<std::string> group_by_if_condition(const std::vector<std::string> &lines);
}

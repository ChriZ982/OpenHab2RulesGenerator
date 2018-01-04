#pragma once
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

std::vector<std::string> read_file(const std::string &filename);
void write_file(const std::string &filename, const std::vector<std::string> &lines);
}

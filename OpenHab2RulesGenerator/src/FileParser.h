#pragma once
#include <vector>
#include <string>
#include "Rule.h"

namespace zindach_openhab_rules_generator {

class FileParser {
public:
    static std::vector<std::string> read_file(const std::string &filename);
    static std::vector<Rule> read_rules_from_file(const std::string &filename);
};
}

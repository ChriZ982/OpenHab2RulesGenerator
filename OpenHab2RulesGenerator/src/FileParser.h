#pragma once
#include <vector>
#include <string>
#include "Rule.h"

namespace zindach_openhab_rules_generator {

class FileParser {
    static std::stringstream get_commands(const Rule &rule);

public:
    static std::vector<std::string> read_file(const std::string &filename);
    static void write_file(const std::string &filename, const std::vector<std::string> &lines);
    static std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines);
    static std::vector<std::string> create_rules_file(const std::vector<Rule> &rules);
};
}

#pragma once
#include <string>
#include <vector>
#include "Rule.h"
#include "Template.h"

namespace zindach_openhab_rules_generator {

class FileParser {
public:
    static std::vector<std::string> read_file(const std::string &filename);
    static void write_file(const std::string &filename, const std::vector<std::string> &lines);
    static std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines);
    static std::map<std::string, Template> read_templates_from_file(const std::vector<std::string> &lines);
    static std::vector<std::string> create_rules_file(std::vector<Rule> &rules, std::map<std::string, Template> &templates);
    static std::vector<std::string> group_times(const std::vector<std::string> &lines);
};
}

#pragma once
#include <map>
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

class Template {
    std::vector<std::string> template_lines_;

public:
    Template();
    explicit Template(const std::vector<std::string> &template_lines);

    std::vector<std::string> replace(const std::map<std::string, std::string> &value_map) const;
};
}

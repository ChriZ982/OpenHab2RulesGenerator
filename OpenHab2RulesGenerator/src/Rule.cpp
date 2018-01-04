#include "Rule.h"
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

Rule::Rule(const std::vector<std::string> &keys, const std::string &line) {
    const std::vector<std::string> values = split(line, ";");
    bool empty = true;

    for (size_t i = 0; i < keys.size(); ++i) {
        value_map[keys[i]] = values[i];
        if (empty && !values[i].empty()) {
            empty = false;
        }
    }

    if (empty) {
        throw std::runtime_error("Error: Empty line in rules table!");
    }
}
}

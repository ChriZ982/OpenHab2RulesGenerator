#include "Rule.h"
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

Rule::Rule(const std::vector<std::string> &keys, const std::string &line) {
    std::vector<std::string> values = StringUtils::split(line, ";");
    for (size_t i = 0; i < keys.size(); ++i) {
        value_map.insert_or_assign(keys[i], values[i]);
    }
}
}

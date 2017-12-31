#include "Template.h"
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

Template::Template() {}

Template::Template(const std::vector<std::string> &template_lines)
    : template_lines_(template_lines) {}

std::vector<std::string> Template::replace(const std::map<std::string, std::string> &value_map) const {
    std::vector<std::string> result = template_lines_;
    for (const auto &pair : value_map) {
        for (auto &line : result) {
            line = StringUtils::replace_all(line, StringUtils::convert_template_key(pair.first), pair.second);
        }
    }
    return result;
}
}

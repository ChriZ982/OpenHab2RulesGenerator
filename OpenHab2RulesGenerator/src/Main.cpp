#include <iostream>
#include "FileParser.h"
#include "StringUtils.h"

namespace org = zindach_openhab_rules_generator;

int main() {
    const std::vector<std::string> in_lines = org::FileParser::read_file("rules.csv");
    const std::vector<std::string> template_lines = org::FileParser::read_file(
        org::StringUtils::get_config_value(in_lines, "Regel Vorlagen", ";"));
    std::vector<std::string> fixed_lines = org::FileParser::read_file(
        org::StringUtils::get_config_value(in_lines, "Benutzerdefinierter Inhalt", ";"));

    std::vector<org::Rule> rules = org::FileParser::read_rules_from_file(in_lines);
    std::map<std::string, org::Template> templates = org::FileParser::read_templates_from_file(template_lines);

    std::vector<std::string> out_lines = org::FileParser::create_rules_file(rules, templates);
    out_lines = org::FileParser::group_by_condition(out_lines);
    out_lines = org::FileParser::group_by_if_condition(out_lines);
    out_lines = org::StringUtils::erase_unnecessary_whitespace(out_lines);

    fixed_lines.emplace_back("");
    for (const auto &out_line : out_lines) {
        fixed_lines.push_back(out_line);
    }

    org::FileParser::write_file(org::StringUtils::get_config_value(in_lines, "Dateiname", ";"), fixed_lines);

    std::cout << "Finished...";
    getchar();

    return 0;
}

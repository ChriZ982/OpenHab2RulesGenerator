#include "FileParser.h"
#include <iostream>

namespace org = zindach_openhab_rules_generator;

int main() {
    const std::vector<std::string> in_lines = org::FileParser::read_file("rules.csv");

    const std::vector<org::Rule> rules = org::FileParser::read_rules_from_file(in_lines);

    const std::vector<std::string> out_lines = org::FileParser::create_rules_file(rules);

    const std::string fixed_filename = org::Rule::erase_all(in_lines[2], { "Benutzerdefinierter Inhalt", ";" });
    std::vector<std::string> fixed_lines = org::FileParser::read_file(fixed_filename);
    fixed_lines.emplace_back("");
    for (const auto &out_line : out_lines) {
        fixed_lines.push_back(out_line);
    }

    const std::string out_filename = org::Rule::erase_all(in_lines[1], { "Dateiname", ";" });
    org::FileParser::write_file(out_filename, fixed_lines);

    std::cout << "Finished...";
    getchar();

    return 0;
}

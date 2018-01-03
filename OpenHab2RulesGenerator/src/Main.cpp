#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#include <iostream>
#include "FileParser.h"
#include "StringUtils.h"
#include <chrono>

namespace org = zindach_openhab_rules_generator;

int main() {
    const auto start_nano = std::chrono::system_clock::now();
    std::cout << "OpenHab 2.0 Rules Generator\n---------------------------\n";
    const std::vector<std::string> in_lines = org::FileParser::read_file("rules.csv");

    const std::string template_file = org::StringUtils::get_config_value(in_lines, "Regel Vorlagen", ";");
    const std::string fixed_file = org::StringUtils::get_config_value(in_lines, "Benutzerdefinierter Inhalt", ";");
    const std::string out_file = org::StringUtils::get_config_value(in_lines, "Dateiname", ";");

    const std::vector<std::string> template_lines = org::FileParser::read_file(template_file);
    std::vector<std::string> fixed_lines = org::FileParser::read_file(fixed_file);

    std::cout << "Output file:         " << out_file << "\n";
    std::cout << "Fixed rules file:    " << fixed_file << " (" << fixed_file.size() << " lines)\n";
    std::cout << "Template rules file: " << template_file << " (" << template_lines.size() << " lines)\n\n";

    std::vector<org::Rule> rules = org::FileParser::read_rules_from_file(in_lines);
    std::map<std::string, org::Template> templates = org::FileParser::read_templates_from_file(template_lines);

    std::cout << "(1/6) Creating rules\n";
    std::vector<std::string> out_lines = org::FileParser::create_rules_file(rules, templates);
    out_lines = org::FileParser::group_by_condition(out_lines);
    out_lines = org::FileParser::group_by_if_condition(out_lines);
    std::cout << "(4/6) Erasing unnecessary whitespace\n";
    out_lines = org::StringUtils::erase_unnecessary_whitespace(out_lines);

    std::cout << "(5/6) Adding fixed rules\n";
    fixed_lines.emplace_back("");
    for (const auto &out_line : out_lines) {
        fixed_lines.push_back(out_line);
    }

    std::cout << "(6/6) Writing " << fixed_lines.size() << " lines to output file\n\n";
    org::FileParser::write_file(out_file, fixed_lines);

    std::cout << "Generator took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_nano).count() << "ms\n";
    std::cout << "Finished";
#ifdef PLATFORM_UNIX
    std::cout << "\n";
#endif
    return 0;
}

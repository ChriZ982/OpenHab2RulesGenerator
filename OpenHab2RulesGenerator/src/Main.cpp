#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#include <iostream>
#include "FileParser.h"
#include "StringUtils.h"
#include <chrono>

namespace zohrg = zindach_openhab_rules_generator;

int main() {
    try {
        const auto start_time = std::chrono::system_clock::now();
        std::cout << "OpenHab 2.0 Rules Generator\n---------------------------\n";
        const std::vector<std::string> in_lines = zohrg::read_file("rules.csv");

        const std::string template_file = zohrg::read_config_value(in_lines, "Regel Vorlagen");
        const std::string fixed_file = zohrg::read_config_value(in_lines, "Benutzerdefinierter Inhalt");
        const std::string out_file = zohrg::read_config_value(in_lines, "Dateiname");

        const std::vector<std::string> template_lines = zohrg::read_file(template_file);
        std::vector<std::string> fixed_lines = zohrg::read_file(fixed_file);

        std::cout << "Output file:         " << out_file << "\n";
        std::cout << "Fixed rules file:    " << fixed_file << " (" << fixed_file.size() << " lines)\n";
        std::cout << "Template rules file: " << template_file << " (" << template_lines.size() << " lines)\n\n";

        std::vector<zohrg::Rule> rules = zohrg::read_rules_from_file(in_lines);
        std::map<std::string, zohrg::Template> templates = zohrg::read_templates_from_file(template_lines);

        std::cout << "(1/6) Creating rules\n";
        std::vector<std::string> out_lines = create_rules_file(rules, templates);
        out_lines = zohrg::group_by_condition(out_lines);
        out_lines = zohrg::group_by_if_condition(out_lines);
        std::cout << "(4/6) Erasing unnecessary whitespace\n";
        out_lines = zohrg::erase_unnecessary_whitespace(out_lines);

        std::cout << "(5/6) Adding fixed rules\n";
        fixed_lines.emplace_back("");
        for (const auto &out_line : out_lines) {
            fixed_lines.push_back(out_line);
        }

        std::cout << "(6/6) Writing " << fixed_lines.size() << " lines to output file\n\n";
        zohrg::write_file(out_file, fixed_lines);

        std::cout << "Generator took " << static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now() - start_time).
            count()) / 1000000.0 << " ms\n";
        std::cout << "Finished";

#ifdef PLATFORM_UNIX
        std::cout << "\n";
#endif
        return 0;
    } catch (std::runtime_error error) {
        std::cerr << error.what();

#ifdef PLATFORM_UNIX
        std::cerr << "\n";
#endif
        return -1;
    }

}

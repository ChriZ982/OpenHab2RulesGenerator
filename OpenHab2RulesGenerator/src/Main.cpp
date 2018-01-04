#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#include <iostream>
#include "Parser.h"
#include "StringUtils.h"
#include "File.h"
#include "Optimizer.h"
#include <chrono>

namespace zohrg = zindach_openhab_rules_generator;

int main() {
    try {
        const auto start_time = std::chrono::system_clock::now();

        std::cout << "OpenHab 2.0 Rules Generator\n---------------------------\n";
        const auto in_lines = zohrg::read_file("rules.csv");

        const std::string out_file = zohrg::read_config_value(in_lines, "Dateiname");
        const std::string fixed_file = zohrg::read_config_value(in_lines, "Benutzerdefinierter Inhalt");
        const std::string template_file = zohrg::read_config_value(in_lines, "Regel Vorlagen");

        std::cout << "Output file:         " << out_file << "\n";
        std::cout << "Fixed rules file:    " << fixed_file << "\n";
        std::cout << "Template rules file: " << template_file << "\n\n";
        
        std::cout << "(1/9) Reading rules from file\n";
        auto rules = zohrg::read_rules_from_file(in_lines);
        std::cout << "\tparsed " << rules.size() << " rules\n";

        std::cout << "(2/9) Reading fixed rules from file\n";
        auto fixed_lines = zohrg::read_file(fixed_file);
        std::cout << "\t" << fixed_lines.size() << " lines\n";

        std::cout << "(3/9) Reading templates from file\n";
        const std::vector<std::string> template_lines = zohrg::read_file(template_file);
        std::cout << "\t" << template_lines.size() << " lines\n";
        auto templates = zohrg::read_templates_from_file(template_lines);
        std::cout << "\tparsed " << templates.size() << " templates\n";

        std::cout << "(4/9) Creating rules file\n";
        auto out_lines = create_rules_file(rules, templates);
        
        std::cout << "(5/9) Grouping rules by condition\n";
        out_lines = zohrg::group_by_condition(out_lines);
        
        std::cout << "(6/9) Grouping if statements by condition\n";
        out_lines = zohrg::group_by_if_condition(out_lines);

        std::cout << "(7/9) Erasing unnecessary whitespace\n";
        out_lines = zohrg::erase_unnecessary_whitespace(out_lines);

        std::cout << "(8/9) Adding fixed rules\n";
        fixed_lines.emplace_back("");
        for (const auto &out_line : out_lines) {
            fixed_lines.push_back(out_line);
        }

        std::cout << "(9/9) Writing " << fixed_lines.size() << " lines to output file\n\n";
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

#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#include <chrono>
#include <iostream>
#include "File.h"
#include "Optimizer.h"
#include "Parser.h"
#include "StringUtils.h"

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

        std::cout << "(1/8) Reading templates from file\n";
        const std::vector<std::string> template_lines = zohrg::read_file(template_file);
        std::cout << "\t" << template_lines.size() << " lines\n";
        auto templates = zohrg::read_templates_from_file(template_lines);
        std::cout << "\tparsed " << templates.size() << " templates\n";

        std::cout << "(2/8) Reading rules from file\n";
        auto rules = read_rules_from_file(in_lines, templates);
        std::cout << "\tparsed " << rules.size() << " rules\n";

        std::cout << "(3/8) Reading fixed rules from file\n";
        auto fixed_lines = zohrg::read_file(fixed_file);
        std::cout << "\t" << fixed_lines.size() << " lines\n";

        std::cout << "(4/8) Optimizing rules\n";
        rules = optimize_rules(rules);

        std::cout << "(5/8) Creating rules file\n";
        auto out_lines = create_rules_file(rules);

        std::cout << "(6/8) Erasing unnecessary whitespace\n";
        out_lines = zohrg::erase_unnecessary_whitespace(out_lines);

        std::cout << "(7/8) Adding fixed rules\n";
        out_lines.emplace(out_lines.begin(), "");
        out_lines.insert(out_lines.begin(), fixed_lines.begin(), fixed_lines.end());

        std::cout << "(8/8) Writing " << out_lines.size() << " lines to output file\n\n";
        zohrg::write_file(out_file, out_lines);

        std::cout << "Generator took " << static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now() - start_time).count()) / 1000000.0 << " ms\n";
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

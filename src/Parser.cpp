#include "Parser.h"
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

std::vector<Rule> read_rules_from_file(const std::vector<std::string> &lines, std::map<std::string, Template> &templates) {
    std::vector<Rule> result;
    const size_t table_index = line_index_starts_with(lines, "Tabelle");

    if (table_index == std::string::npos) {
        throw std::runtime_error("Error: Rules table does not have valid format!");
    }

    const std::vector<std::string> keys = split(lines[table_index + 1], ";");
    if (keys.empty() || trim(erase_all(lines[table_index + 1], {";"})).empty()) {
        throw std::runtime_error("Error: Rules table does not have valid header row!");
    }

    for (size_t i = table_index + 2; i < lines.size(); ++i) {
        result.emplace_back(keys, lines[i], templates);
    }

    return result;
}

std::map<std::string, Template> read_templates_from_file(const std::vector<std::string> &lines) {
    std::map<std::string, Template> result;
    size_t template_index = line_index_starts_with(lines, "#template:");

    while (template_index != std::string::npos) {
        size_t next_template_index = line_index_starts_with(lines, "#template:", template_index + 1);
        if (next_template_index == std::string::npos) {
            next_template_index = lines.size();
        }

        result[trim(erase_all(lines[template_index], {"#template:"}))]
            = Template(std::vector<std::string>(lines.begin() + template_index + 1, lines.begin() + next_template_index));

        template_index = line_index_starts_with(lines, "#template:", template_index + 1);
    }

    return result;
}

std::vector<std::string> create_rules_file(std::vector<Rule> &rules) {
    std::vector<std::string> result;

    for (auto &rule : rules) {
        auto rule_lines = rule.to_string();
        result.insert(result.end(), rule_lines.begin(), rule_lines.end());
    }

    return result;
}
}

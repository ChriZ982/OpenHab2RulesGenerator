#include "FileParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

std::vector<std::string> FileParser::read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: File \"" << filename.c_str() << "\" could not be opened!\n";
    }

    std::vector<std::string> result;
    for (std::string line; getline(file, line);) {
        result.emplace_back(StringUtils::erase_all(line, {"\r"}));
    }

    return result;
}

void FileParser::write_file(const std::string &filename, const std::vector<std::string> &lines) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        std::cerr << "Error: File \"" << filename.c_str() << "\" could not be written!\n";
    }

    for (auto line : lines) {
        file.write((line + std::string("\n")).c_str(), line.size() + 1);
    }
}

std::vector<Rule> FileParser::read_rules_from_file(const std::vector<std::string> &lines) {
    std::vector<Rule> result;
    const size_t table_index = StringUtils::get_index_line_begins(lines, "Tabelle", 0);

    std::vector<std::string> keys = StringUtils::split(lines[table_index + 1], ";");

    for (size_t i = table_index + 2; i < lines.size(); ++i) {
        result.emplace_back(keys, lines[i]);
    }

    return result;
}

std::map<std::string, Template> FileParser::read_templates_from_file(const std::vector<std::string> &lines) {
    std::map<std::string, Template> result;

    size_t template_index = StringUtils::get_index_line_begins(lines, "#template:", 0);
    while (template_index != std::string::npos) {
        size_t next_template_index = StringUtils::get_index_line_begins(lines, "#template:", template_index + 1);
        if (next_template_index == std::string::npos) {
            next_template_index = lines.size();
        }

        std::vector<std::string> template_lines;
        for (size_t i = template_index + 1; i < next_template_index; ++i) {
            template_lines.emplace_back(lines[i]);
        }
        result[StringUtils::erase_all(lines[template_index], {"#template:"})] = Template(template_lines);

        template_index = StringUtils::get_index_line_begins(lines, "#template:", template_index + 1);
    }
    return result;
}

std::vector<std::string> FileParser::create_rules_file(std::vector<Rule> &rules,
                                                       std::map<std::string, Template> &templates) {
    std::vector<std::string> result;

    for (auto &rule : rules) {
        const Template temp = templates[rule.value_map[std::string("Aktion")]];
        std::vector<std::string> template_lines = temp.replace(rule.value_map);

        for (auto template_line : template_lines) {
            result.emplace_back(template_line);
        }
    }

    return result;
}
}

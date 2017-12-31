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
    const size_t table_index = StringUtils::get_index_line_begins(lines, "Tabelle");

    std::vector<std::string> keys = StringUtils::split(lines[table_index+1], ";");
    
    for (size_t i = table_index+2; i < lines.size(); ++i) {
        result.emplace_back(keys, lines[i]);
    }

    return result;
}

std::vector<std::string> FileParser::create_rules_file(const std::vector<Rule> &rules) {
    return std::vector<std::string>();
}

std::stringstream FileParser::get_commands(const Rule &rule) {
    return std::stringstream();
}
}

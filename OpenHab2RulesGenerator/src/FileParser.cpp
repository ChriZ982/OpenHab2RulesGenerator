#include "FileParser.h"
#include <fstream>
#include <iostream>

namespace zindach_openhab_rules_generator {

std::vector<std::string> FileParser::read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: File \"" << filename.c_str() << "\" could not be opened!";
    }
    std::vector<std::string> result;
    for (std::string line; std::getline(file, line); ){
        result.push_back(line);
    }
    return result;
}

std::vector<Rule> FileParser::read_rules_from_file(const std::string &filename) {
    std::vector<std::string> lines = read_file(filename);
    return std::vector<Rule>();
}
}

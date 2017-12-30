#include "FileParser.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace zindach_openhab_rules_generator {

std::vector<std::string> FileParser::read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: File \"" << filename.c_str() << "\" could not be opened!\n";
    }
    std::vector<std::string> result;
    for (std::string line; getline(file, line);) {
        result.push_back(Rule::erase_all(line, {"\n", "\r"}));
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
    for (size_t i = 5; i < lines.size(); ++i) {
        result.emplace_back(lines[i]);
    }
    return result;
}

std::vector<std::string> FileParser::create_rules_file(const std::vector<Rule> &rules) {
    std::vector<std::string> lines;
    for (const auto &rule : rules) {
        std::stringstream line;

        line << "rule \"" << rule.item << " " << rule.value << " " << rule.days << " " << rule.day_type << " " << rule.hour
            << ":" << (rule.minute.length() == 1 ? "0" : "") << rule.minute << "\"\n";
        line << "when\n";
        if (rule.days == "SUNSET") {
            line << "    Channel \"astro:sun:68b99d43 : set#event\" triggered START\n";
        } else if (rule.days == "MON-SUN") {
            line << "    Time cron \"0 " << rule.minute << " " << rule.hour << " * * ?\"\n";
        } else {
            line << "    Time cron \"0 " << rule.minute << " " << rule.hour << " ? * " << rule.days << "\"\n";
        }
        line << "then\n";
        line << get_commands(rule).str();
        line << "end\n\n";

        lines.push_back(line.str());
    }
    return lines;
}

std::stringstream FileParser::get_commands(const Rule &rule) {
    std::stringstream line;
    if(value_keywords.find(rule.value) == value_keywords.end()) {
        return line;
    }
    switch (value_keywords.at(rule.value)) {
        case 0:
            break;
        case 1:
            line << "    if(Automatik_Heizung_ein_" << rule.day_type << ".state==ON) {\n";
            line << "        sendCommand(" << rule.item << ", 21)\n";
            line << "    }\n";
            break;
        case 2:
            line << "    if(Automatik_Heizung_aus_" << rule.day_type << ".state==ON) {\n";
            line << "        sendCommand(" << rule.item << ", 21)\n";
            line << "    }\n";
            break;
        case 3:
            line << "    if(Automatik_Rolladen_rauf.state==ON) {\n";
            line << "        sendCommand(" << rule.item << ", UP)\n";
            line << "    }\n";
            break;
        case 4:
            line << "    if(Automatik_Rolladen_runter.state==ON) {\n";
            line << "        sendCommand(" << rule.item << ", DOWN)\n";
            line << "    }\n";
            break;
        case 5:
            line << "    if(" << rule.item << ".state > 95) {\n";
            line << "        sendCommand(" << rule.item << ", 90)\n";
            line << "    }\n";
            break;
        default:
            std::cerr << "Error: Value has unknown value \"" << rule.value << "\"!\n";
    }
    return line;
}
}

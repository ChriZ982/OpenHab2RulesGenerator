#include "Rule.h"
#include <iostream>

namespace zindach_openhab_rules_generator {

Rule::Rule(const std::string &line) {
    std::vector<std::string> values;

    size_t start_index = 0;
    size_t end_index = line.find(';', start_index);
    while (end_index != std::string::npos) {
        values.emplace_back(line.substr(start_index, end_index - start_index));
        start_index = end_index + 1;
        end_index = line.find(';', start_index);
        if (end_index == std::string::npos && start_index != line.length() + 1) {
            end_index = line.length();
        }
    }

    if (values.size() < 6) {
        std::cerr << "Error: Row only contains " << values.size() << " columns!\n";
        return;
    }

    item = values[0];
    days = values[1];
    hour = values[2];
    minute = values[3];
    day_type = values[4];
    value = values[5];

    item = erase_all(item, { " " });
    days = erase_all(days, { " " });
    hour = erase_all(hour, { " " });
    minute = erase_all(minute, { " " });
    day_type = erase_all(day_type, { " " });
    value = erase_all(value, { " " });

    perform_syntax_check();

    days = replace_all(days, "MO", "MON");
    days = replace_all(days, "DI", "TUE");
    days = replace_all(days, "MI", "WED");
    days = replace_all(days, "DO", "THU");
    days = replace_all(days, "FR", "FRI");
    days = replace_all(days, "SA", "SAT");
    days = replace_all(days, "SO", "SUN");
}

void Rule::perform_syntax_check() const {
    std::string days_test = erase_all(days, {"MO", "DI", "MI", "DO", "FR", "SA", "SO", "-", "," , "SUNSET" });
    if (!days_test.empty()) {
        std::cerr << "Warning: Days contains unknown characters \"" << days_test << "\"!\n";
    }

    std::string hour_test = erase_all(hour, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0" });
    if (!hour_test.empty()) {
        std::cerr << "Warning: Hour contains unknown characters \"" << hour_test << "\"!\n";
    }

    std::string minute_test = erase_all(minute, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" });
    if (!minute_test.empty()) {
        std::cerr << "Warning: Minute contains unknown characters \"" << minute_test << "\"!\n";
    }

    std::string day_type_test = erase_all(day_type, {"Normaltag", "Feiertag"});
    if (!day_type_test.empty()) {
        std::cerr << "Warning: Day type contains unknown characters \"" << day_type_test << "\"!\n";
    }
    if(day_type != "Normaltag" && day_type != "Feiertag") {
        std::cerr << "Error: Day type has unknown value \"" << day_type << "\"!\n";
    }

    if (value_keywords.find(value) == value_keywords.end()) {
        std::cerr << "Error: Value has unknown value \"" << value << "\"!\n";
    }

    if (item.empty() || days.empty() || hour.empty() || minute.empty() || day_type.empty() || value.empty()) {
        std::cerr << "Warning: At least one cell is empty!\n";
    }
}

std::string Rule::replace_all(const std::string &text, const std::string &from, const std::string &to) {
    std::string result = text;
    size_t last_replace = 0;
    while ((last_replace = result.find(from, last_replace)) != std::string::npos) {
        result.replace(last_replace, from.length(), to);
        last_replace += to.length();
    }
    return result;
}

std::string Rule::erase_all(const std::string &text, const std::vector<std::string> &values) {
    std::string result = text;
    for (const auto &val : values) {
        result = replace_all(result, val, "");
    }
    return result;
}
}

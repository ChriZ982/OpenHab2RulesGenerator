#pragma once
#include <string>
#include <vector>
#include <map>

namespace zindach_openhab_rules_generator {

static const std::map<std::string, int> value_keywords = {
    {"LICHT_EIN",0},{"HEIZUNG_EIN",1},{"HEIZUNG_AUS",2},{"ROLLADEN_RAUF",3},{"ROLLADEN_RUNTER",4},{"ROLLADEN_RITZE",5}
};

class Rule {
public:
    std::string item;
    std::string days;
    std::string hour;
    std::string minute;
    std::string day_type;
    std::string value;

    explicit Rule(const std::string &line);
    void perform_syntax_check() const;
    static std::string replace_all(const std::string &text, const std::string &from, const std::string &to);
    static std::string erase_all(const std::string &text, const std::vector<std::string> &values);
};
}

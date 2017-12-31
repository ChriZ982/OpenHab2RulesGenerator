﻿#pragma once
#include <map>
#include <string>
#include <vector>

namespace zindach_openhab_rules_generator {

class Rule {
public:
    std::map<std::string, std::string> value_map;

    Rule(const std::vector<std::string> &keys, const std::string &line);
};
}

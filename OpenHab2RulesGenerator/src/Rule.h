#pragma once
#include <map>
#include <string>
#include <vector>
#include "Template.h"

namespace zindach_openhab_rules_generator {

class Rule {
    std::string name_;

    void sub_if_statement_indices(const size_t begin, const size_t change);
    void add_if_statement_indices(const size_t begin, const size_t change);

public:
    std::string condition;
    std::vector<std::string> lines;
    std::map<std::string, std::pair<size_t, size_t>> if_statements;

    Rule(const std::vector<std::string> &keys, const std::string &line, std::map<std::string, Template> &templates);

    void rename();
    void merge_rule(Rule &other);
    std::vector<std::string> to_string();
};
}

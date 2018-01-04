#include "Optimizer.h"
#include <iostream>

namespace zindach_openhab_rules_generator {

std::vector<Rule> optimize_rules(std::vector<Rule> rules) {
    std::map<std::string, std::vector<size_t>> rule_map;
    for (size_t i = 0; i < rules.size(); ++i) {
        rule_map[rules[i].condition].emplace_back(i);
    }

    for (auto iterator = rule_map.begin(); iterator != rule_map.end();) {
        if (iterator->second.size() <= 1) {
            iterator = rule_map.erase(iterator);
        } else {
            ++iterator;
        }
    }
    std::cout << "\toptimizing " << rule_map.size() << " rules\n";

    for (auto &pair : rule_map) {
        for (size_t i = 1; i < pair.second.size(); ++i) {
            rules[pair.second[0]].rename();
            rules[pair.second[0]].merge_rule(rules[pair.second[i]]);

            rules.erase(rules.begin() + pair.second[i], rules.begin() + pair.second[i] + 1);

            for (auto &rule : rule_map) {
                for (auto &index : rule.second) {
                    if (index > pair.second[i]) {
                        --index;
                    }
                }
            }
        }
    }

    return rules;
}
}

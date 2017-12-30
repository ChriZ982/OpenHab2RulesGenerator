#include "FileParser.h"

namespace org = zindach_openhab_rules_generator;

int main() {
    std::vector<org::Rule> rules = org::FileParser::read_rules_from_file("rules.csv");
    getchar();
    return 0;
}

#include "Rule.h"
#include "StringUtils.h"

namespace zindach_openhab_rules_generator {

Rule::Rule(const std::vector<std::string> &keys, const std::string &line, std::map<std::string, Template> &templates) {
    const std::vector<std::string> values = split(line, ";");
    std::map<std::string, std::string> value_map;

    bool empty = true;

    for (size_t i = 0; i < keys.size(); ++i) {
        value_map[keys[i]] = values[i];
        if (empty && !values[i].empty()) {
            empty = false;
        }
    }

    if (empty) {
        throw std::runtime_error("Error: Empty line in rules table!");
    }

    lines = templates[value_map["Aktion"]].replace(value_map);
    const size_t when_index = line_index_equals(lines, "when");
    const size_t then_index = line_index_equals(lines, "then", when_index + 1);

    name_ = join_values(value_map);

    for (size_t i = when_index + 1; i < then_index; ++i) {
        condition += "\t" + trim(lines[i]) + "\n";
    }

    lines.erase(lines.begin(), lines.begin() + then_index + 1);

    const size_t end_index = line_index_equals(lines, "end");
    lines.erase(lines.begin() + end_index, lines.end());

    for (size_t i = 0; i < lines.size(); ++i) {
        std::string trim_line = trim(lines[i]);

        if (trim_line.find("if") == 0 && trim_line.find_last_of("{") == trim_line.size() - 1) {
            size_t closing_brace = find_closing_brace(lines, i);
            if_statements[trim_line] = {i, closing_brace};
            i = closing_brace;
        }
    }
}

void Rule::rename() {
    name_ = "Group " + trim(erase_all(condition, {"*", "\"", "?", ":", "#", "\r", "\n"}));
}

void Rule::merge_rule(Rule &other) {
    for (const auto &own_if_statement : if_statements) {
        const auto if_statement = other.if_statements.find(own_if_statement.first);
        if (if_statement != other.if_statements.end()) {
            lines.insert(lines.begin() + own_if_statement.second.second, "\t\tThread::sleep(2000)");
            lines.insert(lines.begin() + own_if_statement.second.second + 1,
                         other.lines.begin() + if_statement->second.first + 1,
                         other.lines.begin() + if_statement->second.second);

            add_if_statement_indices(own_if_statement.second.first,
                                     if_statement->second.second - if_statement->second.first);

            other.lines.erase(other.lines.begin() + if_statement->second.first,
                              other.lines.begin() + if_statement->second.second + 1);

            other.sub_if_statement_indices(if_statement->second.first - 1,
                                           if_statement->second.second - if_statement->second.first + 1);
        }
    }
    other.add_if_statement_indices(0, lines.size());

    for (auto if_statement : other.if_statements) {
        if_statements.insert(if_statement);
    }

    lines.insert(lines.end(), other.lines.begin(), other.lines.end());
}

void Rule::sub_if_statement_indices(const size_t begin, const size_t change) {
    for (auto &if_statement : if_statements) {
        if (if_statement.second.first > begin) {
            if_statement.second.first -= change;
        }
        if (if_statement.second.second > begin) {
            if_statement.second.second -= change;
        }
    }

}

void Rule::add_if_statement_indices(const size_t begin, const size_t change) {
    for (auto &if_statement : if_statements) {
        if (if_statement.second.first > begin) {
            if_statement.second.first += change;
        }
        if (if_statement.second.second > begin) {
            if_statement.second.second += change;
        }
    }
}

std::vector<std::string> Rule::to_string() {
    lines.insert(lines.begin(), "rule \"" + name_ + "\"\nwhen\n" + condition + "then");
    lines.insert(lines.end(), "end\n");

    return lines;
}
}

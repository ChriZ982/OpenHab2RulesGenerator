#include "File.h"
#include <fstream>

namespace zindach_openhab_rules_generator {

std::vector<std::string> read_file(const std::string &filename) {
    std::wifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: File \"" + filename + "\" could not be opened for reading!");
    }

    std::vector<std::string> result;

    for (std::wstring line; getline(file, line);) {
#ifdef PLATFORM_UNIX
        result.emplace_back(erase_all(std::string(line.begin(), line.end()), { "\r" }));
#else
        result.emplace_back(line.begin(), line.end());
#endif
    }

    return result;
}

void write_file(const std::string &filename, const std::vector<std::string> &lines) {
    std::wofstream file(filename, std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Error: File \"" + filename + "\" could not be opened for writing!");
    }

    for (const auto &line : lines) {
#ifdef PLATFORM_UNIX
        file << std::wstring(line.begin(), line.end()) << "\r\n";
#else
        file << std::wstring(line.begin(), line.end()) << "\n";
#endif
    }
}
}

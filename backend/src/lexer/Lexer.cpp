#include "Lexer.h"
#include <regex>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

TokenizedLine tokenizeLine(const std::string& rawLine) {
    TokenizedLine result;
    std::string line = trim(rawLine);

    if (line.empty() || line[0] == '#') {
        return result;
    }

    size_t firstSpace = line.find(' ');
    result.command = (firstSpace == std::string::npos) ? line : line.substr(0, firstSpace);
    std::string rest = (firstSpace == std::string::npos) ? "" : line.substr(firstSpace + 1);

    std::regex paramPattern("-(\\w+)=(?:\"([^\"]*)\"|(\\S+))");
    auto begin = std::sregex_iterator(rest.begin(), rest.end(), paramPattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;

        std::string key = match[1].str();
        for (auto& c : key) c = std::tolower(c);

        std::string value = match[2].matched ? match[2].str() : match[3].str();

        result.params.push_back({key, value});
    }

    return result;
}
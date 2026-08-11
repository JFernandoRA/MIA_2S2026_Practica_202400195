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
    std::regex paramPattern("-(\\w+)(?:=(?:\"([^\"]*)\"|(\\S+)))?");
    auto begin = std::sregex_iterator(rest.begin(), rest.end(), paramPattern);
    auto end = std::sregex_iterator();

    size_t lastEnd = 0;
    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;

        std::string gap = rest.substr(lastEnd, match.position() - lastEnd);
        std::string trimmedGap = trim(gap);
        if (!trimmedGap.empty()) {
            result.unrecognized.push_back(trimmedGap);
        }

        std::string key = match[1].str();
        for (auto& c : key) c = std::tolower(c);

        std::string value;
        if (match[2].matched) value = match[2].str();
        else if (match[3].matched) value = match[3].str();

        result.params.push_back({key, value});
        lastEnd = match.position() + match.length();
    }

    std::string trailing = trim(rest.substr(lastEnd));
    if (!trailing.empty()) {
        result.unrecognized.push_back(trailing);
    }

    return result;
}
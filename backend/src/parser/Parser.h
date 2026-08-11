#pragma once
#include <string>
#include <vector>


struct AnalysisResult {
    int line;
    std::string raw;
    std::string command;
    std::vector<std::string> tokens;
    std::string status;   // "ok" , "lex_error" , "syntax_error" , "empty"
    std::string message;
};

AnalysisResult analyzeLine(const std::string& rawLine, int lineNumber);
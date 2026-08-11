#pragma once
#include <string>
#include <vector>

struct Param {
    std::string key;
    std::string value;
};

struct TokenizedLine {
    std::string command;
    std::vector<Param> params;
    std::vector<std::string> unrecognized;
};
#pragma once
#include <string>
#include <vector>
#include <map>

struct CommandSpec {
    std::vector<std::string> required;
    std::map<std::string, std::vector<std::string>> optional;
    std::map<std::string, std::string> defaults; 
};

const std::map<std::string, CommandSpec>& getCommandSpecs();
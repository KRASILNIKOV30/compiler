#pragma once
#include <string>
#include <vector>

namespace raw
{
using Alternative = std::vector<std::string>;
using Alternatives = std::vector<Alternative>;
using Rules = std::vector<std::pair<std::string, Alternatives>>;
}

raw::Rules ParseRawRules(std::stringstream& input);

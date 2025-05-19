#pragma once
#include "../table/Table.h"
#include <string>
#include <vector>

namespace raw
{
using Alternative = std::vector<std::string>;
using Alternatives = std::vector<Alternative>;
using Rules = std::vector<std::pair<std::string, Alternatives>>;
} // namespace raw

struct Alternative
{
	std::vector<std::string> rule;
	Guides guides;
};

using Alternatives = std::vector<Alternative>;

struct Rule
{
	std::string name;
	Alternatives alternatives;
};

using Rules = std::vector<Rule>;

raw::Rules ParseRawRules(std::stringstream& input);
Rules ParseRules(std::stringstream& input);

#pragma once
#include "../../table/Table.h"
#include <string>
#include <vector>

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

[[nodiscard]] Rules ParseRules(std::stringstream& input);

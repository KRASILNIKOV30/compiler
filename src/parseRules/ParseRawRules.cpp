#include "../table/Table.h"
#include "ParseRules.h"
#include <algorithm>
#include <sstream>

namespace
{
const std::string DELIMITER = "|";

raw::Alternatives GetRuleAlternatives(std::istream& str)
{
	raw::Alternatives alternatives;
	raw::Alternative alternative;
	std::string lexeme;
	while (str >> lexeme)
	{
		if (lexeme == DELIMITER)
		{
			alternatives.push_back(alternative);
			alternative.clear();
			continue;
		}
		alternative.emplace_back(lexeme);
	}

	if (!alternative.empty())
	{
		alternatives.push_back(alternative);
	}

	return alternatives;
}

}

raw::Rules ParseRawRules(std::stringstream& input)
{
	raw::Rules rules{};
	std::string rule;
	while (std::getline(input, rule))
	{
		if (rule.empty())
		{
			continue;
		}

		std::istringstream iss(rule);
		std::string nonTerm;
		iss >> nonTerm;

		if (IsTerm(nonTerm))
		{
			throw std::runtime_error("Terminal at left side found: " + nonTerm);
		}

		std::string minus;
		iss >> minus;

		const auto alternatives = GetRuleAlternatives(iss);

		auto it = std::ranges::find_if(rules, [&](const auto& r) {
			return r.first == nonTerm;
		});
		if (it == rules.end())
		{
			rules.emplace_back(nonTerm, alternatives);
		}
		else
		{
			for (const auto& alternative : alternatives)
			{
				it->second.push_back(alternative);
			}
		}
	}

	return rules;
}

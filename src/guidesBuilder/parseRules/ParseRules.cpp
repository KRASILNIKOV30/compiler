#include "ParseRules.h"
#include <sstream>
#include <algorithm>

using namespace raw;

std::vector<std::string> GetRuleRightSide(std::istream& str)
{
	std::string lexeme;
	std::vector<std::string> result;
	while (str >> lexeme && lexeme != "/")
	{
		result.emplace_back(lexeme);
	}

	return result;
}

Rules ParseRawRules(std::stringstream& input)
{
	Rules rules{};
	std::string rule;
	while (std::getline(input, rule))
	{
		std::istringstream iss(rule);
		std::string nonTerm;
		iss >> nonTerm;

		std::string minus;
		iss >> minus;

		const auto alternative = GetRuleRightSide(iss);

		auto it = std::ranges::find_if(rules, [&](const auto& r) {
			return r.first == nonTerm;
		});
		if (it == rules.end())
		{
			rules.push_back({ nonTerm, { alternative } });
		}
		else
		{
			it->second.push_back(alternative);
		}
	}

	return rules;
}
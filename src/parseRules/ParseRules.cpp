#include "ParseRules.h"
#include <sstream>

Guides GetGuides(std::istream& str)
{
	Guides result;
	std::string token;
	while (str >> token)
	{
		result.emplace(token);
	}

	return result;
}

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

Alternative GetAlternative(std::istream& str)
{
	const auto rightSide = GetRuleRightSide(str);
	const auto guides = GetGuides(str);

	return { rightSide, guides };
}

Rules ParseRules(std::stringstream& input)
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

		const auto alternative = GetAlternative(iss);

		auto it = std::ranges::find_if(rules, [&](const auto& r) {
			return r.name == nonTerm;
		});
		if (it == rules.end())
		{
			rules.push_back({ nonTerm, { alternative } });
		}
		else
		{
			it->alternatives.push_back(alternative);
		}
	}

	return rules;
}
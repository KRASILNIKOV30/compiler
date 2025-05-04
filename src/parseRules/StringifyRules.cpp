#include "StringifyRules.h"
#include <sstream>
#include "../print/PrintContainer.h"

std::string StringifyRules(raw::Rules const& rules)
{
	std::ostringstream output;

	for (const auto& [nonTerm, alternatives] : rules)
	{
		output << nonTerm << " -";
		for (size_t i = 0; i < alternatives.size(); ++i)
		{
			output << alternatives[i];

			if (i != alternatives.size() - 1)
			{
				output << " |";
			}
		}
		output << std::endl;
	}

	return output.str();
}

std::string StringifyRules(Rules const& rules)
{
	std::ostringstream output;

	for (const auto& [name, alternatives] : rules)
	{
		for (const auto& [alternative, guides] : alternatives)
		{
			output << name << " -" << alternative << " /" << guides << std::endl;
		}
	}

	return output.str();
}

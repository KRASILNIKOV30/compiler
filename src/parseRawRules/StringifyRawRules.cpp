#include "StringifyRawRules.h"
#include <sstream>

std::string StringifyRawRules(raw::Rules const& rules)
{
	std::ostringstream output;

	for (const auto& [nonTerm, alternatives] : rules)
	{
		output << nonTerm << " - ";
		for (size_t i = 0; i < alternatives.size(); ++i)
		{
			const auto& alternative = alternatives[i];
			for (size_t j = 0; j < alternative.size(); ++j)
			{
				output << alternative[j];
				if (j != alternative.size() - 1)
				{
					output << " ";
				}
			}

			if (i != alternatives.size() - 1)
			{
				output << " | ";
			}
		}
		output << std::endl;
	}

	return output.str();
}
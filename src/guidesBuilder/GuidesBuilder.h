#pragma once
#include "../tableBuilder/TableBuilder.h"
#include <optional>
#include <string>
#include <sstream>
#include <vector>

namespace raw
{
using Alternative = std::vector<std::string>;
using Alternatives = std::vector<Alternative>;
using Rules = std::vector<std::pair<std::string, Alternatives>>;
}

class GuidesBuilder
{
public:
	explicit GuidesBuilder(std::string const& str)
		: m_input(str)
	{
		ParseRules();
	}

	explicit GuidesBuilder(std::istream const& strm)
	{
		m_input << strm.rdbuf();
		ParseRules();
	}

	std::optional<Rules> BuildGuidedRules();

private:
	void ParseRules();

private:
	std::stringstream m_input;
};

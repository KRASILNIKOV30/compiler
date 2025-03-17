#pragma once
#include "../tableBuilder/TableBuilder.h"
#include "parseRules/ParseRules.h"
#include <optional>
#include <string>
#include <sstream>
#include <vector>

class GuidesBuilder
{
public:
	explicit GuidesBuilder(std::string const& str)
	{
		std::stringstream ss(str);
		m_rules = ParseRawRules(ss);
	}

	explicit GuidesBuilder(std::istream const& strm)
	{
		std::stringstream ss;
		ss << strm.rdbuf();
		m_rules = ParseRawRules(ss);
	}

	std::optional<Rules> BuildGuidedRules();

private:

private:
	raw::Rules m_rules;
};

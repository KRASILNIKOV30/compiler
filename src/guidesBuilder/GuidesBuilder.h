#pragma once
#include "../tableBuilder/TableBuilder.h"
#include "parseRules/ParseRules.h"
#include <optional>
#include <string>
#include "../print/PrintContainer.h"

class GuidesBuilder
{
public:
	explicit GuidesBuilder(std::string const& str);
	explicit GuidesBuilder(std::istream const& strm);
	std::optional<std::string> BuildGuidedRules();

private:
	void Init();
	Guides GetGuides(std::string const& nonTerm);
	void BuildRelationFirst();
	std::unordered_set<std::string> GetFollow(std::string const& nonTerm) const;
	void TransitiveClosure();

private:
	raw::Rules m_rules;
	std::unordered_set<std::string> m_nonTerms;
	std::unordered_set<std::string> m_lexemes;
	std::unordered_map<std::string, std::unordered_set<std::string>> m_guides;
};
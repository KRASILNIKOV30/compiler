#pragma once
#include "../parseRules/ParseRules.h"
#include "../print/PrintContainer.h"
#include "../tableBuilder/TableBuilder.h"
#include <string>

class GuidesBuilder
{
public:
	explicit GuidesBuilder(std::string const& str);
	explicit GuidesBuilder(std::istream const& strm);
	Rules BuildGuidedRules();

private:
	[[nodiscard]] Rules GetRules();
	void Init();
	void BuildRelationFirst();
	void TransitiveClosure();

private:
	raw::Rules m_rules;
	std::unordered_set<std::string> m_nonTerms;
	std::unordered_set<std::string> m_lexemes;
	std::unordered_map<std::string, std::unordered_set<std::string>> m_guides;
};
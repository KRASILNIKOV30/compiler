#include "GuidesBuilder.h"
#include "emptyRulesDeleter/EmptyRulesDeleter.h"
#include <sstream>

GuidesBuilder::GuidesBuilder(std::string const& str)
{
	std::stringstream ss(str);
	m_rules = ParseRawRules(ss);
	Init();
}

GuidesBuilder::GuidesBuilder(std::istream const& strm)
{
	std::stringstream ss;
	ss << strm.rdbuf();
	m_rules = ParseRawRules(ss);
	Init();
}

Rules GuidesBuilder::BuildGuidedRules()
{
	EmptyRulesDeleter deleter(m_rules);
	m_rules = deleter.DeleteEmptyRules();
	BuildRelationFirst();
	TransitiveClosure();
	return GetRules();
}

void AddRule(Rules& rules, std::string const& left, std::vector<std::string> const& alternative, Guides const& guides)
{
	const auto it = std::ranges::find_if(rules, [&](const auto& r) {
		return r.name == left;
	});
	if (it == rules.end())
	{
		rules.push_back({
			.name = left,
			.alternatives = { { alternative, guides } },
		});
	}
	else
	{
		it->alternatives.push_back({ alternative, guides });
	}
}

Rules GuidesBuilder::GetRules()
{
	Rules rules{};
	std::stringstream ss;
	for (const auto& [left, alternatives] : m_rules)
	{
		for (const auto& alternative : alternatives)
		{
			Guides alternativeGuides{};
			const auto& first = alternative.front();
			alternativeGuides.insert(first);
			if (!IsTerm(first))
			{
				const auto& guides = m_guides[first];
				alternativeGuides.insert(guides.begin(), guides.end());
			}
			AddRule(rules, left, alternative, alternativeGuides);
		}
	}

	return rules;
}

void GuidesBuilder::Init()
{
	for (const auto& [left, alternatives] : m_rules)
	{
		m_nonTerms.emplace(left);
		m_lexemes.emplace(left);
		for (auto const& alternative : alternatives)
		{
			for (auto const& lexeme : alternative)
			{
				m_lexemes.emplace(lexeme);
			}
		}
	}
}

void GuidesBuilder::BuildRelationFirst()
{
	for (const auto& [left, alternatives] : m_rules)
	{
		for (auto const& alternative : alternatives)
		{
			m_guides[left].insert(alternative[0]);
		}
	}
}

void GuidesBuilder::TransitiveClosure()
{
	for (const auto& k : m_lexemes)
	{
		for (const auto& nonTerm : m_nonTerms)
		{
			for (const auto& lexeme : m_lexemes)
			{
				if (m_guides[nonTerm].contains(k) && m_guides.contains(k) && m_guides[k].contains(lexeme))
				{
					m_guides[nonTerm].insert(lexeme);
				}
			}
		}
	}
}
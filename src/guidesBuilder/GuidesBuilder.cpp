#include "GuidesBuilder.h"

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

std::optional<std::string> GuidesBuilder::BuildGuidedRules()
{
	BuildRelationFirst();
	TransitiveClosure();

	std::stringstream ss;
	for (const auto& [left, alternatives] : m_rules)
	{
		auto nonTermGuides = GetGuides(left);
		for (const auto& alternative : alternatives)
		{
			Guides alternativeGuides{};
			const auto& first = alternative[0];
			if (first == EMPTY)
			{
				continue;
			}
			if (!IsTerm(first))
			{
				const auto guides = GetGuides(first);
				for (const auto& guide : guides)
				{
					alternativeGuides.emplace(guide);
					nonTermGuides.extract(guide);
				}
			}
			else
			{
				alternativeGuides.emplace(first);
				nonTermGuides.extract(first);
			}

			ss << left << " -" << alternative << " /" << alternativeGuides << std::endl;
		}

		for (const auto& alternative : alternatives
		     | std::views::filter([](const auto& alt) { return alt[0] == EMPTY; }))
		{
			ss << left << " -" << alternative << " /" << nonTermGuides << std::endl;
		}
	}

	return ss.str();
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

Guides GuidesBuilder::GetGuides(std::string const& nonTerm)
{
	Guides result{};
	for (const auto& guide : m_guides[nonTerm]
	     | std::views::filter([&](const auto& l) { return IsTerm(l); }))
	{
		result.emplace(guide);
	}

	return result;
}

void GuidesBuilder::BuildRelationFirst()
{
	for (const auto& [left, alternatives] : m_rules)
	{
		for (auto const& alternative : alternatives)
		{
			const auto& guide = alternative[0];
			std::unordered_set<std::string> guides = guide == EMPTY
				? GetFollow(left)
				: std::unordered_set{ guide };

			m_guides[left].merge(guides);
		}
	}
}

std::unordered_set<std::string> GuidesBuilder::GetFollow(std::string const& nonTerm) const
{
	std::unordered_set<std::string> followLexemes;
	for (const auto& [left, alternatives] : m_rules)
	{
		for (auto const& alternative : alternatives)
		{
			for (int i = 0; i < alternative.size(); ++i)
			{
				if (alternative[i] != nonTerm)
				{
					continue;
				}
				const bool isLast = i == alternative.size() - 1;
				std::unordered_set<std::string> follow = isLast
					? left != nonTerm
					? GetFollow(left)
					: std::unordered_set<std::string>{}
					: std::unordered_set{ alternative[i + 1] };

				followLexemes.merge(follow);
			}
		}
	}

	return followLexemes;
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
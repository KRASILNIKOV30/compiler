#pragma once
#include "../tableBuilder/TableBuilder.h"
#include "parseRules/ParseRules.h"
#include <optional>
#include <string>
#include <sstream>
#include <ranges>
#include "../print/PrintContainer.h"

class GuidesBuilder
{
public:
	explicit GuidesBuilder(std::string const& str)
	{
		std::stringstream ss(str);
		m_rules = ParseRawRules(ss);
		Init();
	}

	explicit GuidesBuilder(std::istream const& strm)
	{
		std::stringstream ss;
		ss << strm.rdbuf();
		m_rules = ParseRawRules(ss);
		Init();
	}

	std::optional<std::string> BuildGuidedRules()
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

private:
	void Init()
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


	[[nodiscard]] Guides GetGuides(std::string const& nonTerm)
	{
		Guides result{};
		for (const auto& guide : m_guides[nonTerm]
		     | std::views::filter([&](const auto& l) { return IsTerm(l); }))
		{
			result.emplace(guide);
		}

		return result;
	}

	void BuildRelationFirst()
	{
		for (const auto& [left, alternatives] : m_rules)
		{
			for (auto const& alternative : alternatives)
			{
				const auto guide = alternative[0];
				std::unordered_set<std::string> guides = guide == EMPTY
					? GetFollow(left)
					: std::unordered_set{ guide };

				m_guides[left].merge(guides);
			}
		}
	}

	[[nodiscard]] std::unordered_set<std::string> GetFollow(std::string const& nonTerm) const
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

	void TransitiveClosure()
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

	static bool IsTerm(std::string const& term)
	{
		return term[0] != '<';
	}

private:
	raw::Rules m_rules;
	std::unordered_set<std::string> m_nonTerms;
	std::unordered_set<std::string> m_lexemes;
	std::unordered_map<std::string, std::unordered_set<std::string>> m_guides;
};

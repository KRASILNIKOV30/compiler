#pragma once
#include "../table/Table.h"
#include <map>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>

const std::string EMPTY = "e";

/**
 * input format:
 * <S> - <A> / a,c
 * <S> - b <B> / b
 * <A> - a / a
 * <A> - c / c
 * <B> - b / b
 */
class TableBuilder
{
public:
	explicit TableBuilder(std::string const& str)
		: m_input(str)
	{
		ParseRules();
	}

	explicit TableBuilder(std::istream const& strm)
	{
		m_input << strm.rdbuf();
		ParseRules();
	}

	[[nodiscard]] Table BuildTable() const
	{
		Table table;
		FillLeftNonTerms(table);
		FillRightSides(table);

		return table;
	}

private:
	using Alternative = std::pair<std::vector<std::string>, Guides>;
	using Alternatives = std::vector<Alternative>;
	using Rules = std::vector<std::pair<std::string, Alternatives>>;

	void FillLeftNonTerms(Table& table) const
	{
		auto ptr = GetLeftSideNonTermsNumber();
		for (const auto& [name, alternatives] : m_rules)
		{
			for (const auto& [rule, guides] : alternatives)
			{
				table.emplace_back(TableRow{
					.symbol = name,
					.guides = guides,
					.shift = false,
					.error = false,
					.ptr = ptr,
					.stack = false,
					.end = false,
				});
				ptr += rule.size();
			}
			table.back().error = true;
		}
	}

	void FillRightSides(Table& table) const
	{
		bool isAxiom = true;
		for (const auto& alternatives : m_rules | std::ranges::views::values)
		{
			for (const auto& alternative : alternatives)
			{
				FillAlternative(table, alternative, isAxiom);
				isAxiom = false;
			}
		}
	}

	void FillAlternative(Table& table, Alternative const& alternative, bool isAxiom) const
	{
		const auto [rule, guides] = alternative;
		for (const auto& symbol : rule)
		{
			TableRow row{};
			if (IsTerm(symbol))
			{
				if (symbol == EMPTY)
				{
					row.symbol = EMPTY;
					row.guides = guides;
					row.shift = false;
					row.error = true;
					row.ptr = std::nullopt;
					row.stack = false;
					row.end = false;
				}
				else
				{
					row.symbol = symbol;
					row.guides = { symbol };
					row.shift = true;
					row.error = true;
					row.ptr = table.size() + 1;
					row.stack = false;
					row.end = false;
				}
			}
			else
			{
				row.symbol = symbol;
				row.guides = GetNonTermGuides(symbol, table);
				row.shift = false;
				row.error = true;
				row.ptr = GetNonTermPtr(symbol, table);
				row.stack = true;
				row.end = false;
			}

			table.push_back(row);
		}

		auto& last = table.back();
		if (IsTerm(last.symbol))
		{
			last.ptr = std::nullopt;
		}
		else
		{
			last.stack = false;
		}
		last.end = isAxiom;
	}

	static size_t GetNonTermPtr(std::string const& symbol, Table const& table)
	{
		size_t ptr = 0;
		for (const auto& row : table)
		{
			if (row.symbol == symbol)
			{
				return ptr;
			}
			++ptr;
		}

		throw std::runtime_error("unknown non term");
	}

	Guides GetNonTermGuides(std::string symbol, Table const& table) const
	{
		Guides result{};
		for (const auto& row : table
		     | std::ranges::views::take(GetLeftSideNonTermsNumber())
		     | std::ranges::views::filter([&](const auto& r) { return r.symbol == symbol; }))
		{
			for (const auto& guide : row.guides)
			{
				result.emplace(guide);
			}
		}

		return result;
	}

	size_t GetLeftSideNonTermsNumber() const
	{
		size_t count = 0;
		for (const auto& alternatives : m_rules | std::ranges::views::values)
		{
			count += alternatives.size();
		}

		return count;
	}

	void ParseRules()
	{
		std::string rule;
		while (std::getline(m_input, rule))
		{
			std::istringstream iss(rule);
			std::string nonTerm;
			iss >> nonTerm;

			std::string minus;
			iss >> minus;

			const auto alternative = GetAlternative(iss);

			auto it = std::ranges::find_if(m_rules, [&](const auto& r) {
				return r.first == nonTerm;
			});
			if (it == m_rules.end())
			{
				m_rules.push_back({ nonTerm, { alternative } });
			}
			else
			{
				it->second.push_back(alternative);
			}
		}
	}

	static Alternative GetAlternative(std::istream& str)
	{
		const auto rightSide = GetRuleRightSide(str);
		const auto guides = GetGuides(str);

		return { rightSide, guides };
	}

	static std::vector<std::string> GetRuleRightSide(std::istream& str)
	{
		std::string lexeme;
		std::vector<std::string> result;
		while (str >> lexeme && lexeme != "/")
		{
			result.emplace_back(lexeme);
		}

		return result;
	}

	static Guides GetGuides(std::istream& str)
	{
		Guides result;
		std::string token;
		while (str >> token)
		{
			result.emplace(token);
		}

		return result;
	}

	static bool IsTerm(std::string const& term)
	{
		return term[0] != '<';
	}

private:
	Rules m_rules{};
	std::stringstream m_input;
};

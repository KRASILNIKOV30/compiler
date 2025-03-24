#pragma once
#include "../table/Table.h"
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include "./parseRules/ParseRules.h"

/**
 * input format:
 * <S> - <A> / a c
 * <S> - b <B> / b
 * <A> - a / a
 * <A> - c / c
 * <B> - b / b
 */
class TableBuilder
{
public:
	explicit TableBuilder(std::string const& str)
	{
		std::stringstream iss(str);
		m_rules = ParseRules(iss);
	}

	explicit TableBuilder(std::istream const& strm)
	{
		std::stringstream iss;
		iss << strm.rdbuf();
		m_rules = ParseRules(iss);
	}

	[[nodiscard]] Table BuildTable() const
	{
		Table table;
		FillLeftNonTerms(table);
		FillRightSides(table);

		return table;
	}

private:
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

		throw std::runtime_error("unknown non term " + symbol);
	}

	[[nodiscard]] Guides GetNonTermGuides(std::string symbol, Table const& table) const
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

	[[nodiscard]] size_t GetLeftSideNonTermsNumber() const
	{
		size_t count = 0;
		for (const auto& alternatives : m_rules | std::ranges::views::values)
		{
			count += alternatives.size();
		}

		return count;
	}

private:
	Rules m_rules{};
};

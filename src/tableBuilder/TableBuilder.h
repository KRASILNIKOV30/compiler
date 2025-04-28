#pragma once
#include "../table/Table.h"
#include "./parseRules/ParseRules.h"
#include <cassert>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

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

	[[nodiscard]] Table BuildTable()
	{
		assert(m_rules.front().alternatives.size() == 1);
		Table table;
		table.emplace_back();
		const auto firstRule = m_rules.front().name;

		table[0][firstRule] = { .isOk = true };
		AddEntriesToTable(ExtendGrammarEntries({ 0, 0 }), 0);
		m_rowsQueue.emplace(0);

		while (!m_rowsQueue.empty())
		{
			const auto rowIndex = m_rowsQueue.front();
			m_rowsQueue.pop();
			const auto& rowEntries = m_grammarEntries[rowIndex];

			for (const auto& entry : rowEntries)
			{
				if (IsLast(entry))
				{
					FillFolds(entry, rowIndex);
				}
				else
				{
					const auto nextEntry = GetNext(entry);
					AddEntriesToTable(ExtendGrammarEntries(nextEntry), rowIndex);
				}
			}
		}

		return table;
	}

private:
	void FillFolds(GrammarEntry const& entry, const size_t rowIndex)
	{
		const auto ruleIndex = entry.rule;
		for (const auto& symbol : GetFollow(GetRuleName(entry.rule)))
		{
			m_table[rowIndex][symbol] = {
				.type = ActionType::RULE,
				.value = ruleIndex
			};
		}
	}

	static GrammarEntry GetNext(GrammarEntry const& entry)
	{
		return { .rule = entry.rule, .pos = entry.pos + 1 };
	}

	void AddEntriesToTable(std::vector<GrammarEntry> const& entries, const size_t rowIndex)
	{
		for (auto const& entry : entries)
		{
			const auto symbol = GetSymbol(entry);
			if (m_table[rowIndex].contains(symbol))
			{
				m_grammarEntries[m_table[rowIndex].at(symbol).value].emplace_back(entry);
			}
			else
			{
				m_table[rowIndex][symbol] = { .type = ActionType::SHIFT, .value = m_grammarEntries.size() };
				m_grammarEntries.emplace_back(std::vector{ entry });
			}
		}
	}

	std::vector<GrammarEntry> ExtendGrammarEntries(GrammarEntry const& entry)
	{
		std::vector entries{ entry };
		const auto symbol = GetSymbol(entry);
		for (const auto& [rule, guides] : GetAlternatives(symbol))
		{
			AddEntries(guides, entries);
		}

		return entries;
	}

	void AddEntries(Guides const& guides, std::vector<GrammarEntry>& entries)
	{
		size_t ruleIndex = 0;
		for (auto const& [name, alternatives] : m_rules)
		{
			for (auto const& [rule, _] : alternatives)
			{
				const auto firstSymbol = rule.front();
				if (guides.contains(firstSymbol) && guides.contains(name))
				{
					entries.emplace_back(ruleIndex, 0);
				}
				++ruleIndex;
			}
		}
	}

	[[nodiscard]] Alternatives GetAlternatives(std::string const& nonTerm) const
	{
		return std::ranges::find_if(m_rules, [&nonTerm](const Rule& rule) {
			return rule.name == nonTerm;
		})->alternatives;
	}

	[[nodiscard]] std::string GetRuleName(size_t i) const
	{
		auto rule = m_rules.begin();
		while (rule->alternatives.size() <= i)
		{
			i -= rule->alternatives.size();
			++rule;
		}

		return rule->name;
	}

	[[nodiscard]] Alternative GetAlternative(size_t i) const
	{
		auto rule = m_rules.begin();
		while (rule->alternatives.size() <= i)
		{
			i -= rule->alternatives.size();
			++rule;
		}

		return rule->alternatives.at(i);
	}

	[[nodiscard]] bool IsLast(GrammarEntry const& entry) const
	{
		return GetAlternative(entry.rule).rule.size() == entry.pos + 1;
	}

	[[nodiscard]] std::string GetSymbol(GrammarEntry const& entry) const
	{
		return GetAlternative(entry.rule).rule.at(entry.pos);
	}

	[[nodiscard]] std::unordered_set<std::string> GetFollow(std::string const& nonTerm) const
	{
		std::unordered_set<std::string> followLexemes;
		for (const auto& [name, alternatives] : m_rules)
		{
			for (auto const& [rule, guides] : alternatives)
			{
				for (int i = 0; i < rule.size(); ++i)
				{
					if (rule[i] != nonTerm)
					{
						continue;
					}
					const bool isLast = i == rule.size() - 1;
					std::unordered_set<std::string> follow = isLast
						? name != nonTerm
							? GetFollow(name)
							: std::unordered_set<std::string>{}
						: ExtendSymbol(rule[i + 1]);

					followLexemes.merge(follow);
				}
			}
		}

		return followLexemes;
	}

	[[nodiscard]] std::unordered_set<std::string> ExtendSymbol(std::string const& symbol) const
	{
		std::unordered_set symbols{ symbol };
		if (IsTerm(symbol))
		{
			return symbols;
		}

		for (auto [_, guides] : GetAlternatives(symbol))
		{
			symbols.merge(guides);
		}

		return symbols;
	}

private:
	Rules m_rules{};
	std::vector<std::vector<GrammarEntry>> m_grammarEntries{ 1, std::vector<GrammarEntry>() };
	std::queue<size_t> m_rowsQueue{};
	Table m_table;
};

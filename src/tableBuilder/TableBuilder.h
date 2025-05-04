#pragma once
#include "../parseRules/ParseRules.h"
#include "../table/Table.h"
#include <cassert>
#include <queue>
#include <ranges>
#include <string>
#include <utility>
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
	explicit TableBuilder(Rules rules)
		: m_rules(std::move(rules))
	{
	}

	[[nodiscard]] Table BuildTable()
	{
		assert(m_rules.front().alternatives.size() == 1);
		const auto firstRule = m_rules.front().name;

		m_table[0][firstRule] = { .isOk = true };
		AddEntriesToTable(ExtendGrammarEntries({ 0, 0 }), 0);
		m_rowsQueue.emplace(1);

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
					const auto symbol = GetSymbol(nextEntry);
					if (symbol == END_SYMBOL)
					{
						m_table[rowIndex][END_SYMBOL] = BuildRuleFold(entry.rule);
					}
					else
					{
						AddEntriesToTable(ExtendGrammarEntries(nextEntry), rowIndex);
					}
				}
			}
		}

		return m_table;
	}

private:
	void FillFolds(GrammarEntry const& entry, const size_t rowIndex)
	{
		const auto ruleIndex = entry.rule;
		for (const auto& symbol : GetFollow(GetRuleName(entry.rule)))
		{
			m_table[rowIndex][symbol] = BuildRuleFold(ruleIndex);
		}
	}

	[[nodiscard]] Action BuildRuleFold(const size_t ruleIndex) const
	{
		return {
			.type = ActionType::RULE,
			.value = ruleIndex,
			.ruleName = GetRuleName(ruleIndex),
			.ruleSize = GetRuleSize(ruleIndex),
		};
	}

	static GrammarEntry GetNext(GrammarEntry const& entry)
	{
		return { .rule = entry.rule, .pos = entry.pos + 1 };
	}

	void AddEntriesToTable(std::vector<GrammarEntry> const& entries, const size_t rowIndex)
	{
		std::unordered_map<std::string, GrammarEntriesSet> rowEntriesMap;
		for (auto const& entry : entries)
		{
			const auto symbol = GetSymbol(entry);
			rowEntriesMap[symbol].insert(entry);
		}

		for (const auto& [symbol, entriesSet] : rowEntriesMap)
		{
			if (m_table[rowIndex].contains(symbol))
			{
				m_grammarEntries[m_table[rowIndex].at(symbol).value].insert(entriesSet.begin(), entriesSet.end());
			}
			else
			{
				m_table[rowIndex][symbol] = {
					.type = ActionType::SHIFT,
					.value = GetEntriesSetIndex(entriesSet)
				};
			}
		}
	}

	size_t GetEntriesSetIndex(GrammarEntriesSet const& entrySet)
	{
		for (size_t i = 0; i < m_grammarEntries.size(); ++i)
		{
			if (m_grammarEntries[i] == entrySet)
			{
				return i;
			}
		}

		const size_t newRowIndex = m_grammarEntries.size();
		m_grammarEntries.emplace_back(entrySet);
		m_table.emplace_back();
		m_rowsQueue.emplace(newRowIndex);

		return newRowIndex;
	}

	std::vector<GrammarEntry> ExtendGrammarEntries(GrammarEntry const& entry)
	{
		std::vector entries{ entry };
		const auto symbol = GetSymbol(entry);
		if (IsTerm(symbol))
		{
			return entries;
		}

		for (const auto& [rule, guides] : GetAlternatives(symbol))
		{
			AddEntries(guides, entries, symbol);
		}

		return entries;
	}

	void AddEntries(Guides const& guides, std::vector<GrammarEntry>& entries, std::string const& symbol)
	{
		size_t ruleIndex = 0;
		for (auto const& [name, alternatives] : m_rules)
		{
			for (auto const& [rule, _] : alternatives)
			{
				const auto firstSymbol = rule.front();
				if (guides.contains(firstSymbol) && (guides.contains(name) || name == symbol))
				{
					entries.emplace_back(ruleIndex, 0);
				}
				++ruleIndex;
			}
		}
	}

	[[nodiscard]] Alternatives GetAlternatives(std::string const& nonTerm) const
	{
		const auto it = std::ranges::find_if(m_rules, [&nonTerm](const Rule& rule) {
			return rule.name == nonTerm;
		});

		if (it == m_rules.end())
		{
			throw std::runtime_error("unknown non terminal " + nonTerm);
		}

		return it->alternatives;
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

	[[nodiscard]] size_t GetRuleSize(const size_t i) const
	{
		const auto rule = GetAlternative(i).rule;
		return rule.back() == END_SYMBOL
			? rule.size() - 1
			: rule.size();
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
	std::vector<GrammarEntriesSet> m_grammarEntries{ 1, GrammarEntriesSet{} };
	std::queue<size_t> m_rowsQueue{};
	Table m_table{ 1, std::unordered_map<std::string, Action>() };
};

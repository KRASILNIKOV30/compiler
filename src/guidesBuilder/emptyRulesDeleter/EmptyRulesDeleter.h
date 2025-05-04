#pragma once
#include "../../parseRules/ParseRules.h"
#include "../../table/Table.h"
#include <algorithm>
#include <optional>
#include <ranges>

class EmptyRulesDeleter
{
public:
	explicit EmptyRulesDeleter(raw::Rules rules)
		: m_rules(std::move(rules))
	{
	}

	[[nodiscard]] raw::Rules DeleteEmptyRules()
	{
		if (m_rules.size() < 2)
		{
			return m_rules;
		}

		while (const auto emptyRule = FindEmptyRule())
		{
			const auto& rule = emptyRule.value();
			AddRules(rule);
			DeleteEmptyAlternative(rule);
		}
		return m_rules;
	}

private:
	void DeleteEmptyAlternative(std::string const& ruleName)
	{
		const auto it = std::ranges::find_if(m_rules, [&](const auto& rule) {
			return rule.first == ruleName;
		});

		std::erase_if(it->second, [](const auto& alternative) {
			return alternative.size() == 1 && alternative.front() == EMPTY;
		});
	}

	void AddRules(std::string const& emptyRule)
	{
		for (auto& [name, alternatives] : m_rules)
		{
			for (int i = 0; i < alternatives.size(); i++)
			{
				const auto& alternative = alternatives[i];
				const auto& emptyRulesCount = std::ranges::count(alternative, emptyRule);
				if (emptyRulesCount > 1)
				{
					throw std::runtime_error("rule " + name + " have more than one e-generating rules " += emptyRule);
				}
				if (emptyRulesCount == 1)
				{
					raw::Alternative newAlternative;
					for (const auto& symbol : alternative
					     | std::ranges::views::filter([&](const auto& x) { return x != emptyRule; }))
					{
						newAlternative.emplace_back(symbol);
					}
					if (newAlternative.empty())
					{
						newAlternative.emplace_back(EMPTY);
					}
					alternatives.emplace_back(std::move(newAlternative));
				}
			}
		}
	}

	[[nodiscard]] std::optional<std::string> FindEmptyRule() const
	{
		for (const auto& [name, alternatives] : m_rules | std::views::drop(1))
		{
			for (const auto& alternative : alternatives)
			{
				if (alternative.size() == 1 && alternative.front() == EMPTY)
				{
					return name;
				}
			}
		}

		return std::nullopt;
	}

private:
	raw::Rules m_rules;
};
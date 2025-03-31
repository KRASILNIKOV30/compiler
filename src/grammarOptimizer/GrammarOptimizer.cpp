#include "GrammarOptimizer.h"
#include "../table/Table.h"

#include <ranges>
#include <set>

void GrammarOptimizer::RemoveLeftRecursion()
{
	const std::string SALT = "Rem"; // Суффикс для новых нетерминалов
	std::vector<std::pair<std::string, raw::Alternatives>> newRules;

	for (auto& [left, alternatives] : m_rules)
	{
		std::vector<long> altsToRemove;
		std::vector<raw::Alternative*> nonRecursiveAlts;
		std::vector<raw::Alternative*> recursiveAlts;
		for (size_t i = 0; i < alternatives.size(); i++)
		{
			auto& alt = alternatives[i];
			if (!alt.empty() && alt[0] == left)
			{
				altsToRemove.push_back(static_cast<long>(i));
				recursiveAlts.push_back(&alt);
			}
			else
			{
				nonRecursiveAlts.push_back(&alt);
			}
		}

		if (recursiveAlts.empty())
		{
			continue;
		}

		std::string newLeft = left;
		newLeft.insert(newLeft.end() - 1, SALT.begin(), SALT.end());

		// Модифицируем нерекурсивные правила: A -> β становится A -> βA'
		for (const auto alt : nonRecursiveAlts)
		{
			if (alt->empty() || alt->front() != EMPTY) // Не добавляем A' к пустому правилу
			{
				alt->push_back(newLeft);
			}
		}

		// Создаем правила для нового нетерминала
		raw::Alternatives newLeftAlts;
		for (auto& alt : recursiveAlts)
		{
			// Удаляем левый нетерминал из начала правила
			raw::Alternative newAlt(alt->begin() + 1, alt->end());

			// Добавляем новое правило: A' -> αA'
			newAlt.push_back(newLeft);
			newLeftAlts.push_back(std::move(newAlt));

			// Также добавляем правило: A' -> α
			// newLeftAlts.emplace_back(alt.begin() + 1, alt.end());
		}

		// Добавляем пустое правило: A' -> ε
		newLeftAlts.push_back({ EMPTY });

		// Сохраняем новые правила
		newRules.emplace_back(newLeft, std::move(newLeftAlts));

		// Удаляем рекурсивные правила
		for (const auto indexToRemove : altsToRemove | std::ranges::views::reverse)
		{
			alternatives.erase(alternatives.begin() + indexToRemove);
		}
	}

	// Добавляем все новые правила в исходный набор
	m_rules.insert(m_rules.end(), newRules.begin(), newRules.end());
}

void GrammarOptimizer::LeftFactor()
{
	const std::string SALT = "Fact";
	for (auto& [left, alternatives] : m_rules)
	{
		if (alternatives.size() < 2)
		{
			continue;
		}

		std::unordered_map<std::string, std::vector<raw::Alternative>> commonPrefixes;

		// Группируем правила по первому символу
		for (const auto& alt : alternatives)
		{
			if (!alt.empty())
			{
				const auto prefix = alt.front();
				commonPrefixes[prefix].push_back(alt);
			}
		}

		raw::Alternatives newAlternatives;
		std::set<raw::Alternative> altsToRemove;
		for (auto& alts : commonPrefixes | std::views::values)
		{
			if (alts.size() == 1)
			{
				newAlternatives.push_back(alts.front());
				continue;
			}

			altsToRemove.insert(alts.begin(), alts.end());
			// Находим наибольший общий префикс
			size_t commonLen = 1;
			bool hasCommon = true;

			while (hasCommon)
			{
				if (alts[0].size() <= commonLen)
				{
					break;
				}

				const auto& next = alts[0][commonLen];
				for (const auto& alt : alts)
				{
					if (alt.size() <= commonLen || alt[commonLen] != next)
					{
						hasCommon = false;
						break;
					}
				}
				if (hasCommon)
				{
					commonLen++;
				}
			}

			if (commonLen == 1)
			{
				std::string newLeft = left;
				newLeft.insert(newLeft.end() - 1, SALT.begin(), SALT.end());

				raw::Alternative commonPart{ alts[0].begin(), alts[0].begin() + commonLen };
				commonPart.push_back(newLeft);
				newAlternatives.push_back(commonPart);

				raw::Alternatives newAlts;
				for (const auto& alt : alts)
				{
					if (alt.size() > commonLen)
					{
						newAlts.emplace_back(alt.begin() + commonLen, alt.end());
					}
					else
					{
						newAlts.push_back({ EMPTY });
					}
				}

				alternatives.push_back({ commonPart[0], newLeft });
				erase_if(alternatives, [&](const auto& alt) {
					return altsToRemove.contains(alt);
				});
				m_rules.emplace_back(newLeft, newAlts);
			}
		}

		if (!newAlternatives.empty())
		{
			alternatives = std::move(newAlternatives);
		}
	}
}

raw::Rules const& GrammarOptimizer::GetRules() const
{
	return m_rules;
}
#include "GrammarOptimizer.h"
#include "../table/Table.h"

#include <ranges>

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

raw::Rules const& GrammarOptimizer::GetRules() const
{
	return m_rules;
}
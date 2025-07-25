#pragma once
#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::string END_SYMBOL = "#";
const std::string EMPTY = "e";
constexpr char NON_TERM_FIRST_CHAR = '<';
constexpr int NON_TERM_MIN_SIZE = 3;

using Guides = std::unordered_set<std::string>;

inline bool operator==(Guides const& a, Guides const& b)
{
	return a.size() == b.size() && std::ranges::all_of(a, [&](auto const& el) {
		return b.contains(el);
	});
}

struct GrammarEntry
{
	size_t rule;
	size_t pos;
	bool operator==(GrammarEntry const& other) const = default;

	struct Hasher
	{
		size_t operator()(GrammarEntry const& entry) const
		{
			return std::hash<size_t>()(entry.rule) ^ std::hash<size_t>()(entry.pos);
		}
	};
};

using GrammarEntriesSet = std::unordered_set<GrammarEntry, GrammarEntry::Hasher>;

enum class ActionType
{
	SHIFT,
	RULE,
};

struct Action
{
	ActionType type;
	size_t value;
	std::string ruleName;
	size_t ruleSize = 0;
	bool isOk = false;
	bool operator==(const Action&) const = default;
};

using TableRow = std::unordered_map<std::string, Action>;

inline bool operator==(TableRow const& a, TableRow const& b)
{
	return a.size() == b.size() && std::ranges::all_of(a, [&](auto const& el) {
		const auto& [key, value] = el;
		return b.contains(key) && b.at(key) == value;
	});
}

using Table = std::vector<TableRow>;

inline bool operator==(Table const& a, Table const& b)
{
	return std::ranges::equal(a, b);
}

inline bool IsTerm(std::string const& term)
{
	return term.size() < NON_TERM_MIN_SIZE || term[0] != NON_TERM_FIRST_CHAR;
}
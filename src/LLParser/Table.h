#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include "../lexer/token/Token.h"
#include <algorithm>

using Guides = std::unordered_set<TokenType>;

inline bool operator==(Guides const& a, Guides const& b)
{
	if (a.size() != b.size())
	{
		return false;
	}

	return std::ranges::all_of(a, [&](auto const& el) {
		return b.contains(el);
	});
}

struct TableRow
{
	size_t index;
	std::string symbol;
	Guides guides;
	bool shift;
	Error error;
	size_t ptr;
	bool stack;
	bool end;
	bool operator==(const TableRow&) const = default;
};

using Table = std::vector<TableRow>;

inline bool operator==(Table const& a, Table const& b)
{
	return std::ranges::equal(a, b);
}
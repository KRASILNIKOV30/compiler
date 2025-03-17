#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <optional>

const std::string EMPTY = "e";

using Guides = std::unordered_set<std::string>;

inline bool operator==(Guides const& a, Guides const& b)
{
	if (a.size() != b.size())
	{
		return false;
	}

	const auto result = std::ranges::all_of(a, [&](auto const& el) {
		return b.contains(el);
	});

	return result;
}

struct TableRow
{
	std::string symbol;
	Guides guides;
	bool shift;
	bool error;
	std::optional<size_t> ptr;
	bool stack;
	bool end;
};

inline bool operator==(TableRow const& a, TableRow const& b)
{
	return a.symbol == b.symbol &&
		a.guides == b.guides &&
		a.shift == b.shift &&
		a.error == b.error &&
		a.ptr == b.ptr &&
		a.stack == b.stack &&
		a.end == b.end;
}

using Table = std::vector<TableRow>;

inline bool operator==(Table const& a, Table const& b)
{
	return std::ranges::equal(a, b);
}
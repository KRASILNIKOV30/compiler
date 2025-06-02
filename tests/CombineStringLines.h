#pragma once
#include "../src/utils/FoldLeft.h"

inline std::string CombineStringLines(std::vector<std::string> const& lines)
{
	return FoldLeft(lines, [](auto&& acc, auto const& line) { return acc + line + '\n'; });
}
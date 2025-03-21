#pragma once
#include <ostream>
#include <unordered_set>
#include <vector>

template <class T>
std::ostream& PrintContainer(std::ostream& str, T const& c)
{
	for (const auto& element : c)
	{
		str << " " << element;
	}

	return str;
}

inline std::ostream& operator<<(std::ostream& str, std::vector<std::string> const& c)
{
	return PrintContainer(str, c);
}

inline std::ostream& operator<<(std::ostream& str, std::unordered_set<std::string> const& c)
{
	return PrintContainer(str, c);
}
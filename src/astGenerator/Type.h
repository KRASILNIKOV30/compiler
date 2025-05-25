#pragma once
#include <vector>

enum class PrimitiveType
{
	INT,
	FLOAT,
	STRING,
};

using Type = std::vector<PrimitiveType>;
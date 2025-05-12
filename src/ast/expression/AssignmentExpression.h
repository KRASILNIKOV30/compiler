#pragma once
#include "Expression.h"
#include <optional>

struct AssignmentExpression : public Expression
{
	std::string left;
	std::optional<std::string> type = std::nullopt;
	Expression right;
};
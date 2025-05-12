#pragma once
#include "Expression.h"
#include "../statement/BlockStatement.h"
#include <vector>
#include <variant>

struct ArrowFunctionExpression : public Expression
{
	std::vector<std::string> params;
	std::variant<Expression, BlockStatement> body;
};


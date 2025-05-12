#pragma once
#include "Expression.h"
#include <vector>

struct CallExpression : public Expression
{
	std::string callee;
	std::vector<Expression> arguments;
};
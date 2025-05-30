#pragma once
#include "Expression.h"
#include <vector>

struct CallExpression : Expression
{
	std::string callee;
	std::vector<Expression> arguments;
};
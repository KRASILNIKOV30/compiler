#pragma once
#include "Expression.h"
#include <vector>

struct InitializerListExpression : public Expression
{
	std::vector<Expression> elements;
};
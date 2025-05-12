#pragma once
#include "Expression.h"

struct ArrayExpression : public Expression
{
	std::string id;
	Expression index;
};
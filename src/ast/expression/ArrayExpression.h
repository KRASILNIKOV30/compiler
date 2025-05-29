#pragma once
#include "Expression.h"

struct ArrayExpression : Expression
{
	std::string id;
	Expression index;
};
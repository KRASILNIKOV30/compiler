#pragma once
#include "Expression.h"
#include "Operators.h"

struct UnaryExpression : public Expression
{
	UnaryOperators oper;
	Expression argument;
};
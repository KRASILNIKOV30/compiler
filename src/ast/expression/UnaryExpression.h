#pragma once
#include "Expression.h"
#include "Operators.h"

struct UnaryExpression : public Expression
{
	BinaryOperators oper;
	Expression argument;
};
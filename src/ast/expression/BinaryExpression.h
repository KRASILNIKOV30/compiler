#pragma once
#include "Expression.h"
#include "Operators.h"

struct BinaryExpression : public Expression
{
	Expression left, right;
	BinaryOperators oper;
};
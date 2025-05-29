#pragma once
#include "Expression.h"
#include "Operators.h"

struct BinaryExpression : Expression
{
	void Generate(CodeGenerator& generator) const override
	{

	}

	Expression left, right;
	BinaryOperators oper;
};
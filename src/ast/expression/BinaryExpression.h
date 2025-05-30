#pragma once
#include "Expression.h"
#include "Operators.h"

struct BinaryExpression : Expression
{
	void Generate(CodeGenerator& generator) const override
	{
		left.Generate(generator);
		right.Generate(generator);
		generator.AddInstruction(BinaryOperatorsToString.at(oper));
	}

	Expression left, right;
	BinaryOperators oper;
};
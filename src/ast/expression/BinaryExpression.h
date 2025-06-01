#pragma once
#include "Expression.h"
#include "Operators.h"

struct BinaryExpression : Expression
{
	void Generate(CodeGenerator& generator) const override
	{
		left.Generate(generator);
		right.Generate(generator);

		for (auto const& byteCodeOper: BinaryOperatorsToString.at(oper))
		{
			generator.AddInstruction(byteCodeOper);
		}
	}

	Expression left, right;
	BinaryOperators oper;
};
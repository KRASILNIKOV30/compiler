#pragma once
#include "Expression.h"
#include "Operators.h"
#include <set>

struct UnaryExpression : Expression
{
	void Generate(CodeGenerator& generator) const override
	{
		argument.Generate(generator);
		if (std::set{UnaryOperators::MINUS, UnaryOperators::NOT}.contains(oper))
		{
			generator.AddInstruction("not");
		}
	}

	UnaryOperators oper;
	Expression argument;
};
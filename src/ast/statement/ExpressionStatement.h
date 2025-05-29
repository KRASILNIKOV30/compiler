#pragma once
#include "../expression/Expression.h"
#include "Statement.h"

struct ExpressionStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		expression.Generate(generator);
	}

	Expression expression;
};
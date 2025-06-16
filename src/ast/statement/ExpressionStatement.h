#pragma once
#include "../expression/Expression.h"
#include "Statement.h"

class ExpressionStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		expression->Generate(generator);
	}

	ExpressionPtr expression;
};
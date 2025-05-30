#pragma once
#include "../expression/Expression.h"
#include "Statement.h"

struct ReturnStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		argument.Generate(generator);
	}

	Expression argument;
};
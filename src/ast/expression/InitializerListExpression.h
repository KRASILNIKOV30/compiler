#pragma once
#include "Expression.h"
#include <vector>

struct InitializerListExpression : Expression
{
	void Generate(CodeGenerator& generator) const override
	{
		for (auto& element : elements)
		{
			element.Generate(generator);
		}
	}

	std::vector<Expression> elements;
};
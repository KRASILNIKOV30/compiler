#pragma once
#include "../expression/Expression.h"
#include "Statement.h"
#include <assert.h>
#include <optional>

struct AssignmentStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
	}

	std::string left;
	Type type;
	Expression right;
};
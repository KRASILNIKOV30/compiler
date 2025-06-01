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
	std::optional<PrimitiveType> type = std::nullopt;
	Expression right;
};
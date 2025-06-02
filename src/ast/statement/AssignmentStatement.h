#pragma once
#include "../expression/Expression.h"
#include "Statement.h"
#include <assert.h>
#include <optional>

class AssignmentStatement : public Statement
{
public:
	AssignmentStatement(std::string left, ExpressionPtr&& right)
		: m_left(left)
		, m_right(std::move(right))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
	}

private:
	std::string m_left;
	ExpressionPtr m_right;
};
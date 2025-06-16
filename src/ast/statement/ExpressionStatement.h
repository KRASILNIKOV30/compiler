#pragma once
#include "../expression/Expression.h"
#include "Statement.h"

class ExpressionStatement : public Statement
{
public:
	ExpressionStatement(ExpressionPtr&& expression)
		: m_expression(std::move(expression))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		m_expression->Generate(generator);
	}

private:
	ExpressionPtr m_expression;
};
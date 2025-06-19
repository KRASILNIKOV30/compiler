#pragma once
#include "../expression/Expression.h"
#include "Statement.h"

class ReturnStatement : public Statement
{
public:
	ReturnStatement(ExpressionPtr&& expr)
		: m_argument(std::move(expr))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		m_argument->Generate(generator);
		generator.AddInstruction("return");
	}

private:
	ExpressionPtr m_argument = nullptr;
};
#pragma once
#include "../expression/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"

class WhileStatement : public Statement
{
public:
	WhileStatement(ExpressionPtr&& condition)
		: m_condition(std::move(condition))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		auto& labelGenerator = generator.GetLabelGenerator();
		labelGenerator.NewWhileLabel();

		generator.AddLabel(labelGenerator.GetWhileLabel());
		m_condition->Generate(generator);
		generator.AddInstruction("jmp_false " + labelGenerator.GetEndWhileLabel());

		m_body->Generate(generator);
		generator.AddInstruction("jmp " + labelGenerator.GetWhileLabel());

		generator.AddLabel(labelGenerator.GetEndWhileLabel());
	}

	BlockStatement* GetBlock()
	{
		return m_body.get();
	}

private:
	ExpressionPtr m_condition;
	BlockStatementPtr m_body = std::make_unique<BlockStatement>();
};
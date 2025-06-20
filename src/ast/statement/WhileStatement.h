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

		const auto whileLabel = labelGenerator.GetWhileLabel();
		const auto endWhileLabel = labelGenerator.GetEndWhileLabel();
		generator.AddLabel(whileLabel);
		m_condition->Generate(generator);
		generator.AddInstruction("jmp_false " + endWhileLabel);

		m_body->Generate(generator);
		generator.AddInstruction("jmp " + whileLabel);

		generator.AddLabel(endWhileLabel);
	}

	BlockStatement* GetBlock()
	{
		return m_body.get();
	}

private:
	ExpressionPtr m_condition;
	BlockStatementPtr m_body = std::make_unique<BlockStatement>();
};
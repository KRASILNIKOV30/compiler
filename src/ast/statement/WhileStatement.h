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
		static int whileId = 0;
		++whileId;

		generator.AddLabel("while" + std::to_string(whileId));
		m_condition->Generate(generator);
		generator.AddInstruction("jmp_false endwhile" + std::to_string(whileId));

		m_body->Generate(generator);
		generator.AddInstruction("jmp while" + std::to_string(whileId));

		generator.AddLabel("endwhile" + std::to_string(whileId));
	}

	BlockStatement* GetBlock()
	{
		return m_body.get();
	}

private:
	ExpressionPtr m_condition;
	BlockStatementPtr m_body = std::make_unique<BlockStatement>();
};
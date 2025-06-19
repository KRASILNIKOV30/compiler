#pragma once
#include "../expression/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include <optional>

class IfStatement : public Statement
{
public:
	IfStatement(ExpressionPtr&& condition)
		: m_condition(std::move(condition))
	{
	}

	BlockStatement* GetThenBlock()
	{
		return m_body.get();
	}

	BlockStatement* CreateElseBlock()
	{
		BlockStatementPtr elseBlock = std::make_unique<BlockStatement>();
		const auto ptr = elseBlock.get();
		m_alternate.emplace(std::move(elseBlock));
		return ptr;
	}

	IfStatement* CreateElseIfBlock(ExpressionPtr&& condition)
	{
		auto elseIfBlock = std::make_unique<IfStatement>(std::move(condition));
		const auto ptr = elseIfBlock.get();
		m_alternate.emplace(std::move(elseIfBlock));
		return ptr;
	}

	void Generate(CodeGenerator& generator) const override
	{
		m_condition->Generate(generator);

		auto& labelGenerator = generator.GetLabelGenerator();
		labelGenerator.NewIfLabel();

		std::string jmpLabel = m_alternate.has_value() ? labelGenerator.GetElseLabel() : labelGenerator.GetEndIfLabel();
		generator.AddInstruction("jmp_false " + jmpLabel);

		m_body->Generate(generator);
		generator.AddInstruction("jmp " + labelGenerator.GetEndIfLabel());

		if (m_alternate.has_value())
		{
			generator.AddLabel(labelGenerator.GetElseLabel());
			std::visit([&](const auto& alternative) { Generate(generator, alternative); }, m_alternate.value());
			generator.AddInstruction("jmp " + labelGenerator.GetEndIfLabel());
		}

		generator.AddLabel(labelGenerator.GetEndIfLabel());
	}

private:
	using IfStatementPtr = std::unique_ptr<IfStatement>;

	static void Generate(CodeGenerator& generator, IfStatementPtr const& ifStatement)
	{
		ifStatement->Generate(generator);
	}

	static void Generate(CodeGenerator& generator, BlockStatementPtr const& block)
	{
		block->Generate(generator);
	}

	ExpressionPtr m_condition;
	BlockStatementPtr m_body = std::make_unique<BlockStatement>();
	std::optional<std::variant<IfStatementPtr, BlockStatementPtr>> m_alternate = std::nullopt;
};
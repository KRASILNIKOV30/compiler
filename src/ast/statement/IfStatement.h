#pragma once
#include "../expression/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include <optional>

class IfStatement : public Statement
{
	IfStatement(ExpressionPtr&& condition, BlockStatement block, std::optional<std::variant<BlockStatement, IfStatement>> alternative = std::nullopt)
		: m_condition(std::move(condition))
		, m_body(std::move(block))
		, m_alternate(std::move(alternative))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		static int ifId = 0;
		++ifId;

		m_condition->Generate(generator);

		std::string jmpLabel = m_alternate.has_value() ? "else" : "endif";
		generator.AddInstruction("jmp_false" + jmpLabel + std::to_string(ifId));

		m_body.Generate(generator);
		generator.AddInstruction("jmp endif" + ifId);

		if (m_alternate.has_value())
		{
			generator.AddLabel("else" + ifId);
			std::visit([&](const auto& alternative) { Generate(generator, alternative); }, alternate.value());
			generator.AddInstruction("jmp endif" + ifId);
		}

		generator.AddLabel("endif" + ifId);
	}

private:
	void Generate(CodeGenerator& generator, const IfStatement& ifStatement)
	{
		ifStatement.Generate(generator);
	}

	void Generate(CodeGenerator& generator, const BlockStatement& block)
	{
		block.Generate(generator);
	}

	ExpressionPtr m_condition;
	BlockStatement m_body;
	std::optional<std::variant<BlockStatement, IfStatement>> m_alternate = std::nullopt;
};
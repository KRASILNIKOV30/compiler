#pragma once
#include "../expression/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include <optional>

struct IfStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		++ifId;

		condition.Generate(generator);

		std::string jmpLabel = alternate.has_value() ? "else" : "endif";
		generator.AddInstruction("jmpfalse" + jmpLabel + std::to_string(ifId));

		body.Generate(generator);
		generator.AddInstruction("jmp endif" + ifId);

		if (alternate.has_value())
		{
			generator.AddLabel("else" + ifId);
			std::visit([&](const auto& alternative) { Generate(generator, alternative); }, alternate.value());
			generator.AddInstruction("jmp endif" + ifId);
		}

		generator.AddLabel("endif" + ifId);
	}

	static int ifId = 0;
	Expression condition;
	BlockStatement body;
	std::optional<std::variant<BlockStatement, IfStatement>> alternate = std::nullopt;

private:
	void Generate(CodeGenerator& generator, const IfStatement& ifStatement)
	{
		ifStatement.Generate(generator);
	}

	void Generate(CodeGenerator& generator, const BlockStatement& block)
	{
		block.Generate(generator);
	}
};
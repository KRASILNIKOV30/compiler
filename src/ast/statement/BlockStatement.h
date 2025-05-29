#pragma once
#include "../declaration/Declaration.h"
#include "Statement.h"
#include <variant>
#include <vector>

struct BlockStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		for (const auto& bodyPart : body)
		{
			std::visit([&](const auto& bodyNode) { Generate(generator, bodyNode); }, bodyPart);
		}
	}

	std::vector<std::variant<Declaration, Statement>> body;

private:
	static void Generate(CodeGenerator& generator, const Declaration& declaration)
	{
		declaration.Generate(generator);
	}

	static void Generate(CodeGenerator& generator, const Statement& statement)
	{
		statement.Generate(generator);
	}
};
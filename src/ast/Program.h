#pragma once
#include "Entity.h"
#include "declaration/Declaration.h"
#include "statement/Statement.h"
#include <variant>
#include <vector>

struct Program : Entity
{
	std::vector<std::variant<Declaration, Statement>> body;

	void Generate(CodeGenerator& generator) const override
	{
		for (const auto& bodyPart : body)
		{
			std::visit([&](const auto& bodyNode) { Generate(generator, bodyNode); }, bodyPart);
		}
	}

private:
	void Generate(CodeGenerator& generator, const Declaration& declaration)
	{
		declaration.Generate(generator);
	}

	void Generate(CodeGenerator& generator, const Statement& statement)
	{
		statement.Generate(generator);
	}
};
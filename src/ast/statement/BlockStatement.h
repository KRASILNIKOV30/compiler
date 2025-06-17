#pragma once
#include "../declaration/Declaration.h"
#include "Statement.h"
#include <variant>
#include <vector>

using ProgramNode = std::variant<DeclarationPtr, StatementPtr>;

class BlockStatement : public Statement
{
public:
	void Generate(CodeGenerator& generator) const override
	{
		for (const auto& bodyPart : m_body)
		{
			std::visit([&](const auto& bodyNode) { Generate(generator, bodyNode); }, bodyPart);
		}
	}

	void Add(ProgramNode&& node)
	{
		m_body.emplace_back(std::move(node));
	}

private:
	static void Generate(CodeGenerator& generator, DeclarationPtr const& declaration)
	{
		declaration->Generate(generator);
	}

	static void Generate(CodeGenerator& generator, StatementPtr const& statement)
	{
		statement->Generate(generator);
	}

private:
	std::vector<ProgramNode> m_body{};
};

using BlockStatementPtr = std::unique_ptr<BlockStatement>;
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

	void Add(DeclarationPtr&& decl)
	{
		m_body.emplace_back(std::move(decl));
	}

	void Add(StatementPtr&& statement)
	{
		m_body.emplace_back(std::move(statement));
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
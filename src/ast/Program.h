#pragma once
#include "declaration/Declaration.h"
#include "Entity.h"
#include "statement/Statement.h"
#include <variant>
#include <vector>

using ProgramNode = std::variant<DeclarationPtr, StatementPtr>;

class Program : public Entity
{
public:
	explicit Program(std::vector<ProgramNode>&& nodes = {})
		: m_body(std::move(nodes))
	{
	}

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
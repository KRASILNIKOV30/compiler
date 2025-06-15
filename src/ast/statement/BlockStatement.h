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
		if (m_topBlock)
		{
			m_topBlock->Add(std::move(node));
			return;
		}
		m_body.emplace_back(std::move(node));
	}

	void OpenBlock()
	{
		if (m_topBlock)
		{
			m_topBlock->OpenBlock();
			return;
		}
		auto block = std::make_unique<BlockStatement>();
		m_topBlock = block.get();
		m_body.emplace_back(std::move(block));
	}

	void CloseBlock()
	{
		if (m_topBlock)
		{
			m_topBlock->CloseBlock();
			return;
		}
		m_topBlock = nullptr;
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
	BlockStatement* m_topBlock = nullptr;
	std::vector<ProgramNode> m_body{};
};
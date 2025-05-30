#pragma once
#include "../expression/Expression.h"
#include "Declaration.h"

class VariableDeclaration : public Declaration
{
public:
	explicit VariableDeclaration(ExpressionPtr&& init)
		: m_init(std::move(init))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		const auto pos = generator.GetVariablePosOrAdd(id);
		generator.AddInstruction("const " + std::to_string(pos));
		m_init->Generate(generator);
		generator.AddInstruction("setlocal " + std::to_string(pos));
	}

	ExpressionPtr m_init;
};
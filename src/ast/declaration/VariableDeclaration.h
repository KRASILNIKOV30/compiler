#pragma once
#include "../expression/Expression.h"
#include "Declaration.h"

class VariableDeclaration : public Declaration
{
public:
	explicit VariableDeclaration(std::string id, Type type, ExpressionPtr&& init)
		: Declaration(std::move(id), std::move(type))
		, m_init(std::move(init))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		const auto pos = generator.GetVariablePosOrAdd(GetId());
		m_init->Generate(generator);
		generator.AddInstruction("set_local " + std::to_string(pos));
	}

	ExpressionPtr m_init;
};
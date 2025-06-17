#pragma once
#include "../expression/Expression.h"
#include "Declaration.h"

class VariableDeclaration : public Declaration
{
public:
	explicit VariableDeclaration(std::string id, Type type, std::optional<ExpressionPtr>&& init)
		: Declaration(std::move(id), std::move(type))
		, m_init(std::move(init))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		const auto pos = generator.GetVariablePosOrAdd(GetId());
		if (m_init.has_value())
		{
			m_init.value()->Generate(generator);
		}
		generator.AddInstruction("set_local " + std::to_string(pos));
	}

	std::optional<ExpressionPtr> m_init;
};
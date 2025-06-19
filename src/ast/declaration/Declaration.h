#pragma once
#include "../expression/Expression.h"

class Declaration : public Entity
{
public:
	explicit Declaration(std::string id, Type type, std::optional<ExpressionPtr>&& init)
		: m_id(std::move(id))
		, m_type(std::move(type))
		, m_init(std::move(init))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		const auto pos = generator.GetVariablePosOrAdd(m_id);
		if (m_init.has_value())
		{
			m_init.value()->Generate(generator);
		}
		generator.AddInstruction("set_local " + std::to_string(pos));
	}

private:
	std::string m_id;
	Type m_type;
	std::optional<ExpressionPtr> m_init;
};

using DeclarationPtr = std::unique_ptr<Declaration>;
#pragma once
#include <utility>

#include "Expression.h"

class Term : public Expression
{
public:
	explicit Term(std::string value, Type const& type, bool isReference = false)
		: Expression(type)
		, m_value(std::move(value))
		, m_isReference(isReference)
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		if (m_isReference)
		{
			const auto varContext = generator.GetVariableContextPos(m_value);
			if (varContext.isVariableFromParent)
			{
				generator.AddInstruction("get_upvalue " + std::to_string(varContext.pos));
			}
			else
			{
				generator.AddInstruction("get_local " + std::to_string(varContext.pos));
			}
			return;
		}
		if (m_type == PrimitiveType::BOOL)
		{
			generator.AddInstruction(m_value);
			return;
		}
		const auto pos = generator.GetConstantPosOrAdd(m_type, m_value);
		generator.AddInstruction("const " + std::to_string(pos));
	}

	[[nodiscard]] bool HasValue() const override
	{
		return !m_isReference;
	}

	[[nodiscard]] std::string GetValue() const override
	{
		return m_value;
	}

private:
	std::string m_value;
	bool m_isReference = false;
};

using TermPtr = std::unique_ptr<Term>;
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
		size_t pos;
		if (m_isReference)
		{
			try
			{
				pos = generator.GetVariablePos(m_value);
				generator.AddInstruction("get_local" + std::to_string(pos));
			}
			catch (std::invalid_argument const& e)
			{
				throw std::runtime_error(std::format("Failed to generate Term. {}", e.what()));
			}
		}
		else
		{
			pos = generator.GetConstantPosOrAdd(m_type, m_value);
			generator.AddInstruction("const " + std::to_string(pos));
		}
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
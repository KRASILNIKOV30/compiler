#pragma once
#include "Expression.h"

class MemberExpression : public Expression
{
public:
	explicit MemberExpression(Type type, std::string id, std::optional<ExpressionPtr> index, bool isRValue = true)
		: Expression(std::move(type))
		, m_id(std::move(id))
		, m_index(std::move(index))
		, m_isRValue(isRValue)
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		generator.AddInstruction("get_local " + std::to_string(generator.GetVariablePos(m_id)));
		if (m_index.has_value())
		{
			m_index.value()->Generate(generator);
			if (m_isRValue)
			{
				generator.AddInstruction("get_el");
			}
		}
	}

	[[nodiscard]] std::string GetValue() const override
	{
		return m_id;
	}

	void MakeLValue()
	{
		m_isRValue = false;
	}

	[[nodiscard]] std::string GetId() const { return m_id; }
	[[nodiscard]] bool IsPartOfArray() const { return m_index.has_value(); }

private:
	std::string m_id;
	std::optional<ExpressionPtr> m_index;
	bool m_isRValue = true;
};

using MemberExpressionPtr = std::unique_ptr<MemberExpression>;
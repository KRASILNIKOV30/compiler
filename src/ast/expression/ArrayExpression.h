#pragma once
#include "Expression.h"

class ArrayExpression : public Expression
{
public:
	explicit ArrayExpression(Type type, std::vector<ExpressionPtr> elements)
		: Expression(std::move(type))
		, m_elements(std::move(elements))
	{
	}

	void Generate(CodeGenerator &generator) const override
	{
		for (const auto& arrayElement: m_elements)
		{
			arrayElement->Generate(generator);
		}
		generator.AddInstruction("create_arr " + std::to_string(m_elements.size()));
	}

private:
	std::vector<ExpressionPtr> m_elements;
};
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
		generator.AddInstruction("create_arr " + m_elements.size());
	}

private:
	std::vector<ExpressionPtr> m_elements;
};
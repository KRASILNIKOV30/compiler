#pragma once
#include "Expression.h"
#include <vector>

class InitializerListExpression : public Expression
{
public:
	explicit InitializerListExpression(Type type, std::vector<ExpressionPtr> elements)
		: Expression(std::move(type))
		, m_elements(std::move(elements))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		for (auto& element : m_elements)
		{
			element->Generate(generator);
		}
		generator.AddInstruction("create_arr " + std::to_string(m_elements.size()));
	}

private:
	std::vector<ExpressionPtr> m_elements;
};
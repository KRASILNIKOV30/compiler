#pragma once
#include "Expression.h"
#include "Operators.h"
#include <utility>

class BinaryExpression : public Expression
{
public:
	BinaryExpression(ExpressionPtr&& left, ExpressionPtr&& right, BinaryOperators oper, Type type)
		: Expression(std::move(type))
		, m_left(std::move(left))
		, m_right(std::move(right))
		, m_oper(oper)
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		m_left->Generate(generator);
		m_right->Generate(generator);

		for (auto const& byteCodeOper : BinaryOperatorsToString.at(m_oper))
		{
			generator.AddInstruction(byteCodeOper);
		}
	}

private:
	ExpressionPtr m_left, m_right;
	BinaryOperators m_oper;
};
#pragma once
#include "Expression.h"

class MemberExpression : public Expression
{
public:
	explicit MemberExpression(Type type, std::string id, ExpressionPtr index)
		: Expression(std::move(type))
		, m_id(std::move(id))
		, m_index(std::move(index))
	{
	}

	void Generate(CodeGenerator &generator) const override
	{
		generator.AddInstruction("get_local " + generator.GetVariablePos(m_id));
		m_index->Generate(generator);
	}

private:
	std::string m_id;
	ExpressionPtr m_index;
};
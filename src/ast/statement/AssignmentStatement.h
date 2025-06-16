#pragma once
#include "../expression/Expression.h"
#include "../expression/MemberExpression.h"
#include "Statement.h"
#include <cassert>
#include <optional>

class AssignmentStatement : public Statement
{
public:
	AssignmentStatement(MemberExpression left, ExpressionPtr&& right)
		: m_left(std::move(left))
		, m_right(std::move(right))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{

		if (m_left.IsPartOfArray())
		{
			m_left.Generate(generator);
			m_right->Generate(generator);
			generator.AddInstruction("set_el");
		}
		else
		{
			m_right->Generate(generator);
			auto pos = generator.GetVariablePos(m_left.GetId());
			generator.AddInstruction("set_local " + std::to_string(pos));
		}
	}

private:
	MemberExpression m_left;
	ExpressionPtr m_right;
};
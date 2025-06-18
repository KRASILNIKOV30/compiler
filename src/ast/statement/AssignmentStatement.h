#pragma once
#include "../expression/Expression.h"
#include "../expression/MemberExpression.h"
#include "Statement.h"
#include <cassert>
#include <optional>

class AssignmentStatement : public Statement
{
public:
	AssignmentStatement(MemberExpressionPtr&& left, ExpressionPtr&& right)
		: m_left(std::move(left))
		, m_right(std::move(right))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		m_right->Generate(generator);

		if (m_left->IsPartOfArray())
		{
			m_left->Generate(generator);
			generator.AddInstruction("set_el");
		}
		else
		{
			const auto varContext = generator.GetVariableContextPos(m_left->GetId());
			if (varContext.isVariableFromParent)
			{
				generator.AddInstruction("set_upvalue " + std::to_string(varContext.pos));
			}
			else
			{
				generator.AddInstruction("set_local " + std::to_string(varContext.pos));
			}
		}
	}

private:
	MemberExpressionPtr m_left;
	ExpressionPtr m_right;
};
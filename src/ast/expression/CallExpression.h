#pragma once
#include "Expression.h"
#include <unordered_set>
#include <vector>

class CallExpression : public Expression
{
public:
	explicit CallExpression(std::string callee, Type type, std::vector<ExpressionPtr>&& arguments, bool isNativeCallee = false)
		: Expression(std::move(type))
		, m_callee(std::move(callee))
		, m_arguments(std::move(arguments))
		, m_isNativeCallee(isNativeCallee)
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		for (auto const& argument : m_arguments)
		{
			argument->Generate(generator);
		}

		if (m_isNativeCallee)
		{
			generator.AddInstruction("get_global " + std::to_string(generator.GetConstantPosOrAdd(PrimitiveType::STRING, m_callee)));
		}
		else
		{
			const auto calleeContext = generator.GetVariableContextPos(m_callee);
			if (calleeContext.isVariableFromParent)
			{
				generator.AddInstruction("get_upvalue " + std::to_string(calleeContext.pos));
			}
			else
			{
				generator.AddInstruction("get_local " + std::to_string(calleeContext.pos));
			}
		}

		generator.AddInstruction("call " + std::to_string(m_arguments.size()));

		if (m_isNativeCallee)
		{
			generator.AddInstruction("pop");
		}
	};

private:
	std::string m_callee;
	std::vector<ExpressionPtr> m_arguments;
	bool m_isNativeCallee = false;
};
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
		if (m_isNativeCallee)
		{
			GenerateNativeFunctionCall(generator);
		}
		else
		{
			GenerateCustomFunctionCall(generator);
		}
	};

private:
	void GenerateNativeFunctionCall(CodeGenerator& generator) const
	{
		for (auto const& argument : m_arguments)
		{
			argument->Generate(generator);
		}
		generator.AddInstruction("get_global " + std::to_string(generator.GetConstantPosOrAdd(PrimitiveType::STRING, m_callee)));
		generator.AddInstruction("call " + std::to_string(m_arguments.size()));
		if (GetType() == PrimitiveType::VOID)
		{
			generator.AddInstruction("pop");
		}
	}

	void GenerateCustomFunctionCall(CodeGenerator& generator) const
	{
		for (auto const& argument : m_arguments | std::views::reverse)
		{
			generator.ShouldNotClosureFunctionDirectly();
			argument->Generate(generator);
			generator.ClosureFunction();
		}

		const auto calleeContext = generator.GetVariableContextPos(m_callee);
		if (calleeContext.isVariableFromParent)
		{
			generator.AddInstruction("get_upvalue " + std::to_string(calleeContext.pos));
		}
		else
		{
			generator.AddInstruction("get_local " + std::to_string(calleeContext.pos));
		}
		if (m_arguments.empty())
		{
			generator.AddInstruction("call 0");
		}
		for (auto const& argument : m_arguments)
		{
			generator.AddInstruction("call 1");
		}
	}

	std::string m_callee;
	std::vector<ExpressionPtr> m_arguments;
	bool m_isNativeCallee = false;
};
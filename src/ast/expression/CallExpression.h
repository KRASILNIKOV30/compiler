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

		size_t functionPos;
		if (m_isNativeCallee)
		{
			functionPos = generator.GetConstantPosOrAdd(PrimitiveType::STRING, m_callee);
		}
		else
		{
			functionPos = generator.GetFunctionPos(m_callee);
		}

		generator.AddInstruction("get_global " + std::to_string(functionPos));
		generator.AddInstruction("call");

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
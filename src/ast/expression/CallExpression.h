#pragma once
#include "Expression.h"
#include <vector>

class CallExpression : public Expression
{
public:
	explicit CallExpression(std::string callee, Type type, std::vector<ExpressionPtr>&& arguments)
		: Expression(std::move(type))
		, m_callee(std::move(callee))
		, m_arguments(std::move(arguments))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		for (auto const& argument : m_arguments)
		{
			argument->Generate(generator);
		}

		auto functionPos = generator.GetConstantPosOrAdd({ PrimitiveType::STRING }, m_callee);
		generator.AddInstruction("get_global " + functionPos);
		generator.AddInstruction("call");
	};

private:
	std::string m_callee;
	std::vector<ExpressionPtr> m_arguments;
};
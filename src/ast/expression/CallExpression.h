#pragma once
#include "Expression.h"
#include <vector>

struct CallExpression : Expression
{
public:
	explicit CallExpression(std::string callee, Type const& type, std::vector<Expression> arguments)
		: Expression(type)
		, m_callee(callee)
		, m_arguments(arguments)
	{
	}

	void Generate(CodeGenerator& generator) const override {
		for (auto const& argument : m_arguments)
		{
			argument.Generate(generator);
		}

		auto functionPos = generator.GetConstantPosOrAdd({ PrimitiveType::STRING }, m_callee);
		generator.AddInstruction("get_global " + functionPos);
		generator.AddInstruction("call");
	};

private:
	std::string m_callee;
	std::vector<Expression> m_arguments;
};
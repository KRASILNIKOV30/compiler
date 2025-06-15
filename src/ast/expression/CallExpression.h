#pragma once
#include "Expression.h"
#include <vector>

class CallExpression : Expression
{
public:
	explicit CallExpression(std::string const& callee, Type const& type, std::vector<ExpressionPtr> const& arguments)
		: Expression(type)
		, m_callee(callee)
		, m_arguments(arguments)
	{
	}

	void Generate(CodeGenerator& generator) const override {
		for (auto const& argument : m_arguments)
		{
			argument->Generate(generator);
		}

		if (NATIVE_FUNCTIONS_WITH_ARGUMENTS_COUNT.contains(m_callee))
		{
			if (m_arguments.size() != NATIVE_FUNCTIONS_WITH_ARGUMENTS_COUNT.at(m_callee))
			{
				throw std::runtime_error("Invalid count of arguments of " + m_callee);
			}
			const auto functionPos = generator.GetConstantPosOrAdd({ PrimitiveType::STRING }, m_callee);
			generator.AddInstruction("get_global " + functionPos);
			generator.AddInstruction("call");
		}
		else
		{
			// TODO: доделать вместе с функциями
		}
	};

private:
	std::string m_callee;
	std::vector<ExpressionPtr> m_arguments;

	static inline const std::unordered_map<std::string, int> NATIVE_FUNCTIONS_WITH_ARGUMENTS_COUNT = {
		{"print", 1},
		{"println", 1},
		{"getTimestamp", 0},
		{"sqrt", 1},
	};
};
#pragma once
#include "../statement/BlockStatement.h"
#include "Declaration.h"
#include "../expression/Expression.h"

class FunctionDeclaration : public Declaration
{
public:
	explicit FunctionDeclaration(std::string id, Type type, std::vector<ExpressionPtr> arguments, BlockStatement body)
		: Declaration(std::move(id), std::move(type))
		, m_arguments(std::move(arguments))
		, m_body(std::move(body))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		static auto functionId = 0;

		generator.AddInstruction(".function" + std::to_string(++functionId));
		m_body.Generate(generator);
		generator.AddInstruction("return");
	}


private:
	std::vector<ExpressionPtr> m_arguments;
	BlockStatement m_body;
};
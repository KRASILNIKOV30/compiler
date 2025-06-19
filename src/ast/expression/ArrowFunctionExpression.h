#pragma once
#include "../statement/BlockStatement.h"
#include "Expression.h"
#include <variant>
#include <vector>

class ArrowFunctionExpression : public Expression
{
public:
	ArrowFunctionExpression(Type type, std::vector<std::string> params, ExpressionPtr expression = nullptr)
		: Expression(std::move(type))
		, m_params(std::move(params))
	{
		if (expression)
		{
			m_body = std::move(expression);
		}
		else
		{
			m_body = std::make_unique<BlockStatement>();
		}
	}

	BlockStatement* GetBlock()
	{
		if (!holds_alternative<BlockStatementPtr>(m_body))
		{
			throw std::logic_error("Attempt to get block from arrow function with expression body");
		}
		return get<BlockStatementPtr>(m_body).get();
	}

	void Generate(CodeGenerator &generator) const override
	{
		if (m_params.empty())
		{
			GenerateFunctionBeginning(generator, 0);
		}
		for (auto const& param: m_params)
		{
			GenerateFunctionBeginning(generator, 1);
			generator.GetVariablePosOrAdd(param);
		}

		std::visit([&](const auto& body) { Generate(generator, body); }, m_body);

		for (auto const& param: m_params)
		{
			GenerateFunctionEnding(generator);
		}
	}

private:
	static void GenerateFunctionBeginning(CodeGenerator& generator, int argc)
	{
		static auto arrowFunctionId = 0;
		const auto currentArrowFunctionId = ++arrowFunctionId;

		generator.AddInstruction("load_fn " + std::to_string(currentArrowFunctionId));
		generator.AddInstruction("closure");

		const std::string arrowFunctionName = "arrowFunction" + std::to_string(currentArrowFunctionId);
		generator.BeginFunction(arrowFunctionName, argc);
	}

	static void GenerateFunctionEnding(CodeGenerator& generator)
	{
		generator.EndFunction();
	}

	static void Generate(CodeGenerator& generator, ExpressionPtr const& expression)
	{
		expression->Generate(generator);
	}

	static void Generate(CodeGenerator& generator, BlockStatementPtr const& block)
	{
		block->Generate(generator);
	}

	std::vector<std::string> m_params;
	std::variant<ExpressionPtr, BlockStatementPtr> m_body;
};

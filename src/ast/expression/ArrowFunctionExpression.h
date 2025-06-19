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

		for (size_t i = 0; i < m_params.size(); ++i)
		{
			GenerateFunctionBeginning(generator, 1);
			generator.GetVariablePosOrAdd(m_params[i]);

			if (i > 0)
			{
				generator.AddParentLocal(m_params[i - 1]);
			}
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
		auto& labelGenerator = generator.GetLabelGenerator();
		labelGenerator.NewArrowFunctionLabel();

		generator.AddInstruction("load_fn " + std::to_string(labelGenerator.GetArrowFunctionLabelId()));
		generator.AddInstruction("closure");

		generator.BeginFunction(labelGenerator.GetArrowFunctionLabel(), argc);
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

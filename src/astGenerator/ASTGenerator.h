#pragma once
#include "../ast/declaration/VariableDeclaration.h"
#include "../ast/expression/BinaryExpression.h"
#include "../ast/expression/CallExpression.h"
#include "../ast/expression/Operators.h"
#include "../ast/expression/Term.h"
#include "../ast/Program.h"
#include "../ast/statement/AssignmentStatement.h"
#include "../lexer/token/Token.h"
#include "Calculate.h"
#include "CalculateCallExpressionType.h"
#include "CalculateType.h"
#include "SymbolTable.h"
#include <stack>

using Node = std::variant<std::string, Token>;
using Nodes = std::vector<Node>;

class ASTGenerator
{
public:
	void Generate(std::string const& rule, Nodes const& nodes)
	{
		if (rule == "<term>")
		{
			GenerateTerm(nodes);
		}
		else if (rule == "<ident>")
		{
			GenerateIdent(nodes);
		}
		else if (rule == "<constDeclaration>")
		{
			DeclareVar(nodes, true);
		}
		else if (rule == "<lowPriorityOp>" || rule == "<highPriorityOp>" || rule == "<relOp>")
		{
			SaveBinaryOperator(nodes);
		}
		else if (rule == "<simexp>" || rule == "<simterm>" || rule == "<expression>")
		{
			FoldBinaryOperator(nodes);
		}
		else if (rule == "<ident>")
		{
			SaveIdent(nodes);
		}
		else if (rule == "<assignmentStatement>")
		{
			GenerateAssignment(nodes);
		}
	}

	Program GetProgram()
	{
		return std::exchange(m_program, Program{});
	}

private:
	void GenerateAssignment(Nodes const& nodes)
	{
		const auto left = get<Token>(nodes[0]).value;
		const auto [isConst, type] = m_table.Get(left);
		if (isConst)
		{
			throw std::runtime_error("Attempt to assign value to constant " + left);
		}
		m_program.Add(std::make_unique<AssignmentStatement>(left, PopExpression()));
	}

	void SaveIdent(Nodes const& nodes)
	{
		m_ident = get<Token>(nodes[0]).value;
	}

	void FoldBinaryOperator(Nodes const& nodes)
	{
		if (nodes.size() == 1)
		{
			return;
		}

		auto right = PopExpression();
		auto left = PopExpression();
		const auto binOp = PopBinaryOperator();

		const auto leftType = get<PrimitiveType>(left->GetType().type);
		const auto rightType = get<PrimitiveType>(right->GetType().type);
		const auto type = CalculateType(leftType, rightType, binOp);

		if (left->HasValue() && right->HasValue())
		{
			const auto result = Calculate(
				left->GetValue(),
				right->GetValue(),
				leftType,
				rightType,
				binOp);
			ExpressionPtr term = std::make_unique<Term>(result, type, false);
			m_exprStack.emplace(std::move(term));
		}
		else
		{
			ExpressionPtr expr = std::make_unique<BinaryExpression>(
				std::move(left),
				std::move(right),
				binOp,
				type);
			m_exprStack.emplace(std::move(expr));
		}
	}

	void SaveBinaryOperator(Nodes const& nodes)
	{
		const auto strBinOper = get<Token>(nodes[0]).value;
		const auto binOper = StringToBinaryOperator(strBinOper);
		m_binOps.push(binOper);
	}

	void GenerateTerm(Nodes const& nodes)
	{
		if (nodes.size() == 1)
		{
			const auto& node = nodes.back();

			if (!std::holds_alternative<Token>(node))
			{
				return;
			}

			const auto& token = get<Token>(node);
			const auto& type = GetPrimitiveType(token.type);
			ExpressionPtr term = std::make_unique<Term>(token.value, type, false);
			m_exprStack.emplace(std::move(term));
		}
		// TODO: добавить обработку более длинных терминалов
	}

	void GenerateIdent(Nodes const& nodes)
	{
		if (nodes.size() == 1)
		{
			const auto& node = nodes.back();
			const auto& token = get<Token>(node);
			const auto name = token.value;
			const auto& symbol = m_table.Get(name);
			ExpressionPtr term = std::make_unique<Term>(name, symbol.type, true);
			m_exprStack.emplace(std::move(term));
			return;
		}

		const auto brace = get<Token>(nodes[1]).value;
		if (brace == "(")
		{
			std::vector<ExpressionPtr> arguments;
			while (m_exprStack.size() > 1)
			{
				arguments.emplace_back(PopExpression());
			}
			std::ranges::reverse(arguments);
			auto callee = PopExpression()->GetValue();
			const auto function = m_table.Get(callee);
			const Type calleeType = CalculateCallExpressionType(function.type, arguments);
			ExpressionPtr callExpr = std::make_unique<CallExpression>(callee, calleeType, std::move(arguments));
			m_exprStack.emplace(std::move(callExpr));
		}
		else
		{
			// id[0]
		}
	}

	void DeclareVar(Nodes const& nodes, bool isConst)
	{
		const auto& token = get<Token>(nodes[1]);
		auto expr = PopExpression();
		const auto type = expr->GetType();
		const auto id = token.value;
		m_table.Add(id, { isConst, type });

		DeclarationPtr decl = std::make_unique<VariableDeclaration>(id, type, std::move(expr));
		m_program.Add(std::move(decl));
	}

	ExpressionPtr PopExpression()
	{
		auto expr = std::move(m_exprStack.top());
		m_exprStack.pop();
		return expr;
	}

	BinaryOperators PopBinaryOperator()
	{
		auto binOp = m_binOps.top();
		m_binOps.pop();
		return binOp;
	}

private:
	SymbolTable m_table;
	std::stack<ExpressionPtr> m_exprStack;
	Program m_program;
	std::stack<BinaryOperators> m_binOps;
	std::optional<std::string> m_ident;
};

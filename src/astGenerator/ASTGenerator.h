#pragma once
#include "../ast/declaration/VariableDeclaration.h"
#include "../ast/expression/Operators.h"
#include "../ast/expression/Term.h"
#include "../ast/Program.h"
#include "../lexer/token/Token.h"
#include "Calculate.h"
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
	}

	Program GetProgram()
	{
		return std::exchange(m_program, Program{});
	}

private:
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

		const auto right = std::move(m_exprStack.top());
		m_exprStack.pop();
		const auto left = std::move(m_exprStack.top());
		m_exprStack.pop();
		const auto binOp = m_binOps.top();
		m_binOps.pop();

		if (left->HasValue() && right->HasValue())
		{
			// TODO: Проверить типы
			const auto result = Calculate(left->GetValue(), right->GetValue(), binOp);
			GenerateTerm({ { Token{ TokenType::FLOAT, result, 0, 0 } } });
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
			if (holds_alternative<Token>(node))
			{
				const auto& token = get<Token>(node);
				const auto& type = GetPrimitiveType(token.type);
				const Type termType = { type };
				ExpressionPtr term = std::make_unique<Term>(token.value, termType, false);
				m_exprStack.emplace(std::move(term));
			}
			else
			{
				const auto rule = get<std::string>(node);
				if (rule == "<ident>")
				{
					// TODO: проверить по таблице символов
				}
			}
		}
	}

	void DeclareVar(Nodes const& nodes, bool isConst)
	{
		const auto& token = get<Token>(nodes[1]);
		auto expr = std::move(m_exprStack.top());
		m_exprStack.pop();
		const auto type = expr->GetType();
		const auto id = token.value;
		m_table.Add(id, { isConst, { type } });

		DeclarationPtr decl = std::make_unique<VariableDeclaration>(id, Type{ type }, std::move(expr));
		m_program.Add(std::move(decl));
	}

private:
	SymbolTable m_table;
	std::stack<ExpressionPtr> m_exprStack;
	Program m_program;
	std::stack<BinaryOperators> m_binOps;
	std::optional<std::string> m_ident;
};

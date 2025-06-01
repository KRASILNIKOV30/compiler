#pragma once
#include "../ast/declaration/VariableDeclaration.h"
#include "../ast/expression/Term.h"
#include "../ast/Program.h"
#include "../lexer/token/Token.h"
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
	}

	Program GetProgram()
	{
		return std::exchange(m_program, Program{});
	}

private:
	void GenerateTerm(Nodes const& nodes)
	{
		if (nodes.size() == 1)
		{
			const auto& node = nodes.back();
			if (holds_alternative<Token>(node))
			{
				const auto& token = get<Token>(node);
				const auto& type = GetPrimitiveType(token.type);
				ExpressionPtr term = std::make_unique<Term>(token.value, type, false);
				m_exprStack.emplace(std::move(term));
			}
		}
	}

	void DeclareVar(Nodes const& nodes, bool isConst)
	{
		const auto& token = get<Token>(nodes[1]);
		auto& expr = m_exprStack.top();
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
};

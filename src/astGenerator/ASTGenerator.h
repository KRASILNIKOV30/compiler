#pragma once
#include "../ast/declaration/VariableDeclaration.h"
#include "../ast/expression/ArrowFunctionExpression.h"
#include "../ast/expression/BinaryExpression.h"
#include "../ast/expression/CallExpression.h"
#include "../ast/expression/InitializerListExpression.h"
#include "../ast/expression/Operators.h"
#include "../ast/expression/Term.h"
#include "../ast/Program.h"
#include "../ast/statement/AssignmentStatement.h"
#include "../ast/statement/ExpressionStatement.h"
#include "../ast/statement/IfStatement.h"
#include "../ast/statement/ReturnStatement.h"
#include "../ast/statement/WhileStatement.h"
#include "../lexer/token/Token.h"
#include "Calculate.h"
#include "CalculateCallExpressionType.h"
#include "CalculateType.h"
#include "SymbolTable.h"
#include <iostream>
#include <stack>

using Node = std::variant<std::string, Token>;
using Nodes = std::vector<Node>;

class ASTGenerator
{
public:
	ASTGenerator()
	{
		m_blockStack.emplace(&m_program);
	}

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
		else if (rule == "<variableDeclaration>")
		{
			DeclareVar(nodes, false);
		}
		else if (rule == "<lowPriorityOp>" || rule == "<highPriorityOp>" || rule == "<relOp>")
		{
			SaveBinaryOperator(nodes);
		}
		else if (rule == "<simexp>" || rule == "<simterm>" || rule == "<expression>")
		{
			FoldBinaryOperator(nodes);
		}
		else if (rule == "<assignmentStatement>")
		{
			GenerateAssignment(nodes);
		}
		else if (rule == "<expressionStatement>")
		{
			FlushExpression();
		}
		else if (rule == "<openBlock>")
		{
			OpenBlock();
		}
		else if (rule == "<closeBlock>")
		{
			CloseBlock();
		}
		else if (rule == "<whileHead>")
		{
			GenerateWhile();
		}
		else if (rule == "<ifHead>")
		{
			GenerateIf();
		}
		else if (rule == "<elseHead>")
		{
			GenerateElse();
		}
		else if (rule == "<elseIfHead>")
		{
			GenerateElseIf();
		}
		else if (rule == "<ifStatement>" || rule == "<whileStatement>")
		{
			CloseIf();
		}
		else if (rule == "<initializerList>")
		{
			GenerateInitializerList();
		}
		else if (rule == "<primitiveType>")
		{
			SavePrimitiveType(nodes);
		}
		else if (rule == "<parameter>")
		{
			GenerateParameter(nodes);
		}
		else if (rule == "<arrowFunctionWithExpr>")
		{
			GenerateArrowFunctionWithExpr();
		}
		else if (rule == "<arrowFunctionWithBlockHead>")
		{
			GenerateArrowFunctionWithBlock();
		}
		else if (rule == "<arrowFunctionWithBlock>")
		{
			CloseFunctionBlock();
		}
		else if (rule == "<returnStatement>")
		{
			GenerateReturn();
		}
		else if (rule == "<arrayItem>")
		{
			SaveArrayItem();
		}
		else if (rule == "<arg>")
		{
			SaveFunctionArgument();
		}
		else if (rule == "<arrowFunctionHead>")
		{
			GenerateArrowFunctionHead();
		}
	}

	Program GetProgram()
	{
		return std::exchange(m_program, Program{});
	}

private:
	void GenerateArrowFunctionHead()
	{
		if (m_parameters.empty())
		{
			m_table.CreateScope();
		}
	}

	void SaveFunctionArgument()
	{
		m_functionArgumentsStack.emplace(PopExpression());
	}

	void SaveArrayItem()
	{
		m_arrayItemStack.emplace(PopExpression());
	}

	void GenerateReturn()
	{
		if (!m_openedFunctionPtr)
		{
			throw std::runtime_error("Return can be called in function only.");
		}
		auto ft = get<FunctionType>(m_openedFunctionPtr->GetType().type);
		ExpressionPtr expr;
		if (m_exprStack.empty())
		{
			expr = nullptr;
			ft.emplace_back(PrimitiveType::VOID);
		}
		else
		{
			expr = PopExpression();
			ft.emplace_back(expr->GetType());
		}
		m_openedFunctionPtr->SetType(ft);
		m_openedFunctionPtr = nullptr;
		Add(std::make_unique<ReturnStatement>(std::move(expr)));
	}

	void GenerateArrowFunctionWithBlock()
	{
		auto ft = GetFunctionParamsType();
		auto arrowFunction = std::make_unique<ArrowFunctionExpression>(ft, PopParameters());
		auto block = arrowFunction->GetBlock();
		m_openedFunctionPtr = arrowFunction.get();
		m_exprStack.emplace(std::move(arrowFunction));
		m_blockStack.emplace(block);
		m_ignoreNextOpenBlock = true;
	}

	void CloseFunctionBlock()
	{
		if (m_openedFunctionPtr)
		{
			auto ft = get<FunctionType>(m_openedFunctionPtr->GetType().type);
			ft.emplace_back(PrimitiveType::VOID);
			m_openedFunctionPtr->SetType(ft);
		}
		m_openedFunctionPtr = nullptr;
	}

	void GenerateArrowFunctionWithExpr()
	{
		auto ft = GetFunctionParamsType();
		auto expr = PopExpression();
		ft.emplace_back(expr->GetType());

		auto arrowFunctionExpr = std::make_unique<ArrowFunctionExpression>(ft, PopParameters(), std::move(expr));
		m_exprStack.emplace(std::move(arrowFunctionExpr));
	}

	FunctionType GetFunctionParamsType()
	{
		FunctionType ft{};
		for (const auto& paramType : m_parameters
				| std::views::transform([&](const auto& p) { return m_table.Get(p).type; }))
		{
			ft.emplace_back(paramType);
		}
		if (ft.empty())
		{
			ft.emplace_back(PrimitiveType::VOID);
		}
		return ft;
	}

	void GenerateParameter(Nodes const& nodes)
	{
		const auto name = get<Token>(nodes.front()).value;
		if (m_parameters.empty())
		{
			m_table.CreateScope();
		}
		m_parameters.emplace_back(name);
		m_table.Add(name, { false, m_type.value() });
		m_type.reset();
	}

	void SavePrimitiveType(Nodes const& nodes)
	{
		auto pt = GetPrimitiveType(get<Token>(nodes[0]).type);
		if (m_type.has_value())
		{
			if (holds_alternative<FunctionType>(m_type->type))
			{
				auto ft = get<FunctionType>(m_type->type);
				ft.emplace_back(pt);
				m_type.emplace(ft);
			}
			else if (holds_alternative<ArrayTypePtr>(m_type->type))
			{
				auto at = get<ArrayTypePtr>(m_type->type);
				m_type.emplace(FunctionType{ at, pt });
			}
			else
			{
				auto ptCurrent = get<PrimitiveType>(m_type->type);
				m_type.emplace(FunctionType{ ptCurrent, pt });
			}
		}
		else
		{
			m_type.emplace(pt);
		}
	}

	void GenerateInitializerList()
	{
		std::vector<ExpressionPtr> exprList;
		std::optional<Type> arrayElementType;
		while (!m_arrayItemStack.empty())
		{
			auto expr = PopArrayItem();
			if (arrayElementType.has_value())
			{
				const auto currentType = expr->GetType();
				if (!(currentType == arrayElementType.value()))
				{
					std::string errorMessage = "Inconsistent types in initializer list. "
											   "All elements must have the same type. "
											   "Expected type '"
						+ TypeToString(arrayElementType.value()) + "' (based on the first element), "
																   "but found type '"
						+ TypeToString(currentType) + "'.";
					throw std::runtime_error(errorMessage);
				}
			}
			else
			{
				arrayElementType = expr->GetType();
			}
			exprList.emplace_back(std::move(expr));
		}
		std::ranges::reverse(exprList);

		// TODO: Добавить обработку пустого массива

		auto arrayType = std::make_shared<ArrayType>(arrayElementType.value());
		auto initializerList = std::make_unique<InitializerListExpression>(arrayType, std::move(exprList));
		m_exprStack.emplace(std::move(initializerList));
	}

	void GenerateWhile()
	{
		auto condition = GetConditionExpression();
		auto whileStatement = std::make_unique<WhileStatement>(std::move(condition));
		auto blockPtr = whileStatement->GetBlock();
		Add(std::move(whileStatement));
		OpenOperatorBlock(blockPtr);
	}

	void CloseIf()
	{
		m_ifStack.pop();
	}

	void GenerateIf()
	{
		auto condition = GetConditionExpression();
		auto ifStatement = std::make_unique<IfStatement>(std::move(condition));
		const auto thenBlockPtr = ifStatement->GetThenBlock();
		const auto ifPtr = ifStatement.get();
		Add(std::move(ifStatement));
		OpenOperatorBlock(thenBlockPtr);
		m_ifStack.emplace(ifPtr);
	}

	ExpressionPtr GetConditionExpression()
	{
		auto expr = PopExpression();
		auto type = expr->GetType();
		if (!(type == PrimitiveType::BOOL))
		{
			throw std::runtime_error("Condition must be a boolean expression, but got '" + TypeToString(type) + "'.");
		}

		return std::move(expr);
	}

	void GenerateElse()
	{
		const auto currentIf = m_ifStack.top();
		const auto elseBlock = currentIf->CreateElseBlock();
		OpenOperatorBlock(elseBlock);
	}

	void GenerateElseIf()
	{
		const auto currentIf = m_ifStack.top();
		m_ifStack.pop();
		auto condition = GetConditionExpression();
		const auto elseIf = currentIf->CreateElseIfBlock(std::move(condition));
		OpenOperatorBlock(elseIf->GetThenBlock());
		m_ifStack.emplace(elseIf);
	}

	void OpenOperatorBlock(BlockStatement* block)
	{
		m_blockStack.emplace(block);
		m_table.CreateScope();
		m_ignoreNextOpenBlock = true;
	}

	void OpenBlock()
	{
		if (!m_ignoreNextOpenBlock)
		{
			auto block = std::make_unique<BlockStatement>();
			m_blockStack.emplace(block.get());
			Add(std::move(block));
			m_table.CreateScope();
		}
		m_ignoreNextOpenBlock = false;
	}

	void CloseBlock()
	{
		m_blockStack.pop();
		m_table.DeleteScope();
	}

	void GenerateAssignment(Nodes const& nodes)
	{
		auto right = PopExpression();

		std::optional<ExpressionPtr> index = std::nullopt;

		auto leftExpr = PopExpression();
		auto leftRawPtr = dynamic_cast<MemberExpression*>(leftExpr.get());
		std::ignore = leftExpr.release();
		auto leftMember = std::unique_ptr<MemberExpression>(leftRawPtr);
		leftMember->MakeLValue();
		auto memberName = leftMember->GetValue();

		const auto [isConst, type, _] = m_table.Get(memberName);
		if (isConst && !std::holds_alternative<ArrayTypePtr>(type.type))
		{
			throw std::runtime_error("Attempt to assign value to constant " + memberName);
		}

		Add(std::make_unique<AssignmentStatement>(std::move(leftMember), std::move(right)));
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
			ExpressionPtr member = std::make_unique<MemberExpression>(symbol.type, name, std::nullopt);
			m_exprStack.emplace(std::move(member));
			return;
		}

		const auto brace = get<Token>(nodes[1]).value;
		if (brace == "(")
		{
			std::vector<ExpressionPtr> arguments;
			while (!m_functionArgumentsStack.empty())
			{
				arguments.emplace_back(PopFunctionArgument());
			}
			std::ranges::reverse(arguments);
			auto callee = PopExpression()->GetValue();
			const auto function = m_table.Get(callee);
			const Type calleeType = CalculateCallExpressionType(function.type, arguments);
			ExpressionPtr callExpr = std::make_unique<CallExpression>(callee, calleeType, std::move(arguments), function.isNative);
			m_exprStack.emplace(std::move(callExpr));
		}
		else
		{
			auto index = PopExpression();
			auto idExpr = PopExpression();
			auto name = idExpr->GetValue();
			auto type = idExpr->GetType();
			if (!std::holds_alternative<ArrayTypePtr>(type.type))
			{
				throw std::runtime_error("Type '" + TypeToString(type) + "' does not provide a subscript operator.");
			}
			const auto arrayItemType = std::get<ArrayTypePtr>(type.type)->elementType;
			auto member = std::make_unique<MemberExpression>(arrayItemType, name, std::move(index));
			m_exprStack.emplace(std::move(member));
		}
	}

	void FlushExpression()
	{
		auto exprStatement = std::make_unique<ExpressionStatement>(PopExpression());
		Add(std::move(exprStatement));
	}

	void DeclareVar(Nodes const& nodes, bool isConst)
	{
		const auto& token = get<Token>(nodes[1]);
		auto expr = PopExpression();
		const auto type = expr->GetType();
		const auto id = token.value;
		m_table.Add(id, { isConst, type });

		DeclarationPtr decl = std::make_unique<VariableDeclaration>(id, type, std::move(expr));
		Add(std::move(decl));
	}

	ExpressionPtr PopExpression()
	{
		if (m_exprStack.empty())
		{
			throw std::logic_error("Pop expression from an empty stack\n");
		}
		auto expr = std::move(m_exprStack.top());
		m_exprStack.pop();
		return expr;
	}

	ExpressionPtr PopArrayItem()
	{
		if (m_arrayItemStack.empty())
		{
			throw std::logic_error("Pop array item from an empty stack\n");
		}
		auto item = std::move(m_arrayItemStack.top());
		m_arrayItemStack.pop();
		return item;
	}

	ExpressionPtr PopFunctionArgument()
	{
		if (m_functionArgumentsStack.empty())
		{
			throw std::logic_error("Pop function argument from an empty stack\n");
		}
		auto arg = std::move(m_functionArgumentsStack.top());
		m_functionArgumentsStack.pop();
		return arg;
	}

	BinaryOperator PopBinaryOperator()
	{
		auto binOp = m_binOps.top();
		m_binOps.pop();
		return binOp;
	}

	void Add(ProgramNode&& node)
	{
		m_blockStack.top()->Add(std::move(node));
	}

	std::vector<std::string> PopParameters()
	{
		const auto copy = m_parameters;
		m_parameters.clear();
		return copy;
	}

private:
	SymbolTable m_table;
	std::stack<ExpressionPtr> m_exprStack;
	std::stack<ExpressionPtr> m_arrayItemStack;
	std::stack<ExpressionPtr> m_functionArgumentsStack;
	std::stack<BinaryOperator> m_binOps;

	std::stack<BlockStatement*> m_blockStack;
	std::stack<IfStatement*> m_ifStack;
	std::optional<Type> m_type = std::nullopt;
	std::vector<std::string> m_parameters;
	ArrowFunctionExpression* m_openedFunctionPtr = nullptr;

	Program m_program;
	bool m_ignoreNextOpenBlock = false;
};

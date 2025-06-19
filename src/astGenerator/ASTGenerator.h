#pragma once
#include "../ast/declaration/Declaration.h"
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
#include "Adapter.h"
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
		else if (rule == "<arrayType>")
		{
			MakeLastTypeArray();
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
		else if (rule == "<adapter>")
		{
			SaveAdapter(nodes);
		}
		else if (rule == "<iterHead>")
		{
			GenerateIterHead(nodes);
		}
		else if (rule == "<iterStatement>")
		{
			CloseIter();
		}
	}

	Program GetProgram()
	{
		return std::exchange(m_program, Program{});
	}

private:
	void CloseIter()
	{
		for (int i = 0; i < m_iterOpenBlocks; ++i)
		{
			m_blockStack.pop();
		}
		m_iterOpenBlocks = 0;
	}

	void GenerateIterHead(Nodes const& nodes)
	{
		const auto arrItemName = get<Token>(nodes[2]).value;
		auto arrayExpr = PopExpression();
		const auto arrayExprType = arrayExpr->GetType();
		if (!holds_alternative<ArrayTypePtr>(arrayExprType.type))
		{
			throw std::runtime_error("Array expected in iter, but " + TypeToString(arrayExprType) + " provided.");
		}
		const auto arrayElementType = get<ArrayTypePtr>(arrayExprType.type)->elementType;

		// Generate iter actions before while

		Add(std::make_unique<Declaration>("_arr", arrayExprType, std::move(arrayExpr)));

		auto beginInit = std::make_unique<Term>("0", PrimitiveType::INT, false);
		Add(std::make_unique<Declaration>("_begin", PrimitiveType::INT, std::move(beginInit)));

		auto arrTerm = std::make_unique<Term>("_arr", arrayExprType, true);
		std::vector<ExpressionPtr> argument;
		argument.emplace_back(std::move(arrTerm));

		auto callExpr = std::make_unique<CallExpression>("arrayLength", PrimitiveType::INT, std::move(argument), true);
		Add(std::make_unique<Declaration>("_end", PrimitiveType::INT, std::move(callExpr)));

		auto stepInit = std::make_unique<Term>("1", PrimitiveType::INT, false);
		Add(std::make_unique<Declaration>("_step", PrimitiveType::INT, std::move(stepInit)));

		int temp = 0;
		for (auto& adapter : m_adapters
				| std::views::filter([](const auto& a) { return a.type == AdapterType::DROP || a.type == AdapterType::TAKE || a.type == AdapterType::REVERSE; }))
		{
			if (adapter.type == AdapterType::DROP)
			{
				auto beginMember = std::make_unique<MemberExpression>(PrimitiveType::INT, "_begin", std::nullopt, false);
				auto beginTerm = std::make_unique<Term>("_begin", PrimitiveType::INT, true);
				auto stepMult = std::make_unique<BinaryExpression>(
					std::make_unique<Term>("_step", PrimitiveType::INT, true),
					std::move(adapter.expr),
					BinaryOperator::MUL,
					PrimitiveType::INT);
				auto beginPlusStep
					= std::make_unique<BinaryExpression>(
						std::move(beginTerm),
						std::move(stepMult),
						BinaryOperator::PLUS,
						PrimitiveType::INT);
				Add(std::make_unique<AssignmentStatement>(std::move(beginMember), std::move(beginPlusStep)));
			}
			else if (adapter.type == AdapterType::TAKE)
			{
				auto stepMult = std::make_unique<BinaryExpression>(
					std::make_unique<Term>("_step", PrimitiveType::INT, true),
					std::move(adapter.expr),
					BinaryOperator::MUL,
					PrimitiveType::INT);
				auto endValue = std::make_unique<BinaryExpression>(
					std::make_unique<Term>("_begin", PrimitiveType::INT, true),
					std::move(stepMult),
					BinaryOperator::PLUS,
					PrimitiveType::INT);
				auto endMember = std::make_unique<MemberExpression>(PrimitiveType::INT, "_end", std::nullopt, false);
				Add(std::make_unique<AssignmentStatement>(std::move(endMember), std::move(endValue)));
			}
			else // REVERSE
			{
				const auto tempName = "_temp" + std::to_string(temp++);

				auto beginTerm = std::make_unique<Term>("_begin", PrimitiveType::INT, true);
				Add(std::make_unique<Declaration>(tempName, PrimitiveType::INT, std::move(beginTerm)));

				auto beginMember = std::make_unique<MemberExpression>(PrimitiveType::INT, "_begin", std::nullopt, false);
				auto endTerm = std::make_unique<Term>("_end", PrimitiveType::INT, true);
				Add(std::make_unique<AssignmentStatement>(std::move(beginMember),
					std::make_unique<BinaryExpression>(
						std::move(endTerm),
						std::make_unique<Term>("_step", PrimitiveType::INT, true),
						BinaryOperator::MINUS,
						PrimitiveType::INT)));

				auto endMember = std::make_unique<MemberExpression>(PrimitiveType::INT, "_end", std::nullopt, false);
				auto tempTerm = std::make_unique<Term>(tempName, PrimitiveType::INT, true);
				Add(std::make_unique<AssignmentStatement>(std::move(endMember),
					std::make_unique<BinaryExpression>(
						std::move(tempTerm),
						std::make_unique<Term>("_step", PrimitiveType::INT, true),
						BinaryOperator::MINUS,
						PrimitiveType::INT)));

				auto stepMult = std::make_unique<BinaryExpression>(
					std::make_unique<Term>("_step", PrimitiveType::INT, true),
					std::make_unique<Term>("-1", PrimitiveType::INT, false),
					BinaryOperator::MUL,
					PrimitiveType::INT);
				auto stepMember = std::make_unique<MemberExpression>(PrimitiveType::INT, "_step", std::nullopt, false);
				Add(std::make_unique<AssignmentStatement>(std::move(stepMember), std::move(stepMult)));
			}
		}
		// Generate arrow function
		// const _arrayFun = (index: int) -> arr[index];
		auto arrIdentifier = std::make_unique<MemberExpression>(arrayExprType, "_arr", std::nullopt);
		auto indexIdentifier = std::make_unique<Term>("index", PrimitiveType::INT, true);
		auto accessExpr = std::make_unique<MemberExpression>(arrayElementType, "_arr", std::move(indexIdentifier));

		std::vector<std::string> params = { "index" };

		FunctionType funcType;
		funcType.emplace_back(PrimitiveType::INT);
		funcType.emplace_back(arrayElementType);

		auto arrowFunc = std::make_unique<ArrowFunctionExpression>(
			std::move(funcType),
			params,
			std::move(accessExpr));

		Type arrowFuncType = arrowFunc->GetType();
		DeclarationPtr decl = std::make_unique<Declaration>("_arrayFun", arrowFuncType, std::move(arrowFunc));
		Add(std::move(decl));

		// Generate while
		// var _curr = _begin;
		// while (_curr < _end and curr < arrayLength(_arr))
		// {
		//		var item = _arrayFun(curr);
		// 1. Инициализация итератора: var _curr = _begin;
		{
			auto beginValue = std::make_unique<Term>("_begin", PrimitiveType::INT, true);
			auto currDecl = std::make_unique<Declaration>("_curr", PrimitiveType::INT, std::move(beginValue));
			Add(std::move(currDecl));
		}

		// 2. Создание условия для while: _curr < _end
		auto currLessThanEnd = std::make_unique<BinaryExpression>(
			std::make_unique<Term>("_curr", PrimitiveType::INT, true),
			std::make_unique<Term>("_end", PrimitiveType::INT, true),
			BinaryOperator::NOT_EQUAL,
			PrimitiveType::BOOL);

		// 3. Создаем узел WhileStatement с этим условием
		auto whileStatement = std::make_unique<WhileStatement>(std::move(currLessThanEnd));
		WhileStatement* whilePtr = whileStatement.get(); // Невладеющий указатель для работы

		// 4. Добавляем узел while в текущий блок (блок iter-а)
		Add(std::move(whileStatement));

		// 5. Делаем тело while-цикла новым активным блоком
		BlockStatement* whileBody = whilePtr->GetBlock();
		m_blockStack.push(whileBody);
		m_table.CreateScope();
		m_iterOpenBlocks = 1;

		// 6. Создаем вызов: _arrayFun(_curr)
		// 6.1. Аргумент вызова - _curr
		std::vector<ExpressionPtr> args;
		args.push_back(std::make_unique<Term>("_curr", PrimitiveType::INT, true));

		// 6.4. Создаем узел вызова
		auto callArrayFn = std::make_unique<CallExpression>("_arrayFun", arrayElementType, std::move(args), false);

		// 7. Создаем объявление переменной item: var item = _arrayFun(_curr);
		{
			Type itemType = callArrayFn->GetType();
			auto itemDecl = std::make_unique<Declaration>(arrItemName, itemType, std::move(callArrayFn));
			Add(std::move(itemDecl));
			m_table.Add(arrItemName, { false, itemType });
		}

		// 7. Генерируем инкремент: _curr = _curr + _step;
		//    !!! Мы делаем это ЗДЕСЬ, до обработки адаптеров !!!
		{
			auto currLValue = std::make_unique<MemberExpression>(Type(PrimitiveType::INT), "_curr", std::nullopt, false);
			auto currRValue = std::make_unique<Term>("_curr", PrimitiveType::INT, true);
			auto stepValue = std::make_unique<Term>("_step", PrimitiveType::INT, true);
			auto incrementExpr = std::make_unique<BinaryExpression>(std::move(currRValue), std::move(stepValue), BinaryOperator::PLUS, PrimitiveType::INT);

			// Добавляем инкремент в тело while
			Add(std::make_unique<AssignmentStatement>(std::move(currLValue), std::move(incrementExpr)));
		}

		// Generate filter and transform
		int adapterIndex = 0;
		for (auto& adapter : m_adapters
				| std::views::filter([](const auto& a) { return a.type == AdapterType::FILTER || a.type == AdapterType::TRANSFORM; }))
		{
			if (adapter.type == AdapterType::TRANSFORM)
			{
				std::string tempFuncName = "_t" + std::to_string(adapterIndex++);

				// 1. Объявляем временную константу для transform-функции
				auto transformFuncExpr = std::move(adapter.expr);
				Type funcType = transformFuncExpr->GetType();
				Add(std::make_unique<Declaration>(tempFuncName, funcType, std::move(transformFuncExpr)));

				// 2. Создаем вызов этой функции: _t0(item)
				std::vector<ExpressionPtr> args;
				args.emplace_back(std::make_unique<Term>(arrItemName, arrayElementType, true));
				auto callExpr = std::make_unique<CallExpression>(tempFuncName, arrayElementType, std::move(args));

				// 3. Создаем присваивание: item = ...
				auto itemLValue = std::make_unique<MemberExpression>(arrayElementType, arrItemName, std::nullopt, false);
				Add(std::make_unique<AssignmentStatement>(std::move(itemLValue), std::move(callExpr)));
			}
			else // FILTER
			{
				std::string tempFuncName = "_f" + std::to_string(adapterIndex++);

				// 1. Объявляем временную константу для filter-функции (предиката)
				auto predicateExpr = std::move(adapter.expr);
				Type funcType = predicateExpr->GetType();
				m_table.Add(tempFuncName, { true, funcType });
				Add(std::make_unique<Declaration>(tempFuncName, funcType, std::move(predicateExpr)));

				// 2. Создаем вызов предиката: _f0(item)
				std::vector<ExpressionPtr> args;
				args.emplace_back(std::make_unique<Term>(arrItemName, arrayElementType, true));

				Type resultType = CalculateCallExpressionType(funcType, args);
				// Проверяем, что предикат возвращает bool
				if (resultType != PrimitiveType::BOOL)
				{
					throw std::runtime_error("Filter predicate must return a boolean, but it returns '" + TypeToString(resultType) + "'.");
				}
				auto callExpr = std::make_unique<CallExpression>(tempFuncName, resultType, std::move(args));

				// 3. Создаем IfStatement с этим вызовом в качестве условия
				auto ifStatement = std::make_unique<IfStatement>(std::move(callExpr));
				BlockStatement* thenBlock = ifStatement->GetThenBlock();

				Add(std::move(ifStatement));
				m_blockStack.push(thenBlock);
				m_iterOpenBlocks++;
			}
		}

		m_adapters.clear();
	}

	void SaveAdapter(Nodes const& nodes)
	{
		const auto tokenType = get<Token>(nodes[0]).type;
		const auto adapterType = GetAdapterType(tokenType);
		ExpressionPtr expr = adapterType == AdapterType::REVERSE
			? nullptr
			: PopExpression();
		m_adapters.emplace_back(adapterType, std::move(expr));
	}

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
			if (!m_wasReturn)
			{
				ft.emplace_back(PrimitiveType::VOID);
			}
		}
		else
		{
			expr = PopExpression();
			if (!m_wasReturn)
			{
				ft.emplace_back(expr->GetType());
			}
		}
		m_wasReturn = true;
		m_openedFunctionPtr->SetType(ft);
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
		if (m_openedFunctionPtr && !m_wasReturn)
		{
			auto ft = get<FunctionType>(m_openedFunctionPtr->GetType().type);
			ft.emplace_back(PrimitiveType::VOID);
			m_openedFunctionPtr->SetType(ft);
		}
		m_openedFunctionPtr = nullptr;
		m_wasReturn = false;
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

	void MakeLastTypeArray()
	{
		if (holds_alternative<FunctionType>(m_type->type))
		{
			auto ft = get<FunctionType>(m_type->type);
			ft.back() = std::make_shared<ArrayType>(ft.back());
			m_type.emplace(ft);
		}
		else if (holds_alternative<ArrayTypePtr>(m_type->type))
		{
			auto at = get<ArrayTypePtr>(m_type->type);
			m_type.emplace(std::make_shared<ArrayType>(at));
		}
		else
		{
			auto pt = get<PrimitiveType>(m_type->type);
			m_type.emplace(std::make_shared<ArrayType>(pt));
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
			auto blockPtr = block.get();
			Add(std::move(block));
			m_blockStack.emplace(blockPtr);
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

		const auto [isConst, type, native, val] = m_table.Get(memberName);
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

		const auto leftValue = GetSymbolValue(left);
		const auto rightValue = GetSymbolValue(right);

		if (!leftValue.empty() && !rightValue.empty())
		{
			const auto result = Calculate(
				leftValue,
				rightValue,
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

	std::string GetSymbolValue(ExpressionPtr& expr)
	{
		const auto value = expr->GetValue();
		if (expr->HasValue())
		{
			return expr->GetValue();
		}
		const auto symbol = m_table.Find(value);
		if (symbol.has_value())
		{
			return symbol->value;
		}
		return "";
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
		const auto value = isConst ? GetSymbolValue(expr) : "";
		m_table.Add(id, { isConst, type, false, value });

		DeclarationPtr decl = std::make_unique<Declaration>(id, type, std::move(expr));
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
	bool m_wasReturn = false;

	std::vector<Adapter> m_adapters;
	int m_iterOpenBlocks = 0;

	Program m_program;
	bool m_ignoreNextOpenBlock = false;
};

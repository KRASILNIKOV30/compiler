#include "../../src/ast/expression/Expression.h"
#include "../../src/astGenerator/CalculateCallExpressionType.h"
#include <catch.hpp>

SCENARIO("Type checking function calls with value-like recursive types", "[type_checker]")
{
	GIVEN("A function of type INT -> BOOL -> STRING")
	{
		Type funcType{ PrimitiveType::INT, PrimitiveType::BOOL, PrimitiveType::STRING };

		WHEN("it is called with one correct argument (INT)")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::INT)));

			Type resultType = CalculateCallExpressionType(funcType, args);

			THEN("the result is a partially applied function of type BOOL -> STRING")
			{
				Type expectedType = { PrimitiveType::BOOL, PrimitiveType::STRING };
				REQUIRE(resultType == expectedType);
			}
		}

		WHEN("it is called with all correct arguments (INT, BOOL)")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(PrimitiveType::INT));
			args.push_back(std::make_unique<Expression>(PrimitiveType::BOOL));

			Type resultType = CalculateCallExpressionType(funcType, args);

			THEN("the result is the final return type STRING")
			{
				Type expectedType(PrimitiveType::STRING);
				REQUIRE(resultType == expectedType);
			}
		}

		WHEN("it is called with a wrong argument type (STRING)")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::STRING)));

			THEN("it throws a type mismatch exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(funcType, args),
					"Type mismatch in function call: Argument 1 has type 'string', but function expects 'number'.");
			}
		}
	}

	GIVEN("A higher-order function of type (INT -> BOOL) -> STRING")
	{
		Type paramType = { PrimitiveType::INT, PrimitiveType::BOOL };
		Type hofType(FunctionType({ paramType, PrimitiveType::STRING }));

		WHEN("it is called with a function of the correct type")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(paramType));

			Type resultType = CalculateCallExpressionType(hofType, args);

			THEN("the result is the final return type STRING")
			{
				REQUIRE(resultType == Type(PrimitiveType::STRING));
			}
		}

		WHEN("it is called with incorrect function type")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(FunctionType{ PrimitiveType::INT, PrimitiveType::INT }));

			THEN("it throws exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(hofType, args),
					"Type mismatch in function call: Argument 1 has type 'number -> number', but function expects 'number -> bool'.");
			}
		}
	}

	GIVEN("A higher-order function of type INT -> ((INT -> BOOL) -> INT) -> STRING")
	{
		Type innerFuncParamType = { PrimitiveType::INT, PrimitiveType::BOOL };
		Type innerFuncType(FunctionType({ innerFuncParamType, Type(PrimitiveType::INT) }));

		Type funcType(FunctionType({ Type(PrimitiveType::INT),
			innerFuncType,
			Type(PrimitiveType::STRING) }));

		WHEN("it is called with one argument (INT)")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::INT)));

			Type resultType = CalculateCallExpressionType(funcType, args);

			THEN("the result is a partially applied function of type ((INT -> BOOL) -> INT) -> STRING")
			{
				Type expectedType(FunctionType({ innerFuncType,
					Type(PrimitiveType::STRING) }));
				REQUIRE(resultType == expectedType);
			}
		}

		WHEN("it is called with two correct arguments (INT, and a function)")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::INT)));
			args.push_back(std::make_unique<Expression>(innerFuncType));

			Type resultType = CalculateCallExpressionType(funcType, args);

			THEN("the result is the final return type STRING")
			{
				Type expectedType(PrimitiveType::STRING);
				REQUIRE(resultType == expectedType);
			}
		}

		WHEN("the second argument is a function with a wrong signature, e.g. (INT -> INT) -> INT")
		{
			Type wrongInnerFuncParamType = { PrimitiveType::INT, PrimitiveType::INT };
			Type wrongInnerFuncType(FunctionType({ wrongInnerFuncParamType, Type(PrimitiveType::INT) }));

			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::INT)));
			args.push_back(std::make_unique<Expression>(wrongInnerFuncType));

			THEN("it throws a type mismatch exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(funcType, args),
					"Type mismatch in function call: Argument 2 has type '(number -> number) -> number', but function expects '(number -> bool) -> number'.");
			}
		}

		WHEN("the second argument is not a function, but a primitive type")
		{
			std::vector<ExpressionPtr> args;
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::INT)));
			args.push_back(std::make_unique<Expression>(Type(PrimitiveType::BOOL)));

			THEN("it throws a type mismatch exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(funcType, args),
					"Type mismatch in function call: Argument 2 has type 'bool', but function expects '(number -> bool) -> number'.");
			}
		}
	}
}
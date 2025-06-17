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

SCENARIO("function without args test")
{
	GIVEN("A function that expects arguments, e.g. INT -> BOOL")
	{
		Type funcType = { PrimitiveType::INT, PrimitiveType::BOOL };

		WHEN("it is called with no arguments, i.e. func()")
		{
			THEN("it throws a type mismatch exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(funcType, {}),
					"Type mismatch: Function expects an argument of type 'number' but was called with no arguments.");
			}
		}
	}

	GIVEN("A function of type () -> INT (represented as VOID -> INT)")
	{
		Type funcType = { PrimitiveType::VOID, PrimitiveType::INT };

		WHEN("it is called with no arguments, i.e. func()")
		{
			std::vector<ExpressionPtr> args;
			Type resultType = CalculateCallExpressionType(funcType, args);

			THEN("the result is the final return type INT")
			{
				REQUIRE(resultType == Type(PrimitiveType::INT));
			}
		}
	}

	GIVEN("A procedure of type INT -> VOID")
	{
		Type funcType = { PrimitiveType::VOID, PrimitiveType::VOID };

		WHEN("it is called correctly")
		{
			const auto resultType = CalculateCallExpressionType(funcType, {});

			THEN("the result type is VOID")
			{
				REQUIRE(resultType == PrimitiveType::VOID);
			}
		}
	}

	GIVEN("A non-function type, e.g. INT")
	{
		Type nonFuncType(PrimitiveType::INT);

		WHEN("an attempt is made to call it, i.e. my_int_var()")
		{
			std::vector<ExpressionPtr> args;

			THEN("it throws a 'calling non-function' exception")
			{
				REQUIRE_THROWS_WITH(
					CalculateCallExpressionType(nonFuncType, args),
					"Type error: Attempt to call a non-function type 'number'.");
			}
		}
	}
}
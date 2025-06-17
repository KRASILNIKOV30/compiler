#pragma once
#include "../ast/expression/Expression.h"
#include "../ast/Type.h"
#include <ranges>

namespace
{
std::string TypeToString(const Type& type)
{
	if (std::holds_alternative<PrimitiveType>(type.type))
	{
		return StringifyPrimitiveType(std::get<PrimitiveType>(type.type));
	}

	std::stringstream ss;
	const auto& funcType = std::get<FunctionType>(type.type);

	if (funcType.empty())
	{
		return "()";
	}
	if (funcType.size() == 1)
	{
		return "() -> " + TypeToString(funcType.front());
	}

	for (size_t i = 0; i < funcType.size() - 1; ++i)
	{
		bool needsParens = std::holds_alternative<FunctionType>(funcType[i].type);
		if (needsParens)
		{
			ss << "(";
		}

		ss << TypeToString(funcType[i]);

		if (needsParens)
		{
			ss << ")";
		}

		ss << " -> ";
	}
	ss << TypeToString(funcType.back());
	return ss.str();
}

Type CalculateFunctionWithoutArgs(const Type& calleeType)
{
	const auto& signature = std::get<FunctionType>(calleeType.type);
	const Type& expectedParamType = signature.front();

	if (std::holds_alternative<PrimitiveType>(expectedParamType.type) && std::get<PrimitiveType>(expectedParamType.type) == PrimitiveType::VOID)
	{
		if (signature.size() == 2)
		{
			return signature.back();
		}
		else // Тип был VOID -> T1 -> T2...
		{
			return FunctionType{ signature.begin() + 1, signature.end() };
		}
	}
	else
	{
		throw std::runtime_error(
			"Type mismatch: Function expects an argument of type '" + TypeToString(expectedParamType) + "' but was called with no arguments.");
	}
}
} // namespace

inline Type CalculateCallExpressionType(const Type& calleeType, const std::vector<ExpressionPtr>& arguments)
{
	Type currentType = calleeType;

	if (!std::holds_alternative<FunctionType>(currentType.type))
	{
		throw std::runtime_error(
			"Type error: Attempt to call a non-function type '" + TypeToString(currentType) + "'.");
	}

	if (arguments.empty())
	{
		return CalculateFunctionWithoutArgs(currentType);
	}

	for (size_t i = 0; i < arguments.size(); ++i)
	{
		const auto& argExpr = arguments[i];

		if (!std::holds_alternative<FunctionType>(currentType.type))
		{
			throw std::runtime_error(
				"Type error: Attempt to call a non-function type '" + TypeToString(currentType) + "'.");
		}

		auto& signature = std::get<FunctionType>(currentType.type);

		if (signature.size() <= 1)
		{
			throw std::runtime_error(
				"Type error: Too many arguments provided. Function of type '" + TypeToString(calleeType) + "' cannot take more than " + std::to_string(i) + " arguments.");
		}

		const Type& expectedParamType = signature.front();
		Type actualArgType = argExpr->GetType();

		if (actualArgType != expectedParamType)
		{
			throw std::runtime_error(
				"Type mismatch in function call: Argument " + std::to_string(i + 1) + " has type '" + TypeToString(actualArgType) + "', but function expects '" + TypeToString(expectedParamType) + "'.");
		}

		FunctionType newSignature{ signature.begin() + 1, signature.end() };

		if (newSignature.size() == 1)
		{
			currentType = newSignature.front();
		}
		else
		{
			currentType = newSignature;
		}
	}

	return currentType;
}

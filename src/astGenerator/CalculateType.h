#pragma once
#include "../ast/expression/Operators.h"
#include "../ast/Type.h"

namespace
{
inline std::string TypeToString(const PrimitiveType& type)
{
	switch (type)
	{
	case PrimitiveType::INT:
		return "INT";
	case PrimitiveType::FLOAT:
		return "FLOAT";
	case PrimitiveType::STRING:
		return "STRING";
	case PrimitiveType::BOOL:
		return "BOOL";
	}
	return "UNKNOWN_TYPE";
}

inline std::string OperatorToString(const BinaryOperators& op)
{
	switch (op)
	{
	case BinaryOperators::PLUS:
		return "+";
	case BinaryOperators::MINUS:
		return "-";
	case BinaryOperators::MUL:
		return "*";
	case BinaryOperators::MOD:
		return "mod";
	case BinaryOperators::DIV:
		return "div";
	case BinaryOperators::DIVISION:
		return "/";
	case BinaryOperators::ASSIGNMENT:
		return "=";
	case BinaryOperators::EQUAL:
		return "==";
	case BinaryOperators::NOT_EQUAL:
		return "!=";
	case BinaryOperators::LESS:
		return "<";
	case BinaryOperators::GREATER:
		return ">";
	case BinaryOperators::LESS_OR_EQUAL:
		return "<=";
	case BinaryOperators::GREATER_OR_EQUAL:
		return ">=";
	case BinaryOperators::AND:
		return "and";
	case BinaryOperators::OR:
		return "or";
	}
	return "UNKNOWN_OPERATOR";
}
} // namespace

inline PrimitiveType CalculateType(const PrimitiveType& left, const PrimitiveType& right, BinaryOperators op)
{
	switch (op)
	{
	case BinaryOperators::PLUS:
		if (left == PrimitiveType::STRING && right == PrimitiveType::STRING)
		{
			return PrimitiveType::STRING;
		}
		[[fallthrough]];
	case BinaryOperators::MINUS:
	case BinaryOperators::MUL:
		if (left == PrimitiveType::INT && right == PrimitiveType::INT)
		{
			return PrimitiveType::INT;
		}
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::FLOAT;
		}
		break;

	case BinaryOperators::DIVISION:
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::FLOAT;
		}
		break;

	case BinaryOperators::DIV:
	case BinaryOperators::MOD:
		if (left == PrimitiveType::INT && right == PrimitiveType::INT)
		{
			return PrimitiveType::INT;
		}
		break;

	case BinaryOperators::AND:
	case BinaryOperators::OR:
		if (left == PrimitiveType::BOOL && right == PrimitiveType::BOOL)
		{
			return PrimitiveType::BOOL;
		}
		break;

	case BinaryOperators::EQUAL:
	case BinaryOperators::NOT_EQUAL:
	case BinaryOperators::LESS:
	case BinaryOperators::GREATER:
	case BinaryOperators::LESS_OR_EQUAL:
	case BinaryOperators::GREATER_OR_EQUAL:
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::BOOL;
		}
		if (left == right && (left == PrimitiveType::STRING || left == PrimitiveType::BOOL))
		{
			return PrimitiveType::BOOL;
		}
		break;

	case BinaryOperators::ASSIGNMENT:
		if (left == right)
		{
			return left;
		}
		if (left == PrimitiveType::FLOAT && right == PrimitiveType::INT)
		{
			return PrimitiveType::FLOAT;
		}
		break;
	}

	throw std::runtime_error("Binary operator '" + OperatorToString(op) + "' is not applicable to types '" + TypeToString(left) + "' and '" + TypeToString(right) + "'.");
}
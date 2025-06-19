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

inline std::string OperatorToString(const BinaryOperator& op)
{
	switch (op)
	{
	case BinaryOperator::PLUS:
		return "+";
	case BinaryOperator::MINUS:
		return "-";
	case BinaryOperator::MUL:
		return "*";
	case BinaryOperator::MOD:
		return "mod";
	case BinaryOperator::DIV:
		return "div";
	case BinaryOperator::DIVISION:
		return "/";
	case BinaryOperator::ASSIGNMENT:
		return "=";
	case BinaryOperator::EQUAL:
		return "==";
	case BinaryOperator::NOT_EQUAL:
		return "!=";
	case BinaryOperator::LESS:
		return "<";
	case BinaryOperator::GREATER:
		return ">";
	case BinaryOperator::LESS_OR_EQUAL:
		return "<=";
	case BinaryOperator::GREATER_OR_EQUAL:
		return ">=";
	case BinaryOperator::AND:
		return "and";
	case BinaryOperator::OR:
		return "or";
	}
	return "UNKNOWN_OPERATOR";
}
} // namespace

inline PrimitiveType CalculateType(const PrimitiveType& left, const PrimitiveType& right, BinaryOperator op)
{
	switch (op)
	{
	case BinaryOperator::PLUS:
		if (left == PrimitiveType::STRING && right == PrimitiveType::STRING)
		{
			return PrimitiveType::STRING;
		}
		[[fallthrough]];
	case BinaryOperator::MINUS:
	case BinaryOperator::MUL:
		if (left == PrimitiveType::INT && right == PrimitiveType::INT)
		{
			return PrimitiveType::INT;
		}
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::FLOAT;
		}
		break;

	case BinaryOperator::DIVISION:
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::FLOAT;
		}
		break;

	case BinaryOperator::DIV:
	case BinaryOperator::MOD:
		if (left == PrimitiveType::INT && right == PrimitiveType::INT)
		{
			return PrimitiveType::INT;
		}
		break;

	case BinaryOperator::AND:
	case BinaryOperator::OR:
		if (left == PrimitiveType::BOOL && right == PrimitiveType::BOOL)
		{
			return PrimitiveType::BOOL;
		}
		break;

	case BinaryOperator::EQUAL:
	case BinaryOperator::NOT_EQUAL:
	case BinaryOperator::LESS:
	case BinaryOperator::GREATER:
	case BinaryOperator::LESS_OR_EQUAL:
	case BinaryOperator::GREATER_OR_EQUAL:
		if ((left == PrimitiveType::INT || left == PrimitiveType::FLOAT) && (right == PrimitiveType::INT || right == PrimitiveType::FLOAT))
		{
			return PrimitiveType::BOOL;
		}
		if (left == right && (left == PrimitiveType::STRING || left == PrimitiveType::BOOL))
		{
			return PrimitiveType::BOOL;
		}
		break;

	case BinaryOperator::ASSIGNMENT:
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
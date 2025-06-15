#pragma once
#include "../ast/expression/Operators.h"
#include <algorithm>
#include <cmath>
#include <string>

inline std::string FormatFloat(double val)
{
	std::string s = std::to_string(val);
	size_t last_not_zero = s.find_last_not_of('0');
	if (std::string::npos != last_not_zero)
	{
		if (s[last_not_zero] == '.')
		{
			s.erase(last_not_zero + 2);
		}
		else
		{
			s.erase(last_not_zero + 1);
		}
	}
	return s;
}

inline std::string Calculate(const std::string& left, const std::string& right,
	PrimitiveType leftType, PrimitiveType rightType,
	BinaryOperators op)
{
	if (leftType == PrimitiveType::STRING)
	{
		switch (op)
		{
		case BinaryOperators::PLUS:
			return left + right;
		case BinaryOperators::EQUAL:
			return (left == right) ? "true" : "false";
		case BinaryOperators::NOT_EQUAL:
			return (left != right) ? "true" : "false";
		case BinaryOperators::LESS:
			return (left < right) ? "true" : "false";
		case BinaryOperators::GREATER:
			return (left > right) ? "true" : "false";
		case BinaryOperators::LESS_OR_EQUAL:
			return (left <= right) ? "true" : "false";
		case BinaryOperators::GREATER_OR_EQUAL:
			return (left >= right) ? "true" : "false";
		default:
			break;
		}
	}

	if (leftType == PrimitiveType::BOOL)
	{
		bool leftBool = (left == "true");
		bool rightBool = (right == "true");
		switch (op)
		{
		case BinaryOperators::AND:
			return (leftBool && rightBool) ? "true" : "false";
		case BinaryOperators::OR:
			return (leftBool || rightBool) ? "true" : "false";
		case BinaryOperators::EQUAL:
			return (leftBool == rightBool) ? "true" : "false";
		case BinaryOperators::NOT_EQUAL:
			return (leftBool != rightBool) ? "true" : "false";
		default:
			break;
		}
	}

	if (leftType == PrimitiveType::FLOAT || rightType == PrimitiveType::FLOAT)
	{
		double leftNum = std::stod(left);
		double rightNum = std::stod(right);

		switch (op)
		{
		case BinaryOperators::PLUS:
			return FormatFloat(leftNum + rightNum);
		case BinaryOperators::MINUS:
			return FormatFloat(leftNum - rightNum);
		case BinaryOperators::MUL:
			return FormatFloat(leftNum * rightNum);
		case BinaryOperators::DIVISION:
			if (rightNum == 0.0)
				throw std::runtime_error("Division by zero");
			return FormatFloat(leftNum / rightNum);

		case BinaryOperators::EQUAL:
			return (leftNum == rightNum) ? "true" : "false";
		case BinaryOperators::NOT_EQUAL:
			return (leftNum != rightNum) ? "true" : "false";
		case BinaryOperators::LESS:
			return (leftNum < rightNum) ? "true" : "false";
		case BinaryOperators::GREATER:
			return (leftNum > rightNum) ? "true" : "false";
		case BinaryOperators::LESS_OR_EQUAL:
			return (leftNum <= rightNum) ? "true" : "false";
		case BinaryOperators::GREATER_OR_EQUAL:
			return (leftNum >= rightNum) ? "true" : "false";
		default:
			break;
		}
	}

	if (leftType == PrimitiveType::INT)
	{
		long long leftNum = std::stoll(left);
		long long rightNum = std::stoll(right);

		switch (op)
		{
		case BinaryOperators::PLUS:
			return std::to_string(leftNum + rightNum);
		case BinaryOperators::MINUS:
			return std::to_string(leftNum - rightNum);
		case BinaryOperators::MUL:
			return std::to_string(leftNum * rightNum);
		case BinaryOperators::DIV:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(leftNum / rightNum);
		case BinaryOperators::MOD:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(leftNum % rightNum);

		case BinaryOperators::EQUAL:
			return (leftNum == rightNum) ? "true" : "false";
		case BinaryOperators::NOT_EQUAL:
			return (leftNum != rightNum) ? "true" : "false";
		case BinaryOperators::LESS:
			return (leftNum < rightNum) ? "true" : "false";
		case BinaryOperators::GREATER:
			return (leftNum > rightNum) ? "true" : "false";
		case BinaryOperators::LESS_OR_EQUAL:
			return (leftNum <= rightNum) ? "true" : "false";
		case BinaryOperators::GREATER_OR_EQUAL:
			return (leftNum >= rightNum) ? "true" : "false";

		default:
			break;
		}
	}
	
	throw std::runtime_error("Unsupported operation for given types in Calculate function.");
}
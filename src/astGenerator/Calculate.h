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
	BinaryOperator op)
{
	if (leftType == PrimitiveType::STRING)
	{
		switch (op)
		{
		case BinaryOperator::PLUS:
			return left + right;
		case BinaryOperator::EQUAL:
			return (left == right) ? "true" : "false";
		case BinaryOperator::NOT_EQUAL:
			return (left != right) ? "true" : "false";
		case BinaryOperator::LESS:
			return (left < right) ? "true" : "false";
		case BinaryOperator::GREATER:
			return (left > right) ? "true" : "false";
		case BinaryOperator::LESS_OR_EQUAL:
			return (left <= right) ? "true" : "false";
		case BinaryOperator::GREATER_OR_EQUAL:
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
		case BinaryOperator::AND:
			return (leftBool && rightBool) ? "true" : "false";
		case BinaryOperator::OR:
			return (leftBool || rightBool) ? "true" : "false";
		case BinaryOperator::EQUAL:
			return (leftBool == rightBool) ? "true" : "false";
		case BinaryOperator::NOT_EQUAL:
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
		case BinaryOperator::PLUS:
			return FormatFloat(leftNum + rightNum);
		case BinaryOperator::MINUS:
			return FormatFloat(leftNum - rightNum);
		case BinaryOperator::MUL:
			return FormatFloat(leftNum * rightNum);
		case BinaryOperator::DIVISION:
			if (rightNum == 0.0)
				throw std::runtime_error("Division by zero");
			return FormatFloat(leftNum / rightNum);

		case BinaryOperator::EQUAL:
			return (leftNum == rightNum) ? "true" : "false";
		case BinaryOperator::NOT_EQUAL:
			return (leftNum != rightNum) ? "true" : "false";
		case BinaryOperator::LESS:
			return (leftNum < rightNum) ? "true" : "false";
		case BinaryOperator::GREATER:
			return (leftNum > rightNum) ? "true" : "false";
		case BinaryOperator::LESS_OR_EQUAL:
			return (leftNum <= rightNum) ? "true" : "false";
		case BinaryOperator::GREATER_OR_EQUAL:
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
		case BinaryOperator::PLUS:
			return std::to_string(leftNum + rightNum);
		case BinaryOperator::MINUS:
			return std::to_string(leftNum - rightNum);
		case BinaryOperator::MUL:
			return std::to_string(leftNum * rightNum);
		case BinaryOperator::DIV:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(leftNum / rightNum);
		case BinaryOperator::MOD:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(leftNum % rightNum);

		case BinaryOperator::EQUAL:
			return (leftNum == rightNum) ? "true" : "false";
		case BinaryOperator::NOT_EQUAL:
			return (leftNum != rightNum) ? "true" : "false";
		case BinaryOperator::LESS:
			return (leftNum < rightNum) ? "true" : "false";
		case BinaryOperator::GREATER:
			return (leftNum > rightNum) ? "true" : "false";
		case BinaryOperator::LESS_OR_EQUAL:
			return (leftNum <= rightNum) ? "true" : "false";
		case BinaryOperator::GREATER_OR_EQUAL:
			return (leftNum >= rightNum) ? "true" : "false";

		default:
			break;
		}
	}
	
	throw std::runtime_error("Unsupported operation for given types in Calculate function.");
}
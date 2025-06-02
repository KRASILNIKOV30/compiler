#pragma once
#include "../ast/expression/Operators.h"
#include <algorithm>
#include <cmath>
#include <string>

inline std::string Calculate(std::string const& left, std::string const& right, BinaryOperators op)
{
	try
	{
		double leftNum = std::stod(left);
		double rightNum = std::stod(right);

		switch (op)
		{
		case BinaryOperators::PLUS:
			return std::to_string(leftNum + rightNum);
		case BinaryOperators::MINUS:
			return std::to_string(leftNum - rightNum);
		case BinaryOperators::MUL:
			return std::to_string(leftNum * rightNum);
		case BinaryOperators::DIVISION:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(leftNum / rightNum);
		case BinaryOperators::MOD:
			return std::to_string(std::fmod(leftNum, rightNum));
		case BinaryOperators::DIV:
			if (rightNum == 0)
			{
				throw std::runtime_error("Division by zero");
			}
			return std::to_string(std::floor(leftNum / rightNum));
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
		case BinaryOperators::AND:
			return (leftNum != 0 && rightNum != 0) ? "true" : "false";
		case BinaryOperators::OR:
			return (leftNum != 0 || rightNum != 0) ? "true" : "false";
		default:
			throw std::runtime_error("Unknown operator");
		}
	}
	catch (const std::invalid_argument&)
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
			throw std::runtime_error("Unsupported string operation");
		}
	}
}
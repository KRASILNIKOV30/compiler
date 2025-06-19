#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

enum class UnaryOperators
{
	PLUS,
	MINUS,
	NOT
};

enum class BinaryOperator
{
	PLUS, // +
	MINUS, // -
	MUL, // *
	MOD, // mod
	DIV, // div
	DIVISION, // /
	ASSIGNMENT, // =
	EQUAL, // ==
	NOT_EQUAL, // !=
	LESS, // <
	GREATER, // >
	LESS_OR_EQUAL, // <=
	GREATER_OR_EQUAL, // >=
	AND, // and
	OR, // or
};

inline std::unordered_map<BinaryOperator, std::vector<std::string>> BinaryOperatorsToString = {
	{ BinaryOperator::PLUS, { "add" } },
	{ BinaryOperator::MINUS, { "sub" } },
	{ BinaryOperator::MUL, { "mul" } },
	{ BinaryOperator::MOD, { "mod" } },
	{ BinaryOperator::DIV, { "div" } },
	{ BinaryOperator::DIVISION, { "div" } },
	{ BinaryOperator::ASSIGNMENT, { "not found" } },
	{ BinaryOperator::EQUAL, { "ceq" } },
	{ BinaryOperator::NOT_EQUAL, { "ceq", "not" } },
	{ BinaryOperator::LESS, { "clt" } },
	{ BinaryOperator::GREATER, { "cgt" } },
	{ BinaryOperator::LESS_OR_EQUAL, { "cgt", "not" } },
	{ BinaryOperator::GREATER_OR_EQUAL, { "clt", "not" } },
	{ BinaryOperator::AND, { "and" } },
	{ BinaryOperator::OR, { "or" } },
};

inline std::unordered_map<std::string, BinaryOperator> StringToBinaryOperatorMap = {
	{ "+", BinaryOperator::PLUS },
	{ "-", BinaryOperator::MINUS },
	{ "*", BinaryOperator::MUL },
	{ "mod", BinaryOperator::MOD },
	{ "div", BinaryOperator::DIV },
	{ "/", BinaryOperator::DIVISION },
	{ "=", BinaryOperator::ASSIGNMENT },
	{ "==", BinaryOperator::EQUAL },
	{ "!=", BinaryOperator::NOT_EQUAL },
	{ "<", BinaryOperator::LESS },
	{ ">", BinaryOperator::GREATER },
	{ "<=", BinaryOperator::LESS_OR_EQUAL },
	{ ">=", BinaryOperator::GREATER_OR_EQUAL },
	{ "and", BinaryOperator::AND },
	{ "or", BinaryOperator::OR },
};

inline BinaryOperator StringToBinaryOperator(std::string const& str)
{
	const auto it = StringToBinaryOperatorMap.find(str);
	if (it != StringToBinaryOperatorMap.end())
	{
		return it->second;
	}
	throw std::runtime_error("unknown binary operator " + str);
}
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

enum class BinaryOperators
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

inline std::unordered_map<BinaryOperators, std::vector<std::string>> BinaryOperatorsToString = {
	{ BinaryOperators::PLUS, { "add" } },
	{ BinaryOperators::MINUS, { "sub" } },
	{ BinaryOperators::MUL, { "mul" } },
	{ BinaryOperators::MOD, { "mod" } },
	{ BinaryOperators::DIV, { "div" } },
	{ BinaryOperators::DIVISION, { "div" } },
	{ BinaryOperators::ASSIGNMENT, { "not found" } },
	{ BinaryOperators::EQUAL, { "ceq" } },
	{ BinaryOperators::NOT_EQUAL, { "ceq", "not" } },
	{ BinaryOperators::LESS, { "clt" } },
	{ BinaryOperators::GREATER, { "cgt" } },
	{ BinaryOperators::LESS_OR_EQUAL, { "cgt", "not" } },
	{ BinaryOperators::GREATER_OR_EQUAL, { "clt", "not" } },
	{ BinaryOperators::AND, { "and" } },
	{ BinaryOperators::OR, { "or" } },
};

inline std::unordered_map<std::string, BinaryOperators> StringToBinaryOperatorMap = {
	{ "+", BinaryOperators::PLUS },
	{ "-", BinaryOperators::MINUS },
	{ "*", BinaryOperators::MUL },
	{ "mod", BinaryOperators::MOD },
	{ "div", BinaryOperators::DIV },
	{ "/", BinaryOperators::DIVISION },
	{ "=", BinaryOperators::ASSIGNMENT },
	{ "==", BinaryOperators::EQUAL },
	{ "!=", BinaryOperators::NOT_EQUAL },
	{ "<", BinaryOperators::LESS },
	{ ">", BinaryOperators::GREATER },
	{ "<=", BinaryOperators::LESS_OR_EQUAL },
	{ ">=", BinaryOperators::GREATER_OR_EQUAL },
	{ "and", BinaryOperators::AND },
	{ "or", BinaryOperators::OR },
};

inline BinaryOperators StringToBinaryOperator(std::string const& str)
{
	const auto it = StringToBinaryOperatorMap.find(str);
	if (it != StringToBinaryOperatorMap.end())
	{
		return it->second;
	}
	throw std::runtime_error("unknown binary operator " + str);
}
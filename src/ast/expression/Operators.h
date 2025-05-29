#pragma once
#include <unordered_map>

enum class UnaryOperators
{
	PLUS, MINUS, NOT
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

inline std::unordered_map<BinaryOperators, std::string> BinaryOperatorsToString = {
	{BinaryOperators::PLUS, "add"},
	{BinaryOperators::MINUS, "sub"},
	{BinaryOperators::MUL, "mul"},
	{BinaryOperators::MOD, "div"},
	{BinaryOperators::DIV, "div"},
	{BinaryOperators::DIVISION, "div"},
	{BinaryOperators::ASSIGNMENT, "not found"},
	{BinaryOperators::EQUAL, "ceq"},
	{BinaryOperators::NOT_EQUAL, "not found"},
	{BinaryOperators::LESS, "not found"},
	{BinaryOperators::GREATER, "not found"},
	{BinaryOperators::LESS_OR_EQUAL, "not found"},
	{BinaryOperators::GREATER_OR_EQUAL, "not found"},
	{BinaryOperators::AND, "not found"},
	{BinaryOperators::OR, "not found"},
};
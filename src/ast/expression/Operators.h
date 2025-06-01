#pragma once
#include <unordered_map>
#include <string>
#include <vector>

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

inline std::unordered_map<BinaryOperators, std::vector<std::string>> BinaryOperatorsToString = {
	{BinaryOperators::PLUS, { "add" } },
	{BinaryOperators::MINUS, { "sub" } },
	{BinaryOperators::MUL, { "mul" } },
	{BinaryOperators::MOD, { "div" } },
	{BinaryOperators::DIV, { "div" } },
	{BinaryOperators::DIVISION, { "div" } },
	{BinaryOperators::ASSIGNMENT, { "not found" } },
	{BinaryOperators::EQUAL, { "ceq" } },
	{BinaryOperators::NOT_EQUAL, { "ceq", "not" } },
	{BinaryOperators::LESS, { "clt" } },
	{BinaryOperators::GREATER, { "cgt" } },
	{BinaryOperators::LESS_OR_EQUAL, { "cgt", "not" } },
	{BinaryOperators::GREATER_OR_EQUAL, { "clt", "not" } },
	{BinaryOperators::AND, { "and" } },
	{BinaryOperators::OR, { "or" } },
};
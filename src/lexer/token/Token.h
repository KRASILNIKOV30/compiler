#pragma once
#include <string>

enum class TokenType
{
	ERROR,
	PARAN_OPEN, // (
	PARAN_CLOSE, // )
	CURLY_OPEN, // {
	CURLY_CLOSE, // }
	BRACKET_OPEN, // [
	BRACKET_CLOSE, // ]
	COMMA, // ,
	OP_PLUS, // +
	OP_MINUS, // -
	OP_MUL, // *
	OP_MOD, // mod
	OP_DIV, // div
	OP_DIVISION, // /
	OP_ASSIGNMENT, // =
	OP_EQUAL, // ==
	OP_NOT_EQUAL, // !=
	OP_LESS, // <
	OP_GREATER, // >
	OP_LESS_OR_EQUAL, // <=
	OP_GREATER_OR_EQUAL, // >=
	OP_AND, // and
	OP_OR, // or
	OP_NOT, // not
	ID, // identifier
	INTEGER, // integer
	FLOAT, // float
	STRING_LITERAL, // string literal
	TRUE, // true
	FALSE, // false
};

enum class Error
{
	NONE,
	FLOAT_INVALID,
	STRING_LITERAL_INCOMPLETE,
	EMPTY_INPUT,
	INVALID_ID,
};

struct Token
{
	TokenType type;
	std::string value;
	size_t pos;
	Error error = Error::NONE;
};

inline bool operator==(Token const& left, Token const& right)
{
	return left.type == right.type && left.value == right.value && left.pos == right.pos && left.error == right.error;
}


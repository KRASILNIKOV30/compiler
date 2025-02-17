#pragma once
#include "../../lexer/token/Token.h"
#include <unordered_set>

const std::unordered_set REL_OPS = {
	TokenType::OP_EQUAL,
	TokenType::OP_NOT_EQUAL,
	TokenType::OP_GREATER,
	TokenType::OP_LESS,
	TokenType::OP_GREATER_OR_EQUAL,
	TokenType::OP_LESS_OR_EQUAL,
};

const std::unordered_set LOW_PRIORITY_OPS = {
	TokenType::OP_PLUS,
	TokenType::OP_MINUS,
	TokenType::OP_OR,
};

const std::unordered_set HIGH_PRIORITY_OPS = {
	TokenType::OP_MUL,
	TokenType::OP_DIVISION,
	TokenType::OP_DIV,
	TokenType::OP_MOD,
	TokenType::OP_AND,
};

inline bool IsRelOp(Token const& token)
{
	return REL_OPS.contains(token.type);
}

inline bool IsLowPriorityOp(Token const& token)
{
	return LOW_PRIORITY_OPS.contains(token.type);
}

inline bool IsHighPriorityOp(Token const& token)
{
	return HIGH_PRIORITY_OPS.contains(token.type);
}
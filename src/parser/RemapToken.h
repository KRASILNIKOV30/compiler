#pragma once
#include "../lexer/token/Token.h"
#include <string>
#include <unordered_map>

inline std::unordered_map<TokenType, std::string> TokenTypeToString = {
	{ TokenType::ERROR, "error" },
	{ TokenType::PARAN_OPEN, "(" },
	{ TokenType::PARAN_CLOSE, ")" },
	{ TokenType::CURLY_OPEN, "{" },
	{ TokenType::CURLY_CLOSE, "}" },
	{ TokenType::BRACKET_OPEN, "[" },
	{ TokenType::BRACKET_CLOSE, "]" },
	{ TokenType::COMMA, "," },
	{ TokenType::OP_PLUS, "+" },
	{ TokenType::OP_MINUS, "-" },
	{ TokenType::OP_MUL, "*" },
	{ TokenType::OP_MOD, "mod" },
	{ TokenType::OP_DIV, "div" },
	{ TokenType::OP_DIVISION, "/" },
	{ TokenType::OP_ASSIGNMENT, "=" },
	{ TokenType::OP_EQUAL, "==" },
	{ TokenType::OP_NOT_EQUAL, "!=" },
	{ TokenType::OP_LESS, "<" },
	{ TokenType::OP_GREATER, ">" },
	{ TokenType::OP_LESS_OR_EQUAL, "<=" },
	{ TokenType::OP_GREATER_OR_EQUAL, ">=" },
	{ TokenType::OP_AND, "and" },
	{ TokenType::OP_OR, "or" },
	{ TokenType::OP_NOT, "not" },
	{ TokenType::OP_NOT_MARK, "!" },
	{ TokenType::ID, "id" },
	{ TokenType::INTEGER, "int" },
	{ TokenType::FLOAT, "float" },
	{ TokenType::STRING_LITERAL, "string" },
	{ TokenType::TRUE, "true" },
	{ TokenType::FALSE, "false" },
	{ TokenType::IF, "if" },
	{ TokenType::SEMICOLON, ";" },
	{ TokenType::COLON, ":" },
	{ TokenType::THEN, "then" },
	{ TokenType::ELSE, "else" },
	{ TokenType::CONST, "const" },
	{ TokenType::LET, "let" },
	{ TokenType::FUNCTION, "function" },
	{ TokenType::WHILE, "while" },
	{ TokenType::RETURN, "return" },
};

inline std::string RemapTokenTypeToString(const TokenType type)
{
	return TokenTypeToString.at(type);
}

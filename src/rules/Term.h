#pragma once
#include "Expression.h"
#include "../lexer/Lexer.h"

/**
 * term -> (expression) | + term | - term | not term | ! term | ident | number | true | false | string
 */
inline bool Term(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return false;
	}

	const auto tokenType = lexer.Get().type;

	if (tokenType == TokenType::PARAN_OPEN)
	{
		return Expression(lexer) && !lexer.Empty() && lexer.Get().type == TokenType::PARAN_CLOSE;
	}

	if (tokenType == TokenType::OP_PLUS
		|| tokenType == TokenType::OP_MINUS
		|| tokenType == TokenType::OP_NOT
		|| tokenType == TokenType::OP_NOT_MARK)
	{
		return Term(lexer);
	}

	if (tokenType == TokenType::ID
		|| tokenType == TokenType::INTEGER
		|| tokenType == TokenType::FLOAT
		|| tokenType == TokenType::TRUE
		|| tokenType == TokenType::FALSE
		|| tokenType == TokenType::STRING_LITERAL)
	{
		return true;
	}

	return false;
}
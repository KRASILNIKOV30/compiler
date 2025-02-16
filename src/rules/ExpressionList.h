#pragma once
#include "Expression.h"

/**
* expListRemainder -> e | , expression expListRemainder
 */
inline bool ExpressionListRemainder(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return true;
	}

	if (lexer.Peek().type == TokenType::COMMA)
	{
		return Expression(lexer) && ExpressionListRemainder(lexer);
	}

	return true;
}

/**
 * expList -> expression | expListRemainder
 * expListRemainder -> e | , expression expListRemainder
 */
inline bool ExpressionList(Lexer& lexer)
{
	return Expression(lexer) && ExpressionListRemainder(lexer);
}
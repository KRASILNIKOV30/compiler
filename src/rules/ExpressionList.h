#pragma once
#include "Expression.h"

inline bool Expression(Lexer& lexer);

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
		lexer.Get();
		return Expression(lexer) && ExpressionListRemainder(lexer);
	}

	return true;
}

/**
 * expList -> e | expression expListRemainder
 * expListRemainder -> e | , expression expListRemainder
 */
inline bool ExpressionList(Lexer& lexer)
{
	return Expression(lexer) && ExpressionListRemainder(lexer);
}
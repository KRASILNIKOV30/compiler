#include "../Parser.h"

/**
* expListRemainder -> e | , expression expListRemainder
 */
bool Parser::ExpressionListRem()
{
	if (Empty())
	{
		return true;
	}

	if (Peek().type == TokenType::COMMA)
	{
		Get();
		return Expression() && ExpressionListRem();
	}

	return true;
}

/**
 * expList -> e | expression expListRemainder
 * expListRemainder -> e | , expression expListRemainder
 */
bool Parser::ExpressionList()
{
	return Expression() && ExpressionListRem();
}
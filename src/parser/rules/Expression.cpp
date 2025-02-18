#include "Operations.h"
#include "../Parser.h"

/**
 * expressionRem -> e | relOp simexp expressionRem
 */
bool Parser::ExpressionRem()
{
	if (Empty())
	{
		return true;
	}

	if (IsRelOp(m_lexer.Peek()))
	{
		Get();
		return (SimExp() && ExpressionRem()) || Panic(Error::INVALID_EXP);
	}

	return true;
}

/**
 * expression -> simexp expressionRem
 * expressionRem -> e | relOp simexp expressionRem
 */
bool Parser::Expression()
{
	if (Empty())
	{
		return Panic(Error::EMPTY_INPUT);
	}

	return (SimExp() && ExpressionRem()) || Panic(Error::INVALID_EXP);
}
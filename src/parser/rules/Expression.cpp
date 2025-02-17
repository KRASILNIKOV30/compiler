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
		return SimExp() && ExpressionRem();
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
		return false;
	}

	return SimExp() && ExpressionRem();
}
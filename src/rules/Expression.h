#pragma once
#include "Operations.h"
#include "SimExp.h"
#include "../lexer/Lexer.h"

/**
 * expressionRem -> e | relOp simexp expressionRem
 */
inline bool ExpressionRem(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return true;
	}

	if (IsRelOp(lexer.Peek()))
	{
		lexer.Get();
		return SimExp(lexer) && ExpressionRem(lexer);
	}

	return true;
}

/**
 * expression -> simexp expressionRem
 * expressionRem -> e | relOp simexp expressionRem
 */
inline bool Expression(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return false;
	}

	return SimExp(lexer) && ExpressionRem(lexer);
}
#pragma once
#include "Operations.h"
#include "Term.h"
#include "../lexer/Lexer.h"

/**
 * simtermRem -> e | highPriorityOp term simtermRem
 */
inline bool SimTermRem(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return true;
	}

	if (IsHighPriorityOp(lexer.Peek()))
	{
		lexer.Get();
		return Term(lexer) && SimTermRem(lexer);
	}

	return true;
}

/**
 * simterm -> term simtermRem
 * simtermRem -> e | highPriorityOp term simtermRem
 */
inline bool SimTerm(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return false;
	}

	return Term(lexer) && SimTermRem(lexer);
}
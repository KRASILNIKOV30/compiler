#pragma once
#include "Operations.h"
#include "SimTerm.h"
#include "../lexer/Lexer.h"

/**
 * simexpRem -> e | lowPriorityOp simterm simexpRem
 */
inline bool SimExpRem(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return true;
	}

	if (IsLowPriorityOp(lexer.Peek()))
	{
		lexer.Get();
		return SimTerm(lexer) && SimExpRem(lexer);
	}

	return true;
}

/**
 * simexp -> simterm simexpRem
 * simexpRem -> e | lowPriorityOp simterm simexpRem
 */
inline bool SimExp(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return false;
	}

	return SimTerm(lexer) && SimExpRem(lexer);
}
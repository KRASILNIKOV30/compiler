#include "Operations.h"
#include "../Parser.h"

/**
 * simtermRem -> e | highPriorityOp term simtermRem
 */
bool Parser::SimTermRem()
{
	if (Empty())
	{
		return true;
	}

	if (IsHighPriorityOp(m_lexer.Peek()))
	{
		m_lexer.Get();
		return Term() && SimTermRem();
	}

	return true;
}

/**
 * simterm -> term simtermRem
 * simtermRem -> e | highPriorityOp term simtermRem
 */
bool Parser::SimTerm()
{
	if (Empty())
	{
		return false;
	}

	return Term() && SimTermRem();
}
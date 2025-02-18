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
		return (Term() && SimTermRem()) || Panic(Error::TERM_EXPECTED);
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
		return Panic(Error::EMPTY_INPUT);
	}

	return (Term() && SimTermRem()) || Panic(Error::TERM_EXPECTED);
}
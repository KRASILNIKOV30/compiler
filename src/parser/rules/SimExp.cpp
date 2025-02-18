#include "Operations.h"
#include "../Parser.h"

/**
 * simexpRem -> e | lowPriorityOp simterm simexpRem
 */
bool Parser::SimExpRem()
{
	if (Empty())
	{
		return true;
	}

	if (IsLowPriorityOp(m_lexer.Peek()))
	{
		Get();
		return SimTerm() && SimExpRem();
	}

	return true;
}

/**
 * simexp -> simterm simexpRem
 * simexpRem -> e | lowPriorityOp simterm simexpRem
 */
bool Parser::SimExp()
{
	if (Empty())
	{
		return Panic(Error::EMPTY_INPUT);
	}

	return SimTerm() && SimExpRem();
}
#pragma once
#include "../Reader.h"

/**
 * expList -> expression | expListRemainder
 * expListRemainder -> e | , expListRemainder
 */
inline bool ExpressionList(Reader& reader)
{
	return false;
}

inline bool Expression(Reader& reader)
{
	return false;
}
#pragma once
#include "../Reader.h"

/**
 * id -> letter idRemainder
 * idRemainder -> e | idPart idRemainder
 */

/**
 * idPart -> letter | digit
 */
inline bool IdPart(Reader& reader)
{
	if (char c; input.get(c))
	{
		return std::isalpha(c) || std::isdigit(c);
	}

	return false;
}

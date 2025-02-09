#pragma once
#include "Expression.h"

#include <iostream>
#include "../Reader.h"

/**
 * idPart -> letter | digit
 */
inline bool IdPart(Reader& reader)
{
	const char ch = reader.Get();
	return std::isalpha(ch) || std::isdigit(ch);
}

/**
 * idRemainder -> e | idPart idRemainder
 */
inline bool IdRemainder(Reader& reader)
{
	if (reader.Empty())
	{
		return true;
	}

	const auto count = reader.Count();
	if (!IdPart(reader))
	{
		reader.Seek(count);
		return true;
	}

	return IdRemainder(reader);
}

/**
 * id -> letter idRemainder
 */
inline bool Id(Reader& reader)
{
	if (reader.Empty())
	{
		return false;
	}

	if (!std::isalpha(reader.Peek()))
	{
		return false;
	}

	return IdPart(reader) && IdRemainder(reader);
}

inline bool ComplexId(Reader& reader);

/**
 * complexIdRemainder -> e | .complexId
 */
inline bool ComplexIdRemainder(Reader& reader)
{
	if (reader.Empty() || reader.Peek() != '.')
	{
		return true;
	}

	reader.Get();
	return ComplexId(reader);
}

/**
 * complexId -> id complexIdRemainder
 */
inline bool ComplexId(Reader& reader)
{
	return Id(reader) && ComplexIdRemainder(reader);
}

/**
 * index -> e | [expression]
 */
inline bool Index(Reader& reader)
{
	if (reader.Empty() || reader.Peek() != '[')
	{
		return true;
	}
	reader.Get();

	if (!Expression(reader))
	{
		return false;
	}

	if (reader.Peek() != ']')
	{
		return false;
	}
	reader.Get();

	return true;
}

/**
 * idIndex -> complexId index
 */
inline bool IdIndex(Reader& reader)
{
	return ComplexId(reader) && Index(reader);
}

/**
 * ident -> idIndex
 * idIndex -> complexId index
 * index -> e | [expression]
 * complexId -> id complexIdRemainder
 * complexIdRemainder -> e | .complexId
 * id -> letter idRemainder
 * idRemainder -> e | idPart idRemainder
 * idPart -> letter | digit
 */
inline bool Ident(Reader& reader)
{
	return IdIndex(reader);
}
#pragma once
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


#pragma once
#include "Expression.h"

#include <iostream>
#include "../Reader.h"

bool Expression(Reader& reader);
bool ExpressionList(Reader& reader);

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
 * index -> [expression]
 */
inline bool Index(Reader& reader)
{
	if (reader.Peek() != '[')
	{
		return false;
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
 * funcCall -> (expList)
 */
inline bool FuncCall(Reader& reader)
{
	if (reader.Peek() != '(')
	{
		return false;
	}
	reader.Get();

	if (!ExpressionList(reader))
	{
		return false;
	}

	if (reader.Peek() != ')')
	{
		return false;
	}
	reader.Get();

	return true;
}

/**
* complexIdRemainder -> e | .complexId | index complexIdRemainder | funcCall complexIdRemainder
 */
inline bool ComplexIdRemainder(Reader& reader)
{
	if (reader.Empty())
	{
		return true;
	}

	if (reader.Peek() == '.')
	{
		reader.Get();
		return ComplexId(reader);
	}

	if (reader.Peek() == '[')
	{
		return Index(reader) && ComplexIdRemainder(reader);
	}

	if (reader.Peek() == '(')
	{
		return FuncCall(reader) && ComplexIdRemainder(reader);
	}

	return true;
}

/**
 * complexId -> id complexIdRemainder
 */
inline bool ComplexId(Reader& reader)
{
	return Id(reader) && ComplexIdRemainder(reader);
}

/**
 * ident -> complexId
 * complexId -> id complexIdRemainder
 * complexIdRemainder -> e | .complexId | index complexIdRemainder | funcCall complexIdRemainder
 * index -> [expression]
 * funcCall -> (expList)
 * id -> letter idRemainder
 * idRemainder -> e | idPart idRemainder
 * idPart -> letter | digit
 */
inline bool Ident(Reader& reader)
{
	return ComplexId(reader);
}
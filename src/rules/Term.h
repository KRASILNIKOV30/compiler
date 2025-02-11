#pragma once
#include <string>
#include "../Reader.h"
#include "Ident.h"
#include "Expression.h"

inline bool Num(Reader& reader)
{
	auto startPos = reader.Count();
	bool foundDigit = false;

	while (!reader.Empty() && std::isdigit(reader.Peek()))
	{
		foundDigit = true;
		reader.Get();
	}

	if (!foundDigit)
	{
		reader.Seek(startPos);
		return false;
	}

	return true;
}

inline bool LitConst(Reader& reader)
{
	auto startPos = reader.Count();
	if (reader.Empty())
	{
		return false;
	}

	if (reader.Peek() != '"')
	{
		return false;
	}

	reader.Get();

	while (!reader.Empty() && reader.Peek() != '"')
	{
		reader.Get();
	}

	if (reader.Empty())
	{
		reader.Seek(startPos);
		return false;
	}

	reader.Get();
	return true;
}

inline bool ParseKeyword(Reader& reader, const std::string& keyword)
{
	auto startPos = reader.Count();
	for (char k : keyword)
	{
		if (reader.Empty() || reader.Peek() != k)
		{
			reader.Seek(startPos);
			return false;
		}
		reader.Get();
	}
	return true;
}

/**
 * term -> + term 
 *       | - term 
 *       | ( expression ) 
 *       | not term 
 *       | ident 
 *       | num 
 *       | true 
 *       | false 
 *       | litconst
 */
inline bool Term(Reader& reader)
{
	auto pos = reader.Count();

	if (!reader.Empty())
	{
		char c = reader.Peek();
		if (c == '+' || c == '-')
		{
			reader.Get();

			if (Term(reader))
			{
				return true;
			}
			reader.Seek(pos);
		}
	}

	if (!reader.Empty() && reader.Peek() == '(')
	{
		reader.Get();
		if (!Expression(reader))
		{
			reader.Seek(pos);
			return false;
		}
		if (reader.Empty() || reader.Peek() != ')')
		{
			reader.Seek(pos);
			return false;
		}
		reader.Get();
		return true;
	}

	if (ParseKeyword(reader, "not"))
	{
		if (Term(reader))
		{
			return true;
		}
		reader.Seek(pos);
		return false;
	}

	if (Ident(reader))
	{
		return true;
	}
	reader.Seek(pos);

	if (Num(reader))
	{
		return true;
	}
	reader.Seek(pos);

	if (ParseKeyword(reader, "true"))
	{
		return true;
	}
	reader.Seek(pos);

	if (ParseKeyword(reader, "false"))
	{
		return true;
	}
	reader.Seek(pos);

	if (LitConst(reader))
	{
		return true;
	}
	reader.Seek(pos);

	return false;
}

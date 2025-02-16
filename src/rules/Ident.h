#pragma once
#include "ExpressionList.h"
#include "../lexer/Lexer.h"

/**
 * idRem -> e | [expression] idRem | (expList) idRem | () idRem
 */
inline bool IdRem(Lexer& lexer)
{
	if (lexer.Empty())
	{
		return true;
	}

	const auto nextTokenType = lexer.Peek().type;

	if (nextTokenType == TokenType::BRACKET_OPEN)
	{
		lexer.Get();
		return Expression(lexer) && lexer.Get().type == TokenType::BRACKET_CLOSE && IdRem(lexer);
	}

	if (nextTokenType == TokenType::PARAN_OPEN)
	{
		lexer.Get();
		if (lexer.Peek().type == TokenType::PARAN_CLOSE)
		{
			lexer.Get();
			return IdRem(lexer);
		}
		return ExpressionList(lexer) && lexer.Get().type == TokenType::PARAN_CLOSE && IdRem(lexer);
	}

	return true;
}

inline bool Id(Lexer& lexer)
{
	return !lexer.Empty() && lexer.Get().type == TokenType::ID;
}

/**
 * ident -> id idRem
 * idRem -> e | [expression] idRem | (expList) idRem
 */
inline bool Ident(Lexer& lexer)
{
	return Id(lexer) && IdRem(lexer);
}
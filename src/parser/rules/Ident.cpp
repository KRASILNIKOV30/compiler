#include "../Parser.h"

/**
 * idRem -> e | [expression] idRem | (expList) idRem | () idRem
 */
bool Parser::IdRem()
{
	if (Empty())
	{
		return true;
	}

	const auto nextTokenType = Peek().type;

	if (nextTokenType == TokenType::BRACKET_OPEN)
	{
		Get();
		return Expression() && Get().type == TokenType::BRACKET_CLOSE && IdRem();
	}

	if (nextTokenType == TokenType::PARAN_OPEN)
	{
		Get();
		if (Peek().type == TokenType::PARAN_CLOSE)
		{
			Get();
			return IdRem();
		}
		return ExpressionList() && Get().type == TokenType::PARAN_CLOSE && IdRem();
	}

	return true;
}

bool Parser::Id()
{
	return !Empty() && Get().type == TokenType::ID;
}

/**
 * ident -> id idRem
 * idRem -> e | [expression] idRem | (expList) idRem
 */
bool Parser::Ident()
{
	return Id() && IdRem();
}
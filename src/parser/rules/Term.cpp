#include "../Parser.h"

/**
 * term -> (expression) | + term | - term | not term | ! term | ident | number | true | false | string
 */
bool Parser::Term()
{
	if (Empty())
	{
		return Panic(Error::EMPTY_INPUT);
	}

	if (Peek().type == TokenType::ID)
	{
		return Ident();
	}

	const auto tokenType = Get().type;

	if (tokenType == TokenType::PARAN_OPEN)
	{
		return Expression() && ((!Empty() && Get().type == TokenType::PARAN_CLOSE) || Panic(Error::PARAN_CLOSE_EXPECTED));
	}

	if (tokenType == TokenType::OP_PLUS
		|| tokenType == TokenType::OP_MINUS
		|| tokenType == TokenType::OP_NOT
		|| tokenType == TokenType::OP_NOT_MARK)
	{
		return Term() || Panic(Error::TERM_EXPECTED);
	}

	if (tokenType == TokenType::INTEGER
		|| tokenType == TokenType::FLOAT
		|| tokenType == TokenType::TRUE
		|| tokenType == TokenType::FALSE
		|| tokenType == TokenType::STRING_LITERAL)
	{
		return true;
	}

	return Panic(Error::TERM_EXPECTED);;
}
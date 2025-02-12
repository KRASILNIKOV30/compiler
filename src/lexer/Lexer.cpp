#include "Lexer.h"
#include "rules/Id.h"

Lexer::Lexer(std::string const& input)
	: m_reader(input)
{
}

Token Lexer::Get()
{
	SkipWhitespaces();
	if (Empty())
	{
		return Token{
			.type = TokenType::ERROR,
			.error = Error::EMPTY_INPUT,
		};
	}

	return Id();
}

bool Lexer::Empty()
{
	return m_reader.Empty();
}

/**
 * id -> simpleId idRemainder
 * idRemainder -> e | .id
 * simpleId -> char simpleIdRemainder
 * simpleIdRemainder -> e | idChar simpleIdRemainder
 * idChar -> char | digit
 * char -> _ | $ | letter
 */
Token Lexer::Id()
{
	const auto startPos = m_reader.Count();
	m_reader.Record();
	if (!IdRule(m_reader))
	{
		return Token{
			.type = TokenType::ERROR,
			.error = Error::INVALID_ID,
		};
	}

	const auto value = m_reader.StopRecord();
	return Token{
		.type = TokenType::ID,
		.value = m_reader.StopRecord(),
		.pos = startPos,
	};
}

void Lexer::SkipWhitespaces()
{
	while (!m_reader.Empty() && m_reader.Peek() == ' ')
	{
		m_reader.Get();
	}
}

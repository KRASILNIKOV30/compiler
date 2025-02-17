#include "Parser.h"

bool Parser::Parse()
{
	return Expression() && Empty();
}

Error Parser::GetError() const
{
	return m_error;
}

std::optional<Token> Parser::GetLastToken() const
{
	return m_token;
}

Token Parser::Get()
{
	const auto token = m_lexer.Get();
	RecordToken(token);
	return token;
}

Token Parser::Peek()
{
	const auto token = m_lexer.Peek();
	RecordToken(token);
	return token;
}

void Parser::RecordToken(Token const& token)
{
	if (token.type == TokenType::ERROR)
	{
		m_error = token.error;
	}
	m_token = token;
}

bool Parser::Empty()
{
	return m_lexer.Empty();
}

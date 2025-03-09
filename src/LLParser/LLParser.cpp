#include "LLParser.h"
#include <utility>

LLParser::LLParser(Table table)
	: m_table(std::move(table))
{
}

Error LLParser::Parse(std::string const& input)
{
	m_lexer = Lexer{ input };

	return Error::NONE;
}

std::optional<Token> LLParser::GetLastToken() const
{
	return m_lastToken;
}

Token LLParser::Get()
{
	const auto token = m_lexer.Get();
	RecordToken(token);
	return token;
}

Token LLParser::Peek()
{
	const auto token = m_lexer.Peek();
	RecordToken(token);
	return token;
}

bool LLParser::Empty()
{
	return m_lexer.Empty();
}

void LLParser::RecordToken(Token const& token)
{
	m_lastToken = token;
}
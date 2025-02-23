#include "TableParser.h"
#include <utility>

TableParser::TableParser(Table table)
	: m_table(std::move(table))
{
}

Error TableParser::Parse(std::string const& input)
{
	m_lexer = Lexer{ input };

	return Error::NONE;
}

std::optional<Token> TableParser::GetLastToken() const
{
	return m_lastToken;
}

Token TableParser::Get()
{
	const auto token = m_lexer.Get();
	RecordToken(token);
	return token;
}

Token TableParser::Peek()
{
	const auto token = m_lexer.Peek();
	RecordToken(token);
	return token;
}

bool TableParser::Empty()
{
	return m_lexer.Empty();
}

void TableParser::RecordToken(Token const& token)
{
	m_lastToken = token;
}
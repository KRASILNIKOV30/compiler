#pragma once
#include "../lexer/Lexer.h"
#include "../table/Table.h"

class LLParser
{
public:
	explicit LLParser(Table table);
	Error Parse(std::string const& input);
	std::optional<Token> GetLastToken() const;

private:
	Token Get();
	Token Peek();
	bool Empty();
	void RecordToken(Token const& token);

private:
	Lexer m_lexer{ "" };
	Table m_table{};
	std::optional<Token> m_lastToken;
};





#pragma once
#include "../lexer/Lexer.h"

#include <optional>

class Parser
{
public:
	explicit Parser(std::string const& str)
		: m_lexer(str)
	{
	}

	bool Parse();
	Error GetError() const;
	std::optional<Token> GetLastToken() const;

private:
	Token Get();
	Token Peek();
	bool Empty();
	void RecordToken(Token const& token);
	bool Panic(Error error);
	bool Expression();
	bool ExpressionRem();
	bool ExpressionList();
	bool ExpressionListRem();
	bool Ident();
	bool Id();
	bool IdRem();
	bool SimExp();
	bool SimExpRem();
	bool SimTerm();
	bool SimTermRem();
	bool Term();

private:
	Lexer m_lexer;
	Error m_error = Error::NONE;
	std::optional<Token> m_token = std::nullopt;
};

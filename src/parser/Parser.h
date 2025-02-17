#pragma once
#include "../lexer/Lexer.h"

#include <optional>

class Parser
{
public:
	explicit Parser(Lexer&& lexer)
		: m_lexer(std::move(lexer))
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

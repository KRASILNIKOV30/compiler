#pragma once
#include "../lexer/Lexer.h"
#include "../table/Table.h"

#include <stack>

struct ErrorReason
{
	Guides expected;
	Token received;
	bool operator==(const ErrorReason&) const = default;
};

class LLParser
{
public:
	explicit LLParser(Table table);
	bool Parse(std::string const& input);
	ErrorReason GetError() const;

private:
	void Shift();
	Token Peek();
	bool Empty();
	void RecordToken(Token const& token);
	void RecordError();

private:
	Lexer m_lexer{ "" };
	Table m_table{};
	std::optional<Token> m_lastToken;
	ErrorReason m_error{};
	std::string m_symbol;
	TableRow m_row;
	size_t m_index = 0;
	std::stack<size_t> m_stack;
};





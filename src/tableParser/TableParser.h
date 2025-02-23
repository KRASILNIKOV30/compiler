#pragma once
#include "../lexer/Lexer.h"
#include <optional>
#include <unordered_set>

struct TableRow
{
	size_t index;
	std::string symbol;
	std::unordered_set<TokenType> guides;
	bool shift;
	Error error;
	size_t ptr;
	bool stack;
	bool end;
};

using Table = std::vector<TableRow>;

class TableParser
{
public:
	explicit TableParser(Table table);
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





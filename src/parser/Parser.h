#pragma once
#include "../lexer/Lexer.h"
#include "../table/Table.h"
#include <stack>
#include <variant>

struct ErrorReason
{
	Guides expected;
	Token received;
	bool operator==(const ErrorReason&) const = default;
};

class Parser
{
public:
	explicit Parser(Table table);
	bool Parse(std::string const& input);
	ErrorReason GetError() const;

private:
	void Shift(size_t value);
	void Fold(std::string const& ruleName, size_t ruleSize);
	bool NextAction();
	std::string GetCurrentSymbol();
	Token Peek();
	bool Empty();
	void RecordToken(Token const& token);

private:
	Lexer m_lexer{ "" };
	Table m_table{};
	std::optional<Token> m_lastToken;
	ErrorReason m_error{};
	std::stack<size_t> m_stateStack;

	using Node = std::variant<std::string, Token>;
	std::stack<Node> m_readStack;
	std::stack<std::string> m_foldStack;
	Action m_action{};
};

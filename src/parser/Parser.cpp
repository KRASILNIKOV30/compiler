#include "Parser.h"

#include "RemapToken.h"

#include <ranges>
#include <utility>

Parser::Parser(Table table, bool buildAST)
	: m_table(std::move(table))
	, m_buildAST(buildAST)
{
}

bool Parser::Parse(std::string const& input)
{
	m_lexer = Lexer{ input };
	m_error = {};
	m_stateStack = {};
	m_foldStack = {};
	m_readStack = {};
	m_action = {};
	m_lastToken.reset();
	m_stateStack.emplace(0);

	if (!NextAction())
	{
		return false;
	}
	while (!m_action.isOk)
	{
		if (m_action.type == ActionType::SHIFT)
		{
			Shift(m_action.value);
		}
		else
		{
			Fold(m_action.ruleName, m_action.ruleSize);
		}

		if (!NextAction())
		{
			return false;
		}
	}

	return true;
}

ErrorReason Parser::GetError() const
{
	return m_error;
}

void Parser::Shift(const size_t value)
{
	m_stateStack.push(value);
	if (m_foldStack.empty())
	{
		const auto token = m_lexer.Get();
		RecordToken(token);
		if (token.error == Error::EMPTY_INPUT)
		{
			m_readStack.emplace(END_SYMBOL);
		}
		else
		{
			m_readStack.emplace(token);
		}
	}
	else
	{
		m_readStack.emplace(m_foldStack.top());
		m_foldStack.pop();
	}
}

void Parser::Fold(std::string const& ruleName, const size_t ruleSize)
{
	std::vector<Node> nodes;
	for (size_t i = 0; i < ruleSize; ++i)
	{
		m_stateStack.pop();

		const auto& node = m_readStack.top();
		nodes.emplace_back(node);
		m_readStack.pop();
	}

	std::ranges::reverse(nodes);
	if (m_buildAST)
	{
		m_generator.Generate(ruleName, nodes);
	}

	m_foldStack.push(ruleName);
}

bool Parser::NextAction()
{
	const auto& row = m_table[m_stateStack.top()];
	const auto symbol = GetCurrentSymbol();
	const auto nextAction = row.find(symbol);
	if (nextAction != row.end())
	{
		m_action = nextAction->second;
		return true;
	}

	m_error = {
		.received = m_lastToken.value(),
	};

	for (const auto& lexeme : row
			| std::views::keys
			| std::views::filter([](const auto& s) { return IsTerm(s); }))
	{
		m_error.expected.insert(lexeme);
	}
	return false;
}

std::string Parser::GetCurrentSymbol()
{
	if (m_foldStack.empty())
	{
		const auto token = Peek();
		return token.error == Error::EMPTY_INPUT
			? END_SYMBOL
			: RemapTokenTypeToString(token.type);
	}

	return m_foldStack.top();
}

Token Parser::Peek()
{
	const auto token = m_lexer.Peek();
	RecordToken(token);
	return token;
}

bool Parser::Empty()
{
	return m_lexer.Empty();
}

void Parser::RecordToken(Token const& token)
{
	m_lastToken = token;
}
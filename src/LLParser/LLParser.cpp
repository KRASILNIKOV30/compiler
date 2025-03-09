#include "LLParser.h"

#include "RemapToken.h"

#include <utility>

const std::string END_SYMBOL = "#";

LLParser::LLParser(Table table)
	: m_table(std::move(table))
{
	m_row = m_table[0];
}

bool LLParser::Parse(std::string const& input)
{
	m_lexer = Lexer{ input };
	m_index = 0;
	m_row = m_table[0];
	m_error = {};
	Shift();

	while (true)
	{
		const auto& [symbol, guides, shift, error, ptr, stack, end] = m_row;
		if (!guides.contains(m_symbol))
		{
			if (error)
			{
				RecordError();
				return false;
			}
			++m_index;
			m_row = m_table.at(m_index);
			continue;
		}

		if (end)
		{
			return true;
		}

		if (shift)
		{
			Shift();
		}

		if (stack)
		{
			m_stack.push(m_index + 1);
		}

		if (ptr.has_value())
		{
			m_index = ptr.value();
		}
		else
		{
			m_index = m_stack.top();
			m_stack.pop();
		}
		m_row = m_table.at(m_index);
	}
}

ErrorReason LLParser::GetError() const
{
	return m_error;
}

void LLParser::Shift()
{
	const auto token = m_lexer.Get();
	RecordToken(token);
	m_symbol = token.error == Error::EMPTY_INPUT
		? END_SYMBOL
		: RemapTokenTypeToString(token.type);
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

void LLParser::RecordError()
{
	m_error = {
		.expected = m_row.guides,
		.received = m_lastToken.value(),
	};
}
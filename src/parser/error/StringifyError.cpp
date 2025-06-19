#include "StringifyError.h"

#include "../../print/PrintContainer.h"
#include "../RemapToken.h"

std::string StringifyLexerError(Token const& token)
{
	switch (token.error)
	{
	case Error::NONE:
		return "none";
	case Error::UNKNOWN_SYMBOL:
		return "unknown symbol " + token.value;
	case Error::INVALID_NUMBER:
		return "invalid number " + token.value;
	case Error::STRING_LITERAL_INCOMPLETE:
		return "string literal incomplete";
	case Error::EMPTY_INPUT:
		return "empty input";
	case Error::INVALID_ID:
		return "invalid id " + token.value;
	case Error::TERM_EXPECTED:
		return "term expected";
	case Error::PARAN_CLOSE_EXPECTED:
		return "parenthesis close expected";
	}

	throw std::runtime_error("Unknown error");
}

std::string StringifyError(ErrorReason const& error)
{
	const auto [expected, received] = error;
	std::ostringstream stream;
	stream << "Error at " << received.line + 1 << ":" << received.pos << ": ";
	if (received.error == Error::NONE)
	{
		const auto tokenType = RemapTokenTypeToString(received.type);
		stream << "{" << expected << " } expected, but " << tokenType;
		if (const auto value = received.value; value != tokenType)
		{
			stream << " (" << received.value << ")";
		}
		stream << " received." << std::endl;
	}
	else
	{
		stream << StringifyLexerError(received) << std::endl;
	}

	return stream.str();
}
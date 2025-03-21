#include "StringifyError.h"

#include "../RemapToken.h"
#include "../../print/PrintContainer.h"

std::string StringifyError(ErrorReason const& error)
{
	const auto [expected, received] = error;
	std::ostringstream stream;
	stream << "Error at position " << received.pos;
	if (received.error == Error::NONE)
	{
		const auto tokenType = RemapTokenTypeToString(received.type);
		stream << expected << " expected, but "
			<< tokenType << " (" << received.value << ") received." << std::endl;
	}
	else
	{
		// TODO: вывести ошибку лексера
		stream << " lexer error" << std::endl;
	}

	return stream.str();
}
#include "../Reader.h"

#include <iostream>

/**
 * P1 -> P2 P1Rem
 * P1Rem -> e | au P2 P1Rem
 * P2 -> P3 P2Rem
 * P2Rem -> e | ku P3 P2Rem
 * P3 -> uh-ty | ho P3 | nu P1 i_nu
 */

bool P1(Reader& reader);
bool P1Rem(Reader& reader);
bool P2(Reader& reader);
bool P2Rem(Reader& reader);
bool P3(Reader& reader);

bool P1(Reader& reader)
{
	return P2(reader) && P1Rem(reader);
}

bool P1Rem(Reader& reader)
{
	return reader.Empty() || reader.Peek() != 'a' || (reader.Get() == "au" && P2(reader) && P1Rem(reader));
}

bool P2(Reader& reader)
{
	return P3(reader) && P2Rem(reader);
}

bool P2Rem(Reader& reader)
{
	return reader.Empty() || reader.Peek() != 'k' || (reader.Get() == "ku" && P3(reader) && P2Rem(reader));
}

bool P3(Reader& reader)
{
	const auto str = reader.Get();
	return str == "uh-ty" || (str == "ho" && P3(reader)) || (str == "nu" && P1(reader) && reader.Get() == "i_nu");
}

int main()
{
	std::string input;
	while (std::getline(std::cin, input))
	{
		Reader reader(input);
		std::cout << (P1(reader) && reader.Empty() ? "OK" : "ERROR") << std::endl;
	}

	return EXIT_SUCCESS;
}


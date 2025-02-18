#include "Reader.h"
#include <iostream>
#include <algorithm>

bool IsNumber(std::string const& str)
{
	return std::ranges::all_of(str, ::isdigit);
}

bool IsDigit(const char c)
{
	return std::isdigit(c);
}

/**
 * P6 -> number
 */
bool P6(Reader& reader)
{
	return IsNumber(reader.Get());
}

/**
 * P5 -> turn_head
 */
bool P5(Reader& reader)
{
	return reader.Get() == "turn_head";
}

/**
 * P4 -> step_( P6 )
 */
bool P4(Reader& reader)
{
	return reader.Get() == "step_(" && P6(reader) && reader.Get() == ")";
}

bool P1(Reader& reader);

/**
 * P3 -> left | right | on45 P3 | hands_up P1 hands_down
 */
bool P3(Reader& reader)
{
	const auto str = reader.Get();
	return str == "left"
		|| str == "right"
		|| (str == "on45" && P3(reader))
		|| (str == "hands_up" && P1(reader) && reader.Get() == "hands_down");
}

/**
 * P2Rem -> e | P5 P3 P2Rem
 */
bool P2Rem(Reader& reader)
{
	return reader.Empty() || reader.Peek() != 't' || (P5(reader) && P3(reader) && P2Rem(reader));
}

/**
 * P2 -> P3 P2Rem
 */
bool P2(Reader& reader)
{
	return P3(reader) && P2Rem(reader);
}

/**
 * P1Rem -> e | P4 P2 P1Rem
 */
bool P1Rem(Reader& reader)
{
	return reader.Empty() || reader.PeekN(3) != "ste" || (P4(reader) && P2(reader) && P1Rem(reader));
}

/**
 * P1 -> P2 P1Rem
 */
bool P1(Reader& reader)
{
	return P2(reader) && P1Rem(reader);
}

/**
 * S -> start P1 stop
 */
bool S(Reader& reader)
{
	return reader.Get() == "start" && P1(reader) && reader.Get() == "stop";
}

/**
 * S -> start P1 stop
 * P1 -> P2 P1Rem
 * P1Rem -> e | P4 P2 P1Rem
 * P2 -> P3 P2Rem
 * P2Rem -> e | P5 P3 P2Rem
 * P3 -> left | right | on45 P3 | hands_up P1 hands_down
 * P4 -> step_( P6 )
 * P5 -> turn_head
 * P6 -> number
 */
int main()
{
	std::string input;
	std::getline(std::cin, input);
	Reader reader(input);
	std::cout << (S(reader) ? "OK" : "ERROR") << std::endl;
	std::cout << reader.GetCount() << std::endl;

	return EXIT_SUCCESS;
}

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <sstream>

#include "../src/Reader.h"
#include "../src/rules/Term.h" 
#include "../src/rules/Ident.h"

SCENARIO("Testing Num function behavior", "[Num]")
{
	WHEN("The stream contains only digits '1234'")
	{
		std::istringstream input("1234");
		Reader reader(input);

		THEN("It should be recognized as a valid num")
		{
			CHECK(Num(reader));
		}
	}

	WHEN("The stream starts with digits but has letters afterward '123abc'")
	{
		std::istringstream input("123abc");
		Reader reader(input);

		THEN("Num should parse only the digits part and succeed, leaving 'abc' in the stream")
		{
			auto startPos = reader.Count();
			CHECK(Num(reader));
			CHECK_FALSE(reader.Empty());

			CHECK(reader.Peek() == 'a');
			CHECK(reader.Count() == startPos + 3);
		}
	}

	WHEN("The stream does not start with a digit 'abc123'")
	{
		std::istringstream input("abc123");
		Reader reader(input);

		THEN("It should not be recognized as a num")
		{
			CHECK_FALSE(Num(reader));
		}
	}

	WHEN("The stream is empty")
	{
		std::istringstream input("");
		Reader reader(input);

		THEN("It should fail immediately")
		{
			CHECK_FALSE(Num(reader));
		}
	}
}

SCENARIO("Testing LitConst function behavior", "[LitConst]")
{
	WHEN(R"(The stream contains a simple string literal "hello")")
	{
		std::istringstream input("\"hello\"");
		Reader reader(input);

		THEN("It should be recognized as a valid string literal")
		{
			CHECK(LitConst(reader));
		}
	}

	WHEN(R"(The stream has missing closing quote "hello)")
	{
		std::istringstream input("\"hello");
		Reader reader(input);

		THEN("It should fail to parse")
		{
			CHECK_FALSE(LitConst(reader));
		}
	}

	WHEN("The stream starts with some other character, not a quote")
	{
		std::istringstream input("hello");
		Reader reader(input);

		THEN("It should fail to parse as a literal")
		{
			CHECK_FALSE(LitConst(reader));
		}
	}

	WHEN("The stream is empty")
	{
		std::istringstream input("");
		Reader reader(input);

		THEN("It should fail")
		{
			CHECK_FALSE(LitConst(reader));
		}
	}
}

SCENARIO("Testing Term function behavior", "[Term]")
{
	WHEN("The input is a simple identifier 'abc'")
	{
		std::istringstream input("abc");
		Reader reader(input);

		THEN("It should be recognized as a valid term (ident)")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is a number '1234'")
	{
		std::istringstream input("1234");
		Reader reader(input);

		THEN("It should be recognized as a valid term (num)")
		{
			CHECK(Term(reader));
		}
	}

	WHEN(R"(The input is a string literal "hello")")
	{
		std::istringstream input("\"hello\"");
		Reader reader(input);

		THEN("It should be recognized as a valid term (litconst)")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is 'true'")
	{
		std::istringstream input("true");
		Reader reader(input);

		THEN("It should be recognized as a valid term (keyword true)")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is 'false'")
	{
		std::istringstream input("false");
		Reader reader(input);

		THEN("It should be recognized as a valid term (keyword false)")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is '+abc'")
	{
		std::istringstream input("+abc");
		Reader reader(input);

		THEN("It should be recognized as a valid term: unary plus + ident")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is '-xyz'")
	{
		std::istringstream input("-xyz");
		Reader reader(input);

		THEN("It should be recognized as a valid term: unary minus - ident")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is 'notfoo'")
	{
		std::istringstream input("notfoo");
		Reader reader(input);

		THEN("It should be recognized as 'not' + term (ident 'foo')")
		{
			CHECK(Term(reader));
		}
	}

	WHEN("The input is 'not' with nothing else")
	{
		std::istringstream input("not");
		Reader reader(input);

		THEN("It should fail, because after 'not' тоже нужен term, а вход пуст")
		{
			CHECK_FALSE(Term(reader));
		}
	}

	WHEN("The input is '(abc)' but Expression(...) is always false")
	{
		std::istringstream input("(abc)");
		Reader reader(input);

		THEN("It should fail because Expression(...) returns false в текущей заглушке")
		{
			CHECK_FALSE(Term(reader));
		}
	}

	WHEN("The input is empty")
	{
		std::istringstream input("");
		Reader reader(input);

		THEN("It should fail, поскольку нет ни одной подходящей альтернативы")
		{
			CHECK_FALSE(Term(reader));
		}
	}

	WHEN("The input is nonsense ???")
	{
		std::istringstream input("???");
		Reader reader(input);

		THEN("It should fail, так как нет совпадений ни по одной ветке")
		{
			CHECK_FALSE(Term(reader));
		}
	}
}

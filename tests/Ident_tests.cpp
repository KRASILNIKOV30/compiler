#include "../src/rules/Ident.h"
#include <sstream>
#include <catch.hpp>

SCENARIO("Testing Id function behavior", "[Id]")
{
	WHEN("The stream contains a valid identifier 'validId123'")
	{
		std::istringstream input("validId123");
		Reader reader(input);

		THEN("It should be recognized as a valid identifier")
		{
			CHECK(Id(reader));
		}
	}

	WHEN("The stream contains an invalid identifier '1abc'")
	{
		std::istringstream input("1abc");
		Reader reader(input);

		THEN("It should be recognized as an invalid identifier (must start with a letter)")
		{
			CHECK_FALSE(Id(reader));
		}
	}

	WHEN("The stream contains a single character identifier 'a'")
	{
		std::istringstream input("a");
		Reader reader(input);

		THEN("It should be recognized as a valid identifier")
		{
			CHECK(Id(reader));
		}
	}

	WHEN("The stream contains an empty string")
	{
		std::istringstream input("");
		Reader reader(input);

		THEN("It should be recognized as an invalid identifier")
		{
			CHECK_FALSE(Id(reader));
		}
	}

	WHEN("The stream contains a string 'object.field'")
	{
		std::istringstream input("object.field");
		Reader reader(input);

		THEN("It should be recognized as an invalid identifier (space is not allowed)")
		{
			CHECK(Id(reader));
			CHECK(reader.Peek() == '.');
			CHECK(reader.Count() == 6);
		}
	}
}

SCENARIO("Testing ComplexId function behavior", "[ComplexId]")
{
	WHEN("The stream contains a valid complex identifier 'object.field.id'")
	{
		std::istringstream input("object.field");
		Reader reader(input);

		THEN("It should be recognized as a valid complex identifier")
		{
			CHECK(ComplexId(reader));
		}
	}

	WHEN("The stream contains a simple valid identifier")
	{
		std::istringstream input("validId");
		Reader reader(input);

		THEN("It should be recognized as a valid complex identifier")
		{
			CHECK(ComplexId(reader));
		}
	}

	WHEN("The stream contains a valid identifier followed by an invalid part")
	{
		std::istringstream input("abc123..ab");
		Reader reader(input);

		THEN("It should be recognized as an invalid complex identifier (special character after dot)")
		{
			CHECK_FALSE(ComplexId(reader));
		}
	}

	WHEN("The stream contains a valid identifier followed by an invalid dot (no following identifier) 'abc123.'")
	{
		std::istringstream input("obj.id) ");
		Reader reader(input);

		THEN("It should be recognized as an invalid complex identifier (dot without following identifier)")
		{
			CHECK(ComplexId(reader));
			CHECK(reader.Peek() == ')');
			CHECK(reader.Count() == 6);
		}
	}
}
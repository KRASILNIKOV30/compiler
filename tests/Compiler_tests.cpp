#include "../src/Compiler.h"
#include "catch.hpp"

SCENARIO("compiler tests")
{
	GIVEN("compiler")
	{
		Compiler compiler("grammar.txt");

		WHEN("compile const declaration")
		{
			std::ostringstream output;
			compiler.Compile("const i = 3;", output);

			THEN("code generated")
			{
				CHECK(output.str() == "def 0 0 1\n"
									  ".constants\n"
									  "number 3\n"
									  "\n"
									  ".code\n"
									  "1 const 0\n"
									  "1 getlocal 0\n"
									  "1 setlocal 0\n"
									  "1 ret");
			}
		}
	}
}

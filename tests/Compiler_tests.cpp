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

		WHEN("compile two constants")
		{
			std::ostringstream output;
			compiler.Compile(""
							 "const a = 3;\n"
							 "const b = 5;\n",
				output);

			THEN("code generated")
			{
				CHECK(output.str() == "def 0 0 2\n"
									  ".constants\n"
									  "number 3\n"
									  "number 5\n"
									  "\n"
									  ".code\n"
									  "1 const 0\n"
									  "1 getlocal 0\n"
									  "1 setlocal 0\n"
									  "1 const 1\n"
									  "1 getlocal 1\n"
									  "1 setlocal 1\n"
									  "1 ret");
			}
		}
	}
}

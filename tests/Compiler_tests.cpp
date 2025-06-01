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
			std::istringstream input("const i = 3;");
			compiler.Compile(input, output);

			THEN("code generated")
			{
				CHECK(output.str() == "def 0 0 1\n"
									  ".constants\n"
									  "number 3\n"
									  "\n"
									  ".code\n"
									  "1 const 0\n"
									  "1 get_local 0\n"
									  "1 set_local 0\n"
									  "1 ret");
			}
		}

		WHEN("compile two constants")
		{
			std::ostringstream output;
			std::istringstream input(""
									 "const a = 3;\n"
									 "const b = 5;\n");
			compiler.Compile(input,output);

			THEN("code generated")
			{
				CHECK(output.str() == "def 0 0 2\n"
									  ".constants\n"
									  "number 3\n"
									  "number 5\n"
									  "\n"
									  ".code\n"
									  "1 const 0\n"
									  "1 get_local 0\n"
									  "1 set_local 0\n"
									  "1 const 1\n"
									  "1 get_local 1\n"
									  "1 set_local 1\n"
									  "1 ret");
			}
		}
	}
}

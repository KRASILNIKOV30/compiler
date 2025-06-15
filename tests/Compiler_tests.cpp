#include "../src/Compiler.h"
#include "../src/utils/FoldLeft.h"
#include "catch.hpp"
#include "CombineStringLines.h"

namespace
{

struct TestData
{
	std::string description;
	std::vector<std::string> codeLines;
	std::vector<std::string> expectedByteCodeLines;
};

struct NegativeTestData
{
	std::string description;
	std::vector<std::string> codeLines;
	std::string expectedErrorMessage;
};

std::vector<TestData> LoadTestCases()
{
	return {
		{ "compile const declaration",
			{ "const i = 3;" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				"",
				".constants",
				"number 3",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
			} },

		{ "compile two constants",
			{ "const a = 3;"
			  "const b = 5;" },
			{ ".def",
				".argc 0",
				".locals 2",
				".name __EntryPoint__",
				"",
				".constants",
				"number 3",
				"number 5",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 1",
				"1 return" } },

		{ "const declaration with simple expression",
			{ "const i = 5 == 2;" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				"",
				".constants",
				"bool false",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
			} },

		{ "const declaration with bool expression",
			{ "const a = (5 - 2 == 3) or (7 div 2 > 3);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				"",
				".constants",
				"bool true",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
			} },

		{ "const declaration with bool expression",
			{ "const a = (5 - 2 == 3) and (7 div 2 > 3);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				"",
				".constants",
				"bool false",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
			} },

		{ "const declaration with arithmetic expression",
			{ "const a = (5 + 3) * 2 - (7 div 2) * (12 mod 10);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				"",
				".constants",
				"number 10",
				"",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
			} },
	};
}

std::vector<NegativeTestData> LoadNegativeTestCases()
{
	return {
		{ "declare two constants with the same names",
			{ "const a = 3;", "const a = 5;" },
			"Error at line 1: Symbol a is already defined" },

		{ "use undefined variable",
			{ "const a = 2 + b;" },
			"Error at line 0: Symbol b is undefined" },

		{ "add bool and integer",
			{ "const a = (2 > 3) + 5;" },
			"Error at line 0: Binary operator '+' is not applicable to types 'BOOL' and 'INT'." }
	};
}

} // namespace

SCENARIO("compiler tests")
{
	GIVEN("compiler")
	{
		Compiler compiler("grammar.txt");

		for (auto const& testCase : LoadTestCases())
		{
			WHEN(testCase.description)
			{
				std::ostringstream output;
				std::istringstream input(CombineStringLines(testCase.codeLines));
				compiler.Compile(input, output);

				THEN("code generated")
				{
					CHECK(output.str() == CombineStringLines(testCase.expectedByteCodeLines));
				}
			}
		}
	}
}

SCENARIO("compiler negative tests")
{
	GIVEN("compiler")
	{
		Compiler compiler("grammar.txt");

		for (auto const& [description, codeLines, expectedErrorMessage] : LoadNegativeTestCases())
		{
			WHEN(description)
			{
				std::ostringstream output;
				std::istringstream input(CombineStringLines(codeLines));

				THEN("error is thrown")
				{
					CHECK_THROWS_WITH(compiler.Compile(input, output), expectedErrorMessage);
				}
			}
		}
	}
}
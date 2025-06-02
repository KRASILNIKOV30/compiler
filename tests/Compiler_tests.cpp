#include "../src/Compiler.h"
#include "../src/utils/FoldLeft.h"
#include "catch.hpp"

struct TestData
{
	std::string description;
	std::vector<std::string> codeLines;
	std::vector<std::string> expectedByteCodeLines;
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
				"1 return" } }
	};
}

std::string CombineStringLines(std::vector<std::string> const& lines)
{
	return FoldLeft(lines, [](auto&& acc, auto const& line) { return acc + line + '\n'; });
}

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
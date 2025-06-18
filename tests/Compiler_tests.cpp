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
				".constants",
				"number 3",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "compile two constants",
			{ "const a = 3;", "const b = 5;" },
			{ ".def",
				".argc 0",
				".locals 2",
				".name __EntryPoint__",
				".constants",
				"number 3",
				"number 5",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 1",
				"1 return",
				".end_def",
				"",
			} },

		{ "const declaration with simple expression",
			{ "const i = 5 == 2;" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				".constants",
				"bool false",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "const declaration with bool expression",
			{ "const a = (5 - 2 == 3) or (7 div 2 > 3);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				".constants",
				"bool true",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "const declaration with bool expression",
			{ "const a = (5 - 2 == 3) and (7 div 2 > 3);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				".constants",
				"bool false",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "const declaration with arithmetic expression",
			{ "const a = (5 + 3) * 2 - (7 div 2) * (12 mod 10);" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				".constants",
				"number 10",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "if statement",
			{ "const a = 0;",
				"const b = 1;",
				"if (a > b)",
				"{",
				"const c = 2;",
				"};" },
			{
				".def",
				".argc 0",
				".locals 3",
				".name __EntryPoint__",
				".constants",
				"number 0",
				"number 1",
				"number 2",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 1",
				"1 get_local 0",
				"1 get_local 1",
				"1 cgt",
				"1 jmp_false endif1",
				"1 const 2",
				"1 set_local 2",
				"1 jmp endif1",
				"endif1:",
				"1 return",
				".end_def",
				"",
			} },

		{ "if else statement",
			{ "const a = 0;",
				"const b = 1;",
				"if (a > b)",
				"{",
				"const c = 2;",
				"}",
				"else",
				"{",
				"const d = 3;",
				"};" },
			{
				".def",
				".argc 0",
				".locals 4",
				".name __EntryPoint__",
				".constants",
				"number 0",
				"number 1",
				"number 2",
				"number 3",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 1",
				"1 get_local 0",
				"1 get_local 1",
				"1 cgt",
				"1 jmp_false else2",
				"1 const 2",
				"1 set_local 2",
				"1 jmp endif2",
				"else2:",
				"1 const 3",
				"1 set_local 3",
				"1 jmp endif2",
				"endif2:",
				"1 return",
				".end_def",
				"",
			} },

		{ "if elseif else statement",
			{ "const a = 0;",
				"const b = 1;",
				"if (a > b)",
				"{",
				"const c = a - b;",
				"}",
				"else if (a < b)",
				"{",
				"const d = b - a;",
				"}",
				"else",
				"{",
				"const f = a;",
				"};" },
			{
				".def",
				".argc 0",
				".locals 5",
				".name __EntryPoint__",
				".constants",
				"number 0",
				"number 1",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 1",
				"1 get_local 0",
				"1 get_local 1",
				"1 cgt",
				"1 jmp_false else3",
				"1 get_local 0",
				"1 get_local 1",
				"1 sub",
				"1 set_local 2",
				"1 jmp endif3",
				"else3:",
				"1 get_local 0",
				"1 get_local 1",
				"1 clt",
				"1 jmp_false else4",
				"1 get_local 1",
				"1 get_local 0",
				"1 sub",
				"1 set_local 3",
				"1 jmp endif4",
				"else4:",
				"1 get_local 0",
				"1 set_local 4",
				"1 jmp endif4",
				"endif4:",
				"1 jmp endif3",
				"endif3:",
				"1 return",
				".end_def",
				"",
			} },

		{ "while statement",
			{ "const a = 0;",
				"while (a < 0)",
				"{",
				"const b = 1;",
				"};" },
			{
				".def",
				".argc 0",
				".locals 2",
				".name __EntryPoint__",
				".constants",
				"number 0",
				"number 1",
				".code",
				"1 const 0",
				"1 set_local 0",
				"while1:",
				"1 get_local 0",
				"1 const 0",
				"1 clt",
				"1 jmp_false endwhile1",
				"1 const 1",
				"1 set_local 1",
				"1 jmp while1",
				"endwhile1:",
				"1 return",
				".end_def",
				"",
			} },

		{ "get array element",
			{ "const a = [123, 123];",
				"a[0] = 321;"
				"const b = a[0];" },
			{
				".def",
				".argc 0",
				".locals 2",
				".name __EntryPoint__",
				".constants",
				"number 123",
				"number 321",
				"number 0",
				".code",
				"1 const 0",
				"1 const 0",
				"1 create_arr 2",
				"1 set_local 0",
				"1 const 1",
				"1 get_local 0",
				"1 const 2",
				"1 set_el",
				"1 get_local 0",
				"1 const 2",
				"1 get_el",
				"1 set_local 1",
				"1 return",
				".end_def",
				"",
			} },

		{ "change variable value",
			{ "var a = 5;", "a = 10;" },
			{
				".def",
				".argc 0",
				".locals 1",
				".name __EntryPoint__",
				".constants",
				"number 5",
				"number 10",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 const 1",
				"1 set_local 0",
				"1 return",
				".end_def",
				"",
			} },

		{ "simple arrow function expression",
			{ "const b = 10;", "var a = (b: int) -> b * 2;" },
			{
				".def",
				".argc 0",
				".locals 2",
				".name __EntryPoint__",
				".constants",
				"number 10",
				".code",
				"1 const 0",
				"1 set_local 0",
				"1 load_fn 1",
				"1 closure",
				"1 set_local 1",
				"1 return",
				".end_def",
				"",
				".def",
				".argc 1",
				".locals 0",
				".name arrowFunction1",
				".constants",
				"number 2",
				".code",
				"1 get_local 0",
				"1 const 0",
				"1 mul",
				"1 return",
				".end_def",
				"",
			} },
	};
}

std::vector<NegativeTestData> LoadNegativeTestCases()
{
	return {
		{ "declare two constants with the same names",
			{ "const a = 3;", "const a = 5;" },
			"Error at line 2: Symbol a is already defined" },

		{ "use undefined variable",
			{ "const a = 2 + b;" },
			"Error at line 1: Symbol b is undefined" },

		{ "add bool and integer",
			{ "const a = (2 > 3) + 5;" },
			"Error at line 1: Binary operator '+' is not applicable to types 'BOOL' and 'INT'." }
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
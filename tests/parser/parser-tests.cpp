#define CATCH_CONFIG_MAIN
#include "../../src/guidesBuilder/GuidesBuilder.h"
#include "../../src/parser/error/StringifyError.h"
#include "../../src/parser/Parser.h"
#include "catch.hpp"

const std::string INPUT_FILE = "grammar.gfx";

TEST_CASE("slr e2e tests")
{
	std::ifstream input(INPUT_FILE);

	GuidesBuilder guidesBuilder(input);
	const auto rules = guidesBuilder.BuildGuidedRules();
	TableBuilder tableBuilder(rules);
	const auto table = tableBuilder.BuildTable();
	Parser parser(table, false);

	const auto Check = [&](std::string const& expr) {
		const auto result = parser.Parse(expr);
		if (!result)
		{
			throw std::runtime_error("Error: " + expr + ". " + StringifyError(parser.GetError()));
		}
		CHECK(result);
	};

	const auto CheckFalse = [&](std::string const& expr) {
		CHECK_FALSE(parser.Parse(expr));
	};

	Check("1;");
	Check("a;");
	Check("a > b;");
	Check("fn();");
	Check("fn(a, b);");
	Check("5 -(-5 - -5);");
	Check("-a + 5.3E-15 * -(-a + -b * (a * -b) -c) != abc;");
	Check("!a[7][a+5][b(3.5, c.d[f * ab])] OR 15 * (r - br MOD 5) AND TRUE;");
	Check("(a) <= (-5) != not - 5;");
	Check("(a) <= -(-5 != not - 5);");
	Check("+(+a) <= -(-+5 != not - 5);");
	Check("(+(+a) <= -(-+5 != not - 5)) * (a and 0.5E-3) or 1.0;");
	Check("(a) +(a)+ not - not a;");
	Check("(((a) +(a))+ not - (not a));");
	Check("0.5E-20;");
	Check("+not-not-+not-(-not-a);");
	Check("var bcd = 1;");
	Check("var bcd : int = 1;");
	Check("var bcd : string;");
	Check("const MAX = 0.5E-20;");
	Check("const MIN : int= 0.5E-20;");
	Check("while (a < b) {};");
	Check("{};");
	Check("function fn(a = 3): string { return \'temp\'; };");
	Check("function fn(a: int): int -> int { };");
	Check("function fn(): int { };");
	Check("if (a) {};");
	Check("if (a) {} else {};");
	Check("if (a) {} else if (b) {} else {};");
	Check("const a = (a: int) -> {};");
	Check("var a: string -> int -> int -> bool = (a: string) -> (a: int, b: int) -> a > b;");
	Check("const B = (b: int = 0) -> {};");
	Check("const A: int -> string = (b: int = 0) -> {};");
	Check("1;5;");
	Check("1;5;\n");
	Check("1;\n5;");
	Check("\n1;5;");
	Check("\n1;\n5;");
	Check("1;\n5;\n");
	Check("\n1;\n5;\n");
	Check("\n\n\n1;\n\n5;\n");
	Check("const a = 3; const b = 5;\n");
	Check("const a1 = 3;\nconst b1 = 5;\n");

	CheckFalse("");
	CheckFalse(";");
	CheckFalse(";;;");
	CheckFalse(")(;");
	CheckFalse("ab.5;");
	CheckFalse("ab.;");
	CheckFalse("5a;");
	CheckFalse("fn(a, );");
	CheckFalse("fn(a +, b);");
	CheckFalse("+-+-;");
	CheckFalse(".;");
	CheckFalse("-a + 5.3E-15 * (-a + -b * (a * -b) -c != abc;");
	CheckFalse("!a[7][a+5][b(3.5, c.d[f * ab,])] OR 15 * (r - br MOD 5) AND TRUE;");
	CheckFalse("(a) <= (-5 != not - 5;");
	CheckFalse("(a)- <= -(-5 != not - 5);");
	CheckFalse("(+(+a <= -(-+5 != not - 5)) * (a and 0.5E-3) or 1.0;");
	CheckFalse("(a) +(a)+ or - not a;");
	CheckFalse("05E-20;");
	CheckFalse("05.E-20;");
	CheckFalse("15.E-20;");
	CheckFalse("(((a) +(a)) ! not - (not a));");
	CheckFalse("var bcd;");
	CheckFalse("var bcd = 3");
	CheckFalse("const MAX: int;");
	CheckFalse("const MAX;");
	CheckFalse("const MAX[3] = 123");
	CheckFalse("while (a < b) ;");
	CheckFalse("function fn(a =): string { return \'temp\'; };");
	CheckFalse("function fn(a): void { };");
	CheckFalse("function fn() { };");
	CheckFalse("if (a);");
	CheckFalse("if (a) else;");
	CheckFalse("a = b");
	CheckFalse("{;};");
}
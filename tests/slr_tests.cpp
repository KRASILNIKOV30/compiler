#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/guidesBuilder/GuidesBuilder.h"
#include "../src/LLParser/LLParser.h"

const std::string INPUT_FILE = "rules.txt";

TEST_CASE("slr e2e tests")
{
	std::ifstream input(INPUT_FILE);

	GuidesBuilder guidesBuilder(input);
	const auto rules = guidesBuilder.BuildGuidedRules();
	TableBuilder tableBuilder(rules);
	const auto table = tableBuilder.BuildTable();
	LLParser parser(table);

	const auto Check = [&](std::string const& expr) {
		CHECK(parser.Parse(expr));
	};

	const auto CheckFalse = [&](std::string const& expr) {
		CHECK_FALSE(parser.Parse(expr));
	};

	Check("1");
	Check("a");
	Check("a > b");
	Check("fn()");
	Check("fn(a, b)");
	Check("5 -(-5 - -5)");
	Check("-a + 5.3E-15 * -(-a + -b * (a * -b) -c) != abc");
	Check("!a[7][a+5][b(3.5, c.d[f * ab])] OR 15 * (r - br MOD 5) AND TRUE");
	Check("(a) <= (-5) != not - 5");
	Check("(a) <= -(-5 != not - 5)");
	Check("+(+a) <= -(-+5 != not - 5)");
	Check("(+(+a) <= -(-+5 != not - 5)) * (a and 0.5E-3) or 1.0");
	Check("(a) +(a)+ not - not a");
	Check("(((a) +(a))+ not - (not a))");
	Check("0.5E-20");
	Check("+not-not-+not-(-not-a)");

	CheckFalse("");
	CheckFalse(")(");
	CheckFalse("ab.5");
	CheckFalse("ab.");
	CheckFalse("5a");
	CheckFalse("fn(a, )");
	CheckFalse("fn(a +, b)");
	CheckFalse("+-+-");
	CheckFalse(".");
	CheckFalse("-a + 5.3E-15 * (-a + -b * (a * -b) -c != abc");
	CheckFalse("!a[7][a+5][b(3.5, c.d[f * ab,])] OR 15 * (r - br MOD 5) AND TRUE");
	CheckFalse("(a) <= (-5 != not - 5");
	CheckFalse("(a)- <= -(-5 != not - 5)");
	CheckFalse("(+(+a <= -(-+5 != not - 5)) * (a and 0.5E-3) or 1.0");
	CheckFalse("(a) +(a)+ or - not a");
	CheckFalse("05E-20");
	CheckFalse("05.E-20");
	CheckFalse("15.E-20");
	CheckFalse("(((a) +(a)) ! not - (not a))");
}
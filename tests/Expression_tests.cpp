#include <catch.hpp>
#include "../../src/lexer/Lexer.h"
#include "../src/rules/Expression.h"

void Check(std::string const& str)
{
	Lexer lexer(str);
	CHECK((Expression(lexer) && lexer.Empty()));
}

void CheckFalse(std::string const& str)
{
	Lexer lexer(str);
	CHECK_FALSE((Expression(lexer) && lexer.Empty()));
}

TEST_CASE("positive tests")
{
	Check("1");
	Check("a");
	Check("a > b");
	Check("fn()");
	Check("fn(a, b)");
	Check("5 -(-5 - -5)");
	Check("-a + 5.3E-15 * -(-a + -b * (a * -b) -c) != abc");
	Check("!a[7][a+5][b(3.5, c.d[f * ab])] OR 15 * (r - br MOD 5) AND TRUE");
}

TEST_CASE("negative tests")
{
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
}

#include "catch.hpp"
#include "../../src/guidesBuilder/GuidesBuilder.h"

void Check(std::string const& rawRules, std::string const& expected)
{
	GuidesBuilder builder(rawRules);
	const auto rules = builder.BuildGuidedRules();
	CHECK(rules.has_value());
	CHECK(rules.value() == expected);
}

TEST_CASE("guides builder tests")
{
	Check(""
		"<S> - a <A> #\n"
		"<S> - b\n"
		"<A> - c <A> <S>\n"
		"<A> - e\n",
		""
		"<S> - a <A> # / a\n"
		"<S> - b / b\n"
		"<A> - c <A> <S> / c\n"
		"<A> - e / # b a\n");
}

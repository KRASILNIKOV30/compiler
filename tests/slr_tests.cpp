#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/guidesBuilder/GuidesBuilder.h"
#include "../src/LLParser/LLParser.h"

TEST_CASE("list tests")
{
	const std::string rulesStr = ""
		"<S> -> <ident> #\n"
		"<expList> -> <ident>\n"
		"<ident> -> id ( <expList> ) | id\n"
		"<A> -> id [ <expression> ]";

	GuidesBuilder guidesBuilder(rulesStr);
	const auto rules = guidesBuilder.BuildGuidedRules();
	TableBuilder tableBuilder(rules);
	const auto table = tableBuilder.BuildTable();
	LLParser parser(table);

	const auto Check = [&](std::string const& expr) {
		CHECK(parser.Parse(expr));
	};

	Check("fn(id)");
}
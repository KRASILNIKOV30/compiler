#include "catch.hpp"
#include <sstream>
#include "../../src/emptyRulesDeleter/EmptyRulesDeleter.h"
#include "../../src/parseRawRules/ParseRules.h"
#include "../../src/parseRawRules/StringifyRules.h"

void Check(std::string const& rulesStr, std::string const& expectedStr)
{
	std::stringstream ss(rulesStr);
	const auto rules = ParseRawRules(ss);
	EmptyRulesDeleter deleter(rules);
	const auto handledRules = deleter.DeleteEmptyRules();
	const auto handledRulesStr = StringifyRules(handledRules);

	REQUIRE(handledRulesStr == expectedStr);
}

TEST_CASE("EmptyRulesDeleter tests")
{
	Check(""
		"<S> - a <A> | <B>\n"
		"<A> - a\n"
		"<B> - b\n",
		""
		"<S> - a <A> | <B>\n"
		"<A> - a\n"
		"<B> - b\n");

	Check(""
		"<S> - ( <optList> )\n"
		"<optList> - <list> | e\n"
		"<list> - <list> , id | id\n",
		""
		"<S> - ( <optList> ) | ( )\n"
		"<optList> - <list>\n"
		"<list> - <list> , id | id\n");

	Check(""
		"<S> - ( <optList> )\n"
		"<optList> - <list>\n"
		"<list> - <list> , id | id | e\n",
		""
		"<S> - ( <optList> ) | ( )\n"
		"<optList> - <list>\n"
		"<list> - <list> , id | id | , id\n");

	Check(""
		"<S> - <A> <B> <C>\n"
		"<A> - a | e\n"
		"<B> - b | e\n"
		"<C> - c | e\n",
		""
		"<S> - <A> <B> <C> | <B> <C> | <A> <C> | <C> | <A> <B> | <B> | <A> | e\n"
		"<A> - a\n"
		"<B> - b\n"
		"<C> - c\n");

	std::string error;
	try
	{
		Check(""
			"<S> - <A> <B>\n"
			"<A> - a <B> <B>\n"
			"<B> - b | e",
			"");
	}
	catch (const std::exception& e)
	{
		error = e.what();
	}
	REQUIRE(error == "rule <A> have more than one e-generating rules <B>");
}

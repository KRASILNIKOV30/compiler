#include "../../src/tableBuilder/TableBuilder.h"
#include "catch.hpp"

void Check(std::string const& str, Table const& expected)
{
	TableBuilder builder(str);
	const auto table = builder.BuildTable();
	CHECK(table == expected);
}

TEST_CASE("table builder tests")
{
	Check(""
		"<S> - int <idList> # / int\n"
		"<idList> - <idList> , <id> / <idList> <id> B C A\n"
		"<idList> - <id> / A C B\n"
		"<id> - A / A\n"
		"<id> - B / B\n"
		"<id> - C / C\n",
		Table{
			{ { "<S>", { .isOk = true } },
			  { "int", { ActionType::SHIFT, 1 } } },
			{
				{ "<idList>", { ActionType::SHIFT, 2 } },
				{ "<id>", { ActionType::SHIFT, 3 } },
				{ "A", { ActionType::SHIFT, 4 } },
				{ "B", { ActionType::SHIFT, 5 } },
				{ "C", { ActionType::SHIFT, 6 } },
			},
			{ { ",", { ActionType::SHIFT, 7 } },
			  { "#", { ActionType::RULE, 0, "<S>", 2 } } },

			{ { ",", { ActionType::RULE, 2, "<idList>", 1 } },
			  { "#", { ActionType::RULE, 2, "<idList>", 1 } } },

			{ { ",", { ActionType::RULE, 3, "<id>", 1 } },
			  { "#", { ActionType::RULE, 3, "<id>", 1 } } },

			{ { ",", { ActionType::RULE, 4, "<id>", 1 } },
			  { "#", { ActionType::RULE, 4, "<id>", 1 } } },

			{ { ",", { ActionType::RULE, 5, "<id>", 1 } },
			  { "#", { ActionType::RULE, 5, "<id>", 1 } } },

			{
				{ "<id>", { ActionType::SHIFT, 8 } },
				{ "A", { ActionType::SHIFT, 4 } },
				{ "B", { ActionType::SHIFT, 5 } },
				{ "C", { ActionType::SHIFT, 6 } },
			},

			{ { ",", { ActionType::RULE, 1, "<idList>", 3 } },
			  { "#", { ActionType::RULE, 1, "<idList>", 3 } } } });
}
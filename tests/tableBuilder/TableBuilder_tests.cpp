#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../../src/tableBuilder/TableBuilder.h"

void Check(std::string const& str, Table const& expected)
{
	const TableBuilder builder(str);
	const auto table = builder.BuildTable();
	CHECK(table == expected);
}

TEST_CASE("table builder tests")
{
	Check(
		"<Z> - <S> # / a b\n"
		"<S> - a / a\n"
		"<S> - b / b\n",
		Table{
			{ "<Z>", { "a", "b" }, false, true, 3, false, false },
			{ "<S>", { "a" }, false, false, 5, false, false },
			{ "<S>", { "b" }, false, true, 6, false, false },
			{ "<S>", { "a", "b" }, false, true, 1, true, false },
			{ "#", { "#" }, true, true, std::nullopt, false, true },
			{ "a", { "a" }, true, true, std::nullopt, false, false },
			{ "b", { "b" }, true, true, std::nullopt, false, false },
		});

	Check(
		"<list> - el <listRem> # / el\n"
		"<listRem> - e / #\n"
		"<listRem> - , el <listRem> / ,\n",
		Table{
			{ "<list>", { "el" }, false, true, 3, false, false },
			{ "<listRem>", { "#" }, false, false, 6, false, false },
			{ "<listRem>", { "," }, false, true, 7, false, false },
			{ "el", { "el" }, true, true, 4, false, false },
			{ "<listRem>", { "#", "," }, false, true, 1, true, false },
			{ "#", { "#" }, true, true, std::nullopt, false, true },
			{ "e", { "#" }, false, true, std::nullopt, false, false },
			{ ",", { "," }, true, true, 8, false, false },
			{ "el", { "el" }, true, true, 9, false, false },
			{ "<listRem>", { "#", "," }, false, true, 1, false, false },
		});
}

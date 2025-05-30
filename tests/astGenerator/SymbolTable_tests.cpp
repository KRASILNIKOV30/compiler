#include "../../src/astGenerator/SymbolTable.h"
#include <catch.hpp>

SCENARIO("SymbolTable supports proper scoping and shadowing")
{
	GIVEN("An empty SymbolTable")
	{
		SymbolTable table;

		WHEN("A symbol is added and retrieved")
		{
			Symbol sym{ false, Type{ PrimitiveType::INT } };
			table.Add("x", sym);

			THEN("It can be retrieved with correct properties")
			{
				auto result = table.Get("x");
				CHECK(result.isConst == false);
				CHECK(result.type == Type{ PrimitiveType::INT });
			}
		}

		WHEN("Trying to retrieve an undefined symbol")
		{
			THEN("An exception is thrown")
			{
				CHECK_THROWS_WITH(table.Get("y"), "Symbol y is undefined");
			}
		}

		WHEN("A symbol is declared in one scope and shadowed in another")
		{
			Symbol outerSym{ false, Type{ PrimitiveType::FLOAT } };
			table.Add("a", outerSym);

			table.CreateScope();

			Symbol innerSym{ true, Type{ PrimitiveType::STRING, PrimitiveType::INT } };
			table.Add("a", innerSym);

			THEN("The inner symbol shadows the outer one")
			{
				auto result = table.Get("a");
				CHECK(result.isConst == true);
				CHECK(result.type == Type{ PrimitiveType::STRING, PrimitiveType::INT });
			}

			AND_WHEN("The inner scope is deleted")
			{
				table.DeleteScope();

				THEN("The outer symbol becomes visible again")
				{
					auto result = table.Get("a");
					CHECK(result.isConst == false);
					CHECK(result.type == Type{ PrimitiveType::FLOAT });
				}
			}
		}

		WHEN("A symbol is declared in an inner scope with a unique name")
		{
			Symbol outerSym{ false, Type{ PrimitiveType::FLOAT } };
			table.Add("x", outerSym);

			table.CreateScope();

			Symbol innerSym{ true, Type{ PrimitiveType::STRING } };
			table.Add("y", innerSym);

			THEN("Both symbols are accessible")
			{
				auto x = table.Get("x");
				auto y = table.Get("y");

				CHECK(x.type == Type{ PrimitiveType::FLOAT });
				CHECK(y.type == Type{ PrimitiveType::STRING });
			}
		}

		WHEN("Trying to declare a symbol with the same name in the same scope")
		{
			Symbol sym1{ false, Type{ PrimitiveType::INT } };
			table.Add("dup", sym1);

			THEN("A duplicate declaration throws an error")
			{
				Symbol sym2{ true, Type{ PrimitiveType::STRING } };
				CHECK_THROWS_WITH(table.Add("dup", sym2), "Symbol dup is already defined");
			}
		}

		WHEN("Symbols are added across nested scopes")
		{
			table.Add("var", Symbol{ false, Type{ PrimitiveType::INT } });
			table.CreateScope();
			table.Add("tmp", Symbol{ true, Type{ PrimitiveType::FLOAT } });
			table.CreateScope();
			table.Add("tmp2", Symbol{ true, Type{ PrimitiveType::STRING } });

			THEN("All symbols can be resolved correctly")
			{
				CHECK(table.Get("var").type == Type{ PrimitiveType::INT });
				CHECK(table.Get("tmp").type == Type{ PrimitiveType::FLOAT });
				CHECK(table.Get("tmp2").type == Type{ PrimitiveType::STRING });
			}

			AND_WHEN("Scopes are deleted one by one")
			{
				table.DeleteScope();
				CHECK_THROWS_WITH(table.Get("tmp2"), "Symbol tmp2 is undefined");

				table.DeleteScope();
				CHECK_THROWS_WITH(table.Get("tmp"), "Symbol tmp is undefined");

				CHECK(table.Get("var").type == Type{ PrimitiveType::INT });
			}
		}
	}
}

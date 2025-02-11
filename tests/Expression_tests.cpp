#include "../src/Reader.h"
#include "../src/rules/Expression.h"
#include <catch.hpp>
#include <sstream>

SCENARIO("Testing Expression function behavior", "[Expression]")
{
    WHEN("The input is a simple term '123'")
    {
        std::istringstream input("123");
        Reader reader(input);

        THEN("It should be recognized as a valid expression (term)")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is a simple expression '123 + 456'")
    {
        std::istringstream input("123 + 456");
        Reader reader(input);

        THEN("It should be recognized as a valid expression (term PLUSO term)")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is a complex expression '123 + 456 * 789'")
    {
        std::istringstream input("123 + 456 * 789");
        Reader reader(input);

        THEN("It should be recognized as a valid expression (term PLUSO term MULO term)")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is a relational expression '123 == 456'")
    {
        std::istringstream input("123 == 456");
        Reader reader(input);

        THEN("It should be recognized as a valid expression (term Rel term)")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is a complex relational expression '123 + 456 == 789 * 10'")
    {
        std::istringstream input("123 + 456 == 789 * 10");
        Reader reader(input);

        THEN("It should be recognized as a valid expression (term PLUSO term Rel term MULO term)")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression '123 +'")
    {
        std::istringstream input("123 +");
        Reader reader(input);

        THEN("It should fail because the expression is incomplete")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression '123 =='")
    {
        std::istringstream input("123 ==");
        Reader reader(input);

        THEN("It should fail because the expression is incomplete")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression '123 *'")
    {
        std::istringstream input("123 *");
        Reader reader(input);

        THEN("It should fail because the expression is incomplete")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression '123 + * 456'")
    {
        std::istringstream input("123 + * 456");
        Reader reader(input);

        THEN("It should fail because of invalid operator sequence")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression '123 == + 456'")
    {
        std::istringstream input("123 == + 456");
        Reader reader(input);

        THEN("It should fail because of invalid operator sequence")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an empty string")
    {
        std::istringstream input("");
        Reader reader(input);

        THEN("It should fail because the expression is empty")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is a complex expression with parentheses '(123 + 456) * 789'")
    {
        std::istringstream input("(123 + 456) * 789");
        Reader reader(input);

        THEN("It should be recognized as a valid expression")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is a complex expression with nested parentheses '((123 + 456) * 789) == 1000'")
    {
        std::istringstream input("((123 + 456) * 789) == 1000");
        Reader reader(input);

        THEN("It should be recognized as a valid expression")
        {
            CHECK(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression with mismatched parentheses '(123 + 456 * 789'")
    {
        std::istringstream input("(123 + 456 * 789");
        Reader reader(input);

        THEN("It should fail because of mismatched parentheses")
        {
            CHECK_FALSE(Expression(reader));
        }
    }

    WHEN("The input is an invalid expression with mismatched parentheses '123 + 456) * 789'")
    {
        std::istringstream input("123 + 456) * 789");
        Reader reader(input);

        THEN("It should fail because of mismatched parentheses")
        {
            CHECK_FALSE(Expression(reader));
        }
    }
}
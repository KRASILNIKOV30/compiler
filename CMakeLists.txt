cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)

add_executable(lexer
        src/main.cpp
        src/rules/Ident.h
        src/lexer/reader/Reader.h
        src/lexer/token/Token.h
        src/rules/Expression.h
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/rules/Id.h
        src/lexer/rules/Number.h
        src/lexer/rules/String.h
        src/lexer/rules/ReservedWords.h
        src/lexer/rules/SpecialChars.h
        src/rules/ExpressionList.h
)

add_executable(lexer_tests
        src/rules/Ident.h
        src/lexer/reader/Reader.h
        src/lexer/token/Token.h
        src/rules/Expression.h
        tests/Reader_tests.cpp
        tests/Lexer_tests.cpp
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/rules/Id.h
        src/lexer/rules/Number.h
        src/lexer/rules/String.h
        src/lexer/rules/ReservedWords.h
        src/lexer/rules/SpecialChars.h
        src/rules/ExpressionList.h
)

target_link_libraries(lexer_tests PUBLIC catch2)
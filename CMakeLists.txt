cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)

add_executable(parser
        src/main.cpp
        src/parser/rules/Ident.cpp
        src/lexer/reader/Reader.h
        src/lexer/token/Token.h
        src/parser/rules/Expression.cpp
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/rules/Id.h
        src/lexer/rules/Number.h
        src/lexer/rules/String.h
        src/lexer/rules/ReservedWords.h
        src/lexer/rules/SpecialChars.h
        src/parser/rules/ExpressionList.cpp
        src/parser/rules/Operations.h
        src/parser/rules/Term.cpp
        src/parser/rules/SimTerm.cpp
        src/parser/rules/SimExp.cpp
        src/parser/Parser.h
        src/errors/Error.h
        src/parser/Parser.cpp
)

add_executable(parser_tests
        src/parser/rules/Ident.cpp
        src/lexer/reader/Reader.h
        src/lexer/token/Token.h
        src/parser/rules/Expression.cpp
        tests/Reader_tests.cpp
        tests/Lexer_tests.cpp
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/rules/Id.h
        src/lexer/rules/Number.h
        src/lexer/rules/String.h
        src/lexer/rules/ReservedWords.h
        src/lexer/rules/SpecialChars.h
        src/parser/rules/ExpressionList.cpp
        src/parser/rules/Operations.h
        src/parser/rules/Term.cpp
        src/parser/rules/SimTerm.cpp
        src/parser/rules/SimExp.cpp
        tests/Expression_tests.cpp
        src/parser/Parser.h
        src/errors/Error.h
        src/parser/Parser.cpp
        tests/Parser_tests.cpp
)

target_link_libraries(parser_tests PUBLIC catch2)
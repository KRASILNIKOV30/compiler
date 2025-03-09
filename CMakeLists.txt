cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)

set(LEXER_SOURCES
        src/lexer/reader/Reader.h
        src/lexer/rules/Id.h
        src/lexer/rules/Number.h
        src/lexer/rules/String.h
        src/lexer/rules/ReservedWords.h
        src/lexer/rules/SpecialChars.h
        src/lexer/token/Token.h
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/errors/Error.h
)

set(LEXER_TESTS
        tests/lexer/Lexer_tests.cpp
        tests/lexer/Reader_tests.cpp
)

set(PARSER_SOURCES
        src/parser/rules/Ident.cpp
        src/parser/rules/Expression.cpp
        src/parser/rules/ExpressionList.cpp
        src/parser/rules/Operations.h
        src/parser/rules/Term.cpp
        src/parser/rules/SimTerm.cpp
        src/parser/rules/SimExp.cpp
        src/parser/Parser.h
        src/parser/Parser.cpp
)

set(PARSER_TESTS
        tests/parser/Parser_tests.cpp
        tests/parser/Expression_tests.cpp
)

set(LL_PARSER_SOURCES
        ${LEXER_SOURCES}
        src/LLParser/LLParser.h
        src/LLParser/LLParser.cpp
        src/table/Table.h
)

set(LL_PARSER_TESTS
        ${LEXER_TESTS}
)

set(TABLE_BUILDER_SOURCES
        src/tableBuilder/TableBuilder.h
)

set(TABLE_BUILDER_TESTS
        tests/tableBuilder/TableBuilder_tests.cpp
)

add_executable(parser_tests
        ${LEXER_SOURCES}
        ${PARSER_SOURCES}
        ${LEXER_TESTS}
        ${PARSER_TESTS}
)

add_executable(ll-parser_tests
        ${LL_PARSER_SOURCES}
        ${LL_PARSER_TESTS}
)

add_executable(table-builder_tests
        ${TABLE_BUILDER_SOURCES}
        ${TABLE_BUILDER_TESTS}
)

target_link_libraries(ll-parser_tests PUBLIC catch2)
target_link_libraries(parser_tests PUBLIC catch2)
target_link_libraries(table-builder_tests PUBLIC catch2)
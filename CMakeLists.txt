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
        src/LLParser/RemapToken.h
        src/LLParser/error/StringifyError.h
        src/LLParser/error/StringifyError.cpp
)

set(TABLE_BUILDER_SOURCES
        src/tableBuilder/TableBuilder.h
        src/tableBuilder/parseRules/ParseRules.h
        src/tableBuilder/parseRules/ParseRules.cpp
)

set(TABLE_BUILDER_TESTS
        tests/tableBuilder/TableBuilder_tests.cpp
)

set(GUIDES_BUILDER_SOURCES
        src/guidesBuilder/GuidesBuilder.h
        src/parseRawRules/ParseRules.h
        src/parseRawRules/ParseRules.cpp
        src/parseRawRules/StringifyRules.h
        src/parseRawRules/StringifyRules.cpp
        src/print/PrintContainer.h
        src/guidesBuilder/GuidesBuilder.cpp
)

set(GUIDES_BUILDER_TESTS
        tests/guidesBuilder/GuidesBuilder_tests.cpp
)

set(LL_PARSER_TESTS
        ${LEXER_TESTS}
        tests/LLParser/LLParser_tests.cpp
        src/LLParser/error/StringifyError.cpp
)

add_executable(parser_tests
        ${LEXER_SOURCES}
        ${PARSER_SOURCES}
        ${LEXER_TESTS}
        ${PARSER_TESTS}
)

add_executable(ll-parser_tests
        ${TABLE_BUILDER_SOURCES}
        ${GUIDES_BUILDER_SOURCES}
        ${LL_PARSER_SOURCES}
        ${LL_PARSER_TESTS}
        ${TABLE_BUILDER_TESTS}
        ${GUIDES_BUILDER_TESTS}
)

add_executable(ll_parser
        ${TABLE_BUILDER_SOURCES}
        ${GUIDES_BUILDER_SOURCES}
        ${LL_PARSER_SOURCES}
        src/main.cpp
)

add_executable(guides-builder_tests
        ${GUIDES_BUILDER_SOURCES}
        tests/guidesBuilder/GuidesBuilder_tests.cpp
)

add_executable(table-builder_tests
        ${TABLE_BUILDER_SOURCES}
        ${TABLE_BUILDER_TESTS}
)

add_executable(empty-rules-deleter_tests
        src/emptyRulesDeleter/EmptyRulesDeleter.h
        src/parseRawRules/ParseRules.h
        src/parseRawRules/ParseRules.cpp
        src/parseRawRules/StringifyRules.h
        src/parseRawRules/StringifyRules.cpp
        tests/emptyRulesDeleter/EmptyRulesDeleter_tests.cpp
)

target_link_libraries(parser_tests PUBLIC catch2)
target_link_libraries(ll-parser_tests PUBLIC catch2)
target_link_libraries(guides-builder_tests PUBLIC catch2)
target_link_libraries(table-builder_tests PUBLIC catch2)
target_link_libraries(empty-rules-deleter_tests PUBLIC catch2)
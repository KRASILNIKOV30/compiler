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
        ${LEXER_SOURCES}
        src/parser/Parser.h
        src/parser/Parser.cpp
        src/table/Table.h
        src/parser/RemapToken.h
        src/parser/error/StringifyError.h
        src/parser/error/StringifyError.cpp
)

set(TABLE_BUILDER_SOURCES
        src/tableBuilder/TableBuilder.h
)

set(CODE_GENERATOR_TESTS
        tests/codeGenerator/CodeGenerator_tests.cpp
)

set(TABLE_BUILDER_TESTS
        tests/tableBuilder/TableBuilder_tests.cpp
)

set(GUIDES_BUILDER_SOURCES
        src/guidesBuilder/emptyRulesDeleter/EmptyRulesDeleter.h
        src/guidesBuilder/GuidesBuilder.h
        src/guidesBuilder/GuidesBuilder.cpp
        src/parseRules/ParseRules.h
        src/parseRules/ParseRawRules.cpp
        src/parseRules/ParseRules.cpp
        src/parseRules/StringifyRules.h
        src/parseRules/StringifyRules.cpp
        src/print/PrintContainer.h
)

set(GUIDES_BUILDER_TESTS
        tests/guidesBuilder/GuidesBuilder_tests.cpp
        tests/emptyRulesDeleter/EmptyRulesDeleter_tests.cpp
)

set(PARSER_TESTS
        ${LEXER_TESTS}
        tests/e2e-parser-tests.cpp
        src/astGenerator/Type.h
        src/ast/expression/Term.h
)

set(AST_GENERATOR_SOURCES
        src/astGenerator/SymbolTable.h
        src/astGenerator/Type.h
)

set(AST_GENERATOR_TESTS
        tests/astGenerator/SymbolTable_tests.cpp
        src/astGenerator/ASTGenerator.cpp
        src/astGenerator/ASTGenerator.h
)

add_executable(compiler
        ${TABLE_BUILDER_SOURCES}
        ${GUIDES_BUILDER_SOURCES}
        ${PARSER_SOURCES}
        src/main.cpp
)

add_executable(tests
        ${TABLE_BUILDER_SOURCES}
        ${GUIDES_BUILDER_SOURCES}
        ${PARSER_SOURCES}
        ${PARSER_TESTS}
        ${CODE_GENERATOR_TESTS}
        ${TABLE_BUILDER_TESTS}
        ${GUIDES_BUILDER_TESTS}
        ${AST_GENERATOR_SOURCES}
        ${AST_GENERATOR_TESTS}
)

add_custom_command(
        TARGET tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_CURRENT_BINARY_DIR}/grammar.txt"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/grammar.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/grammar.txt"
        COMMENT "Copying grammar.txt to binary dir"
)

add_custom_command(
        TARGET compiler PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_CURRENT_BINARY_DIR}/grammar.txt"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/grammar.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/grammar.txt"
        COMMENT "Copying grammar.txt to binary dir"
)

target_link_libraries(tests PUBLIC catch2)
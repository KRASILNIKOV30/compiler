cmake_minimum_required(VERSION 3.26)
project(lexer)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)

add_executable(lexer
        src/main.cpp
        src/rules/Ident.h
        src/Reader.h
        src/rules/Expression.h
        src/rules/Term.h
)

add_executable(lexer_tests
        src/rules/Ident.h
        src/Reader.h
        tests/Reader_tests.cpp
        tests/Ident_tests.cpp
        src/rules/Expression.h
        src/rules/Term.h
        tests/Expression_tests.cpp
)

target_link_libraries(lexer_tests PUBLIC catch2)
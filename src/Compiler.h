#pragma once
#include "guidesBuilder/GuidesBuilder.h"
#include "parser/error/StringifyError.h"
#include "parser/Parser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class Compiler
{
public:
	explicit Compiler(std::string const& grammarFileName)
	{
		std::ifstream input(grammarFileName);
		GuidesBuilder guidesBuilder(input);
		const auto rules = guidesBuilder.BuildGuidedRules();
		TableBuilder tableBuilder(rules);
		auto table = tableBuilder.BuildTable();
		m_parser.LoadTable(std::move(table));
	}

	bool Compile(std::istream const& input, std::ostream& output)
	{
		const auto code = ReadCodeFromFile(input);
		const auto success = m_parser.Parse(code);
		Program program = m_parser.GetProgram();
		CodeGenerator generator;
		program.Generate(generator);
		generator.PrintCode(output);

		return success;
	}

	ErrorReason GetError() const
	{
		return m_parser.GetError();
	}

private:
	static std::string ReadCodeFromFile(std::istream const& input)
	{
		std::stringstream buffer;
		buffer << input.rdbuf();
		return buffer.str();
	}

	Parser m_parser;
};
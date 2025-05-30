#pragma once
#include "guidesBuilder/GuidesBuilder.h"
#include "parser/error/StringifyError.h"
#include "parser/Parser.h"
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

	bool Compile(std::string const& codeLine, std::ostream& output)
	{
		const auto success = m_parser.Parse(codeLine);
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
	Parser m_parser;
};
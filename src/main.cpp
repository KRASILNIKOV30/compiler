#include "guidesBuilder/GuidesBuilder.h"
#include "LLParser/LLParser.h"
#include "LLParser/error/StringifyError.h"
#include "parseRawRules/StringifyRules.h"

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

struct Args
{
	std::string inputFileName;
};

Args ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		throw std::invalid_argument("Incorrect number of arguments");
	}

	return {
		.inputFileName = argv[1]
	};
}

int main(int argc, char* argv[])
{
	try
	{
		const auto [inputFileName] = ParseArgs(argc, argv);
		std::ifstream input(inputFileName);

		GuidesBuilder guidesBuilder(input);
		const auto rules = guidesBuilder.BuildGuidedRules();
		TableBuilder tableBuilder(rules.value());
		const auto table = tableBuilder.BuildTable();
		LLParser parser(table);

		std::string line;
		while (std::getline(std::cin, line))
		{
			if (parser.Parse(line))
			{
				std::cout << "OK" << std::endl;
			}
			else
			{
				std::cout << StringifyError(parser.GetError()) << std::endl;
			}
		}
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

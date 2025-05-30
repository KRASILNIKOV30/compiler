#include "guidesBuilder/GuidesBuilder.h"
#include "parser/error/StringifyError.h"
#include "parser/Parser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

struct Args
{
	std::string grammarFileName;
};

Args ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		throw std::invalid_argument("Incorrect number of arguments");
	}

	return {
		.grammarFileName = argv[1]
	};
}

int main(int argc, char* argv[])
{
	try
	{
		const auto [grammarFileName] = ParseArgs(argc, argv);
		std::ifstream input(grammarFileName);

		GuidesBuilder guidesBuilder(input);
		const auto rules = guidesBuilder.BuildGuidedRules();
		TableBuilder tableBuilder(rules);
		const auto table = tableBuilder.BuildTable();
		Parser parser(table);

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

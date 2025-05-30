#include "Compiler.h"
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
		Compiler compiler(grammarFileName);

		std::string line;
		while (std::getline(std::cin, line))
		{
			std::ofstream output("byte-code.prmbc");
			if (compiler.Compile(line, output))
			{
				std::cout << "OK" << std::endl;
			}
			else
			{
				std::cout << StringifyError(compiler.GetError()) << std::endl;
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

#include "Compiler.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

struct Args
{
	std::string grammarFileName;
	std::string inputFileName;
};

Args ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		throw std::invalid_argument("Incorrect number of arguments");
	}

	return {
		.grammarFileName = argv[1],
		.inputFileName = argv[2]
	};
}

int main(int argc, char* argv[])
{
	try
	{
		const auto byteCodeFile = "byte-code.prmbc";
		const auto [grammarFileName, inputFileName] = ParseArgs(argc, argv);

		std::ifstream input(inputFileName);
		if (!input.is_open())
		{
			throw std::runtime_error("Failed to open file with source code: " + inputFileName);
		}
		std::ofstream output(byteCodeFile);

		Compiler compiler(grammarFileName);
		if (compiler.Compile(input, output))
		{
			std::cout << "OK" << std::endl;
		}
		else
		{
			std::cout << StringifyError(compiler.GetError()) << std::endl;
		}
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

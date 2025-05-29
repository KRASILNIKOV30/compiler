#pragma once
#include "Expression.h"

class Term : Expression
{
	void Generate(CodeGenerator& generator) const override
	{
		size_t pos;
		if (isReference)
		{
			try
			{
				pos = generator.GetVariablePos(value);
				generator.AddInstruction("const" + std::to_string(pos));
			}
			catch (std::invalid_argument const& e)
			{
				throw std::runtime_error(std::format("Failed to generate Term. {}", e.what()));
			}
		}
		else
		{
			pos = generator.GetConstantPosOrAdd(type, value);
			generator.AddInstruction("getlocal " + std::to_string(pos));
		}
		generator.AddInstruction("setlocal " + std::to_string(pos));
	}

	std::string value;
	bool isReference = false;
};
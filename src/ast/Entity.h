#pragma once
#include "../codeGenerator/CodeGenerator.h"


struct Entity
{
	virtual ~Entity() = default;

	virtual void Generate(CodeGenerator& generator) const = 0;
};
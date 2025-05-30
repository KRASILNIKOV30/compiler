#pragma once
#include "../codeGenerator/CodeGenerator.h"
#include <memory>

struct Entity
{
	virtual ~Entity() = default;

	virtual void Generate(CodeGenerator& generator) const = 0;
};

using EntityPtr = std::unique_ptr<Entity>;
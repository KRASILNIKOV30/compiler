#pragma once
#include "../Entity.h"
#include <string>
#include <vector>

struct Declaration : Entity
{
	std::string id;
	Type type;
};

using DeclarationPtr = std::unique_ptr<Declaration>;

#pragma once
#include "declaration/Declaration.h"
#include "statement/Statement.h"
#include <variant>
#include <vector>

struct Program
{
	std::vector<std::variant<Declaration, Statement>> body;
};
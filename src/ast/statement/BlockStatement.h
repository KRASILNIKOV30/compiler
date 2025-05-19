#pragma once
#include "../declaration/Declaration.h"
#include "Statement.h"
#include <variant>
#include <vector>

struct BlockStatement : public Statement
{
	std::vector<std::variant<Declaration, Statement>> body;
};
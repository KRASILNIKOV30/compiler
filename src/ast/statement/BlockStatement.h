#pragma once
#include "Statement.h"
#include "../declaration/Declaration.h"
#include <vector>
#include <variant>

struct BlockStatement : public Statement
{
	std::vector<std::variant<Declaration, Statement>> body;
};
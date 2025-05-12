#pragma once
#include "Statement.h"
#include "../expression/Expression.h"
#include "BlockStatement.h"
#include <optional>

struct IfStatement : public Statement
{
	Expression condition;
	BlockStatement body;
	std::optional<BlockStatement> alternate = std::nullopt;
};
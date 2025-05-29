#pragma once
#include "../Entity.h"
#include "../expression/ExpressionType.h"
#include <string>

struct Declaration : Entity
{
	std::string id;
	std::vector<ExpressionType> type;
};
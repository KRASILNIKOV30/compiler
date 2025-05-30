#pragma once
#include "../Entity.h"

struct Statement : Entity
{
};

using StatementPtr = std::unique_ptr<Statement>;
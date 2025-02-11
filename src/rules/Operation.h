#pragma once
#include <iostream>
#include "Term.h"
#include "../Reader.h"

/**
 * TheFirstLevelOperation -> AND | * | / | MOD | DIV
 */
inline bool TheFirstLevelOperation(Reader& reader)
{
    if (ParseKeyword(reader, "AND") || reader.Peek() == '*' || reader.Peek() == '/' || op == ParseKeyword(reader, "MOD") || ParseKeyword(reader, "DIV"))
    {
        reader.Get();
        return true;
    }
}

/**
 * TheSecondLevelOperation -> + | - | / | OR
 */
inline bool TheSecondLevelOperation(Reader& reader)
{
    if (reader.Peek() == '+' || reader.Peek() == '-' || ParseKeyword(reader, "OR"))
    {
        reader.Get();
        return true;
    }
}

/**
 * TheThirdLevelOperation -> + | - | / | OR
 */
inline bool TheThirdLevelOperation(Reader& reader)
{
    if (ParseKeyword(reader, "==") || ParseKeyword(reader, "!=") || reader.Peek() == '>' || reader.Peek() == '<' || ParseKeyword(reader, ">=") || ParseKeyword(reader, "<="))
    {
        reader.Get();
        return true;
    }
}
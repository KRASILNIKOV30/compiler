#pragma once
#include "Term.h"
#include "../Reader.h"

/**
 * expList -> expression | expListRemainder
 * expListRemainder -> e | , expListRemainder
 */
inline bool ExpressionList(Reader& reader)
{
    return false;
}

inline bool ExpressionListReminder(Reader& reader)
{
    if (reader.Empty())
    {
        return true;
    }

    if (reader.peek() == ",")
    {
        reader.Get();
        ExpressionListReminder(reader);
    }

    return true;
}

inline bool Expression(Reader& reader)
{
    return false;
}
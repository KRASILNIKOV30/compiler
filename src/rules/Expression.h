#pragma once
#include <string>
#include "../Reader.h"
#include "Term.h"

bool Term(Reader& reader);

inline bool Rel(Reader& reader)
{
    return ParseKeyword(reader, "==") || ParseKeyword(reader, "!=") ||
           ParseKeyword(reader, ">=") || ParseKeyword(reader, "<=") ||
           ParseKeyword(reader, ">") || ParseKeyword(reader, "<");
}

inline bool PLUSO(Reader& reader)
{
    return ParseKeyword(reader, "+") || ParseKeyword(reader, "-") ||
           ParseKeyword(reader, "or");
}

inline bool MULO(Reader& reader)
{
    return ParseKeyword(reader, "and") || ParseKeyword(reader, "*") ||
           ParseKeyword(reader, "/") || ParseKeyword(reader, "mod") ||
           ParseKeyword(reader, "div");
}

inline bool SimTermPrime(Reader& reader)
{
    auto pos = reader.Count();
    if (MULO(reader))
    {
        if (!Term(reader))
        {
            reader.Seek(pos);
            return false;  // Оператор без операнда — ошибка
        }
        return SimTermPrime(reader);
    }
    return true;
}

inline bool SimTerm(Reader& reader)
{
    auto pos = reader.Count();
    if (Term(reader))
    {
        return SimTermPrime(reader);
    }
    reader.Seek(pos);
    return false;
}

inline bool SimExpPrime(Reader& reader)
{
    auto pos = reader.Count();
    if (PLUSO(reader))
    {
        if (!SimTerm(reader))
        {
            reader.Seek(pos);
            return false;  // Оператор без операнда — ошибка
        }
        return SimExpPrime(reader);
    }
    return true;
}

inline bool SimExp(Reader& reader)
{
    if (SimTerm(reader))
    {
        return SimExpPrime(reader);
    }
    return false;
}

inline bool ExpPrime(Reader& reader)
{
    auto pos = reader.Count();
    if (Rel(reader))
    {
        if (!SimExp(reader))
        {
            reader.Seek(pos);
            return false;  // Оператор без операнда — ошибка
        }
        return ExpPrime(reader);
    }
    return true;
}

inline bool Expression(Reader& reader)
{
    auto pos = reader.Count();
    if (SimExp(reader))
    {
        return ExpPrime(reader);
    }
    return false;
}

/**
 * expList -> expression | expListRemainder
 * expListRemainder -> e | , expListRemainder
 */
inline bool ExpressionList(Reader& reader)
{
    auto pos = reader.Count();

    if (Expression(reader))
    {
        if (reader.Peek() == ',')
        {
            reader.Get();
            return ExpressionList(reader);
        }
        return true;
    }

    reader.Seek(pos);
    return false;
}
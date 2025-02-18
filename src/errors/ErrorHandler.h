#include "Error.h"
#include <iostream>

class IErrorHandler
{
public:
    ~IErrorHandler() {};
    virtual void ProcessError(Error error) const;
};

class StreamErrorHandler
{
public:
    StreamErrorHandler(std::ostream& out)
        : m_out(out)
    {}

    void ProcessError(Error error) const
    {
        switch (error)
    {
        case Error::NONE:
            m_out <<  "No error occurred." << std::endl;
        case Error::UNKNOWN_SYMBOL:
            m_out <<  "Unknown symbol encountered." << std::endl;
        case Error::INVALID_NUMBER:
            m_out <<  "Invalid number format." << std::endl;
        case Error::STRING_LITERAL_INCOMPLETE:
            m_out <<  "String literal is incomplete." << std::endl;
        case Error::EMPTY_INPUT:
            m_out <<  "Input is empty." << std::endl;
        case Error::INVALID_ID:
            m_out <<  "Invalid identifier." << std::endl;
        case Error::EXP_EXPECTED:
            m_out <<  "Expression expected." << std::endl;
        case Error::INVALID_RELATION:
            m_out <<  "Invalid relation operator." << std::endl;
        case Error::TERM_EXPECTED:
            m_out <<  "Term expected." << std::endl;
        case Error::PARAN_CLOSE_EXPECTED:
            m_out <<  "Closing parenthesis expected." << std::endl;
        case Error::BRACKET_CLOSE_EXPECTED:
            m_out <<  "Closing bracket expected." << std::endl;
        default:
            m_out <<  "Unknown error." << std::endl;
    }
    }

private:
    std::ostream& m_out;
};
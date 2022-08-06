
#include "exceptions.h"


LogFormatException::LogFormatException(const std::string& msg )
{
    this->msg = msg;
}

const std::string& LogFormatException::what()
{
    return msg;
}

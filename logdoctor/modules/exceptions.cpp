
#include "exceptions.h"


/////////////////
//// GENERIC ////
GenericException::GenericException(const std::string& msg )
{
    this->msg = msg;
}
const std::string& GenericException::what()
{
    return msg;
}


////////////////////
//// WEB SERVER ////
WebServerException::WebServerException(const std::string& msg )
{
    this->msg = msg;
}
const std::string& WebServerException::what()
{
    return msg;
}


////////////////////
//// LOG FORMAT ////
LogFormatException::LogFormatException(const std::string& msg )
{
    this->msg = msg;
}
const std::string& LogFormatException::what()
{
    return msg;
}


////////////////////
//// LOG PARSER ////
LogParserException::LogParserException(const std::string& msg )
{
    this->msg = msg;
}
const std::string& LogParserException::what()
{
    return msg;
}


///////////////////
//// DATE-TIME ////
DateTimeException::DateTimeException(const std::string& msg )
{
    this->msg = msg;
}
const std::string& DateTimeException::what()
{
    return msg;
}

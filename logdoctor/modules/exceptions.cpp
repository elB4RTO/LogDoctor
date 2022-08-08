
#include "exceptions.h"


/////////////////
//// GENERIC ////
GenericException::GenericException(const std::string& msg )
{
    this->msg = QString::fromStdString( msg );
}
const QString& GenericException::what()
{
    return msg;
}


////////////////////
//// WEB SERVER ////
WebServerException::WebServerException(const std::string& msg )
{
    this->msg = QString::fromStdString( msg );
}
const QString& WebServerException::what()
{
    return msg;
}


////////////////////
//// LOG FORMAT ////
LogFormatException::LogFormatException(const std::string& msg )
{
    this->msg = QString::fromStdString( msg );
}
const QString& LogFormatException::what()
{
    return msg;
}


////////////////////
//// LOG PARSER ////
LogParserException::LogParserException(const std::string& msg )
{
    this->msg = QString::fromStdString( msg );
}
const QString& LogParserException::what()
{
    return msg;
}


///////////////////
//// DATE-TIME ////
DateTimeException::DateTimeException(const std::string& msg )
{
    this->msg = QString::fromStdString( msg );
}
const QString& DateTimeException::what()
{
    return msg;
}

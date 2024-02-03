
#include "exceptions.h"

#include <iostream>

#include <QString>


DoNotCatchException::DoNotCatchException( const char* msg )
{
    std::cerr << "LogDoctor: Exception: " << msg << std::endl;
}


/////////////////
//// GENERIC ////
GenericException::GenericException( const std::string& msg, const bool to_sys )
{
    if ( to_sys ) { // when sys, leave un-catched
        std::cerr << "LogDoctor: GenericException: " << msg << std::endl;
    } else {
        this->msg = QString::fromStdString( msg );
    }
}
const QString& GenericException::what()
{
    return msg;
}


////////////////////
//// WEB SERVER ////
WebServerException::WebServerException( const std::string& msg ) // leave un-catched
{
    std::cerr << "LogDoctor: WebServerException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& WebServerException::what()
{
    return msg;
}*/


////////////////////
//// LOG FORMAT ////
LogFormatException::LogFormatException( const std::string& msg )
{
    std::cerr << "LogDoctor: LogFormatException: " << msg << std::endl;
    this->msg = QString::fromStdString( msg );
}
const QString& LogFormatException::what()
{
    return msg;
}


////////////////////
//// LOG PARSER ////
LogParserException::LogParserException( const std::string& txt , const std::string& val )
{
    std::cerr << "LogDoctor: LogParserException: " << txt << ": '" << val << "'" << std::endl;
    this->msg = QString("%1:\n'%2'").arg(
        QString::fromStdString( txt ),
        QString::fromStdString( val ) );
}
const QString& LogParserException::what()
{
    return msg;
}


///////////////////
//// DATE-TIME ////
DateTimeException::DateTimeException( const std::string& msg ) // leave un-catched
{
    std::cerr << "LogDoctor: DateTimeException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& DateTimeException::what()
{
    return msg;
}*/


////////////////////
//// CONVERSION ////
ConversionException::ConversionException( const std::string& msg )
{
    //std::cerr << "LogDoctor: ConversionException: " << msg << std::endl;
    this->msg = QString::fromStdString( msg );
}
const QString& ConversionException::what()
{
    return msg;
}


//////////////////////////
//// BLACK/WARN LISTS ////
BWlistException::BWlistException( const std::string& msg )
{
    std::cerr << "LogDoctor: BWlistException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& DateTimeException::what()
{
    return msg;
}*/


//////////////////
//// DATABASE ////
DatabaseException::DatabaseException( QString&& msg )
    : msg{ std::move(msg) }
{

}
const QString& DatabaseException::what() const noexcept
{
    return msg;
}

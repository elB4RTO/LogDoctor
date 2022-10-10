
#include "exceptions.h"

#include <iostream>


/////////////////
//// GENERIC ////
GenericException::GenericException( const std::string& msg, const bool& to_sys )
{
    if ( to_sys ) { // when sys, leave un-catched
        std::cout << "LogDoctor: Exception: " << msg << std::endl;
        std::cerr << "LogDoctor: Exception: " << msg << std::endl;
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
WebServerException::WebServerException(const std::string& msg ) // leave un-catched
{
    std::cout << "LogDoctor: WebServerException: " << msg << std::endl;
    std::cerr << "LogDoctor: WebServerException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& WebServerException::what()
{
    return msg;
}*/


////////////////////
//// LOG FORMAT ////
LogFormatException::LogFormatException(const std::string& msg )
{
    std::cout << "LogDoctor: LogFormatException: " << msg << std::endl;
    std::cerr << "LogDoctor: LogFormatException: " << msg << std::endl;
    this->msg = QString::fromStdString( msg );
}
const QString& LogFormatException::what()
{
    return msg;
}


////////////////////
//// LOG PARSER ////
LogParserException::LogParserException(const std::string& msg ) // un-catched atm
{
    std::cout << "LogDoctor: LogParserException: " << msg << std::endl;
    std::cerr << "LogDoctor: LogParserException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& LogParserException::what()
{
    return msg;
}*/


///////////////////
//// DATE-TIME ////
DateTimeException::DateTimeException(const std::string& msg ) // leave un-catched
{
    std::cout << "LogDoctor: DateTimeException: " << msg << std::endl;
    std::cerr << "LogDoctor: DateTimeException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& DateTimeException::what()
{
    return msg;
}*/


//////////////////////////
//// BLACK/WARN LISTS ////
BWlistException::BWlistException( const std::string& msg )
{
    std::cout << "LogDoctor: BWlistException: " << msg << std::endl;
    std::cerr << "LogDoctor: BWlistException: " << msg << std::endl;
    /*this->msg = QString::fromStdString( msg );*/
}
/*const QString& DateTimeException::what()
{
    return msg;
}*/

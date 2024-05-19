
#include "exceptions.h"

#include <iostream>


DoNotCatchException::DoNotCatchException( const char* header, const std::string& body )
{
    std::cerr << "LogDoctor: Exception: " << header << ": " << body << std::endl;
}


/////////////////
//// GENERIC ////
GenericException::GenericException( const std::string& msg )
    : msg{ QString::fromStdString( msg ) }
{
}
const QString& GenericException::what()
{
    return msg;
}


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
    this->msg = QStringLiteral("%1:\n'%2'").arg(
        QString::fromStdString( txt ),
        QString::fromStdString( val ) );
}
const QString& LogParserException::what()
{
    return msg;
}


///////////////////
//// DATE-TIME ////
DateTimeException::DateTimeException( const char* header, const std::string& body )
{
    std::cerr << "LogDoctor: DateTimeException: " << header << ": " << body << std::endl;
}


//////////////////////////
//// BLACK/WARN LISTS ////
BWlistException::BWlistException( const std::string& msg )
{
    std::cerr << "LogDoctor: BWlistException: " << msg << std::endl;
}


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


///////////////
//// PATHS ////
InvalidPathException::InvalidPathException( QStringList&& msg )
    : msg{ std::move(msg) }
{
}
const QStringList& InvalidPathException::what() const noexcept
{
    return msg;
}

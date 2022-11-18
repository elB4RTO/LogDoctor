#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>

#include <exception>


//! GenericException
/*!
    Generic exception for general purposes
*/
class GenericException : public std::exception {
public:
    GenericException( const std::string& msg , const bool& to_sys=false );
    const QString& what();

private:
    QString msg;
};


//! WebServerException
/*!
    Exception related to a Web Server
*/
class WebServerException : public std::exception {
public:
    WebServerException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;*/
};


//! LogFormatException
/*!
    Exception related to a Logs Format
*/
class LogFormatException : public std::exception {
public:
    LogFormatException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


//! LogParserException
/*!
    Exception related to the logs parser
*/
class LogParserException : public std::exception {
public:
    LogParserException( const std::string& txt, const std::string& val );
    const QString& what();

private:
    QString msg;
};


//! DateTimeException
/*!
    Exception related to a date/time
*/
class DateTimeException : public std::exception {
public:
    DateTimeException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;*/
};


//! BWlistException
/*!
    Exception related to a blacklist/warnlist
*/
class BWlistException : public std::exception {
public:
    BWlistException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;*/
};

#endif // EXCEPTIONS_H

#ifndef LOGDOCTOR__EXCEPTIONS_H
#define LOGDOCTOR__EXCEPTIONS_H


#include <QStringList>


//! VoidException
/*!
    Base class for some internal exceptions.
    Used when a message has already been shown,
    or there is no need to show one.
*/
class VoidException {};


//! DoNotCatchException
/*!
    Thrown when something really unexptional happens.
    It's not supposed to be catched.
*/
struct DoNotCatchException final
{
    explicit DoNotCatchException( const char* header, const std::string& body );
};


//! LogDoctorException
/*!
    Base class for internal exceptions
*/
class LogDoctorException {};


//! GenericException
/*!
    Generic exception for general purposes
*/
class GenericException final : public LogDoctorException {
public:
    explicit GenericException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


//! LogFormatException
/*!
    Exception related to a Logs Format
*/
class LogFormatException final : public LogDoctorException {
public:
    explicit LogFormatException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


//! LogParserException
/*!
    Exception related to the logs parser
*/
class LogParserException final : public LogDoctorException {
public:
    explicit LogParserException( const std::string& txt, const std::string& val );
    const QString& what();

private:
    QString msg;
};


//! DateTimeException
/*!
    Exception related to a date/time
*/
class DateTimeException final : public LogDoctorException {
public:
    explicit DateTimeException( const char* header, const std::string& body );
};


//! BWlistException
/*!
    Exception related to a blacklist/warnlist
*/
class BWlistException final : public LogDoctorException {
public:
    explicit BWlistException( const std::string& msg );
};


//! DatabaseException
/*!
    Exception related to the database
*/
class DatabaseException final : public LogDoctorException {
public:
    explicit DatabaseException( QString&& msg );
    const QString& what() const noexcept;

private:
    QString msg;
};


//! InvalidPathException
/*!
    Exception related to a path
*/
class InvalidPathException final : public LogDoctorException {
public:
    explicit InvalidPathException( QStringList&& msg );
    const QStringList& what() const noexcept;

private:
    QStringList msg;
};


#endif // LOGDOCTOR__EXCEPTIONS_H

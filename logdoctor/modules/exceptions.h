#ifndef LOGDOCTOR__EXCEPTIONS_H
#define LOGDOCTOR__EXCEPTIONS_H


#include <QString>

#include <exception>


//! LogDoctorException
/*!
    Base class for some internal exceptions.
    Used on its own when a message has already been shown,
    or there is no need to show one.
    \see CrapviewException, DatabaseException
 */
class LogDoctorException {};


struct DoNotCatchException final
{
    explicit DoNotCatchException( const std::string& msg );
};


//! GenericException
/*!
    Generic exception for general purposes
*/
class GenericException final : public std::exception {
public:
    explicit GenericException( const std::string& msg, const bool to_sys=false );
    const QString& what();

private:
    QString msg;
    using std::exception::what;
};


//! WebServerException
/*!
    Exception related to a Web Server
*/
class WebServerException final : public std::exception {
public:
    explicit WebServerException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;
    using std::exception::what;*/
};


//! LogFormatException
/*!
    Exception related to a Logs Format
*/
class LogFormatException final : public std::exception {
public:
    explicit LogFormatException( const std::string& msg );
    const QString& what();

private:
    QString msg;
    using std::exception::what;
};


//! LogParserException
/*!
    Exception related to the logs parser
*/
class LogParserException final : public std::exception {
public:
    explicit LogParserException( const std::string& txt, const std::string& val );
    const QString& what();

private:
    QString msg;
    using std::exception::what;
};


//! DateTimeException
/*!
    Exception related to a date/time
*/
class DateTimeException final : public std::exception {
public:
    explicit DateTimeException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;
    using std::exception::what;*/
};


//! ConversionException
/*!
    Exception related to failure in converting something
*/
class ConversionException final : public std::exception {
public:
    explicit ConversionException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


//! BWlistException
/*!
    Exception related to a blacklist/warnlist
*/
class BWlistException final : public std::exception {
public:
    explicit BWlistException( const std::string& msg );
    /*const QString& what();

private:
    QString msg;
    using std::exception::what;*/
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


#endif // LOGDOCTOR__EXCEPTIONS_H

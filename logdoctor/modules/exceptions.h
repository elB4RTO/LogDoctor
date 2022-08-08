#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>

#include <exception>


class GenericException : public std::exception {
public:
    GenericException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


class WebServerException : public std::exception {
public:
    WebServerException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


class LogFormatException : public std::exception {
public:
    LogFormatException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


class LogParserException : public std::exception {
public:
    LogParserException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};


class DateTimeException : public std::exception {
public:
    DateTimeException( const std::string& msg );
    const QString& what();

private:
    QString msg;
};

#endif // EXCEPTIONS_H

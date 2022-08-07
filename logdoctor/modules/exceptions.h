#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>


class GenericException : public std::exception {
public:
    GenericException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};


class WebServerException : public std::exception {
public:
    WebServerException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};


class LogFormatException : public std::exception {
public:
    LogFormatException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};


class LogParserException : public std::exception {
public:
    LogParserException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};


class DateTimeException : public std::exception {
public:
    DateTimeException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};

#endif // EXCEPTIONS_H

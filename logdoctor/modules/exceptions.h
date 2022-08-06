#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>


class LogFormatException : public std::exception {
public:
    LogFormatException( const std::string& msg );
    const std::string& what();

private:
    std::string msg;
};

#endif // EXCEPTIONS_H

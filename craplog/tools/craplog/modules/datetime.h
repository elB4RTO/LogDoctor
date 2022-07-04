#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <vector>


class DateTimeOps
{
public:
    DateTimeOps();

    static std::vector<std::string> processDateTime( const std::string& datetime, const std::string& format );

private:

    static std::string convertMonth( const std::string& month );

};

#endif // DATETIME_H

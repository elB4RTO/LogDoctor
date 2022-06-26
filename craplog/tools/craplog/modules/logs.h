#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>


class LogOps
{
public:
    LogOps();

    std::vector<std::string> splitLine( std::string line, int type );
    std::vector<std::string> splitLines( std::string line, int type );

private:
    // types constants
    const unsigned int ACCESS_LOGS=1, ERROR_LOGS=2;

};

#endif // LOGS_H

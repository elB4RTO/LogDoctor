#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "utilities/strings.h"
#include "tools/craplog/modules/formats.h"


class LogOps
{
public:
    LogOps();

    // log file types
    enum LogType {
        Failed=0,
        Access=1,
        Error=2
    };

    // define if really access and/or error logs
    LogType defineFileType( std::string name, std::vector<std::string> lines, std::unordered_map<int, FormatOps::LogsFormat> format );

    std::vector<std::string> splitLine( std::string line, int type );
    std::vector<std::string> splitLines( std::string line, int type );

private:
    // types constants
    const unsigned int ACCESS_LOGS=1, ERROR_LOGS=2;

};

#endif // LOGS_H

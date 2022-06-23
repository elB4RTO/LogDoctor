#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>

#include "tools/craplog.h"

class LogOp
{
public:
    LogOp();

    static bool isNameValid( std::string name );

    static Craplog::LogType defineLogType( std::vector<std::string> lines );

    static std::vector<std::string> splitLine( std::string line, Craplog::LogType type );
    static std::vector<std::string> splitLines( std::string line, Craplog::LogType type );
};

#endif // LOGOP_H

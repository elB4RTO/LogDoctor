#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>

#include "tools/craplog.h"

class LogOp
{
public:
    LogOp();
    
    Craplog::LogType defineLogType( std::string line );
    
    std::vector<std::string> splitLine( std::string line );    
    std::vector<std::string> splitLines( std::string line );
};

#endif // LOGOP_H

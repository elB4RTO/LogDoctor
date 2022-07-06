#ifndef CHECKS_H
#define CHECKS_H

#include "tools/craplog/craplog.h"


class CheckSec
{
public:
    CheckSec();

    static bool checkStatsDatabase( std::string path );
    static bool checkHashesDatabase( std::string path );

    static bool checkCraplog( Craplog craplog );
};

#endif // CHECKS_H

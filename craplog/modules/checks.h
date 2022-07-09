#ifndef CHECKS_H
#define CHECKS_H

#include "tools/craplog/craplog.h"


class CheckSec
{
public:
    CheckSec();

    static bool checkStatsDatabase( const std::string& db_path );
    static bool checkHashesDatabase( const std::string& db_path );

    static bool checkCraplog( const Craplog& craplog );


private:
    static int checkDatabaseTablesNames( QSqlDatabase& db, const std::string& db_path, const QString& db_name );
    static bool newStatsDatabase( QSqlDatabase& db, const std::string& db_path, const QString& db_name, const std::vector<QString>& ws_names );
    static bool newHashesDatabase( QSqlDatabase& db, const std::string& db_path, const QString& db_name, const std::vector<QString>& ws_names );
};

#endif // CHECKS_H

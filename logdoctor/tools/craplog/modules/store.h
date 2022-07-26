#ifndef STORE_H
#define STORE_H

#include "tools/craplog/craplog.h"


class StoreOps
{
public:
    StoreOps();

    static bool storeData( QSqlDatabase& db, Craplog& caplog, const std::vector<std::unordered_map<int, std::string>>& data, const int log_type );
};

#endif // STORE_H

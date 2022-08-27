#ifndef STORE_H
#define STORE_H

#include "modules/craplog/craplog.h"


class StoreOps
{
public:
    StoreOps();

    static bool storeData( QSqlDatabase& db, Craplog& caplog, const std::vector<std::unordered_map<int, std::string>>& data );
};

#endif // STORE_H

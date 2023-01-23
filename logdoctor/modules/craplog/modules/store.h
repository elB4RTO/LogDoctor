#ifndef STORE_H
#define STORE_H

#include "modules/craplog/craplog.h"


//! StoreOps
/*!
    Operations to store logs data
*/
class StoreOps
{
public:
    explicit StoreOps();

    //! Stores the data collection in the logs Collection database
    /*!
        \param db A database instance, already initizlized
        \param craplog The Craplog instance
        \param data The logs data collection
        \return Whether the operation has been successful or not
        \throw WebServerException
    */
    static bool storeData( QSqlDatabase& db, Craplog& craplog, const std::vector<std::unordered_map<int, std::string>>& data );
};

#endif // STORE_H

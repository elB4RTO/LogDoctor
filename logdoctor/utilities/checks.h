#ifndef CHECKS_H
#define CHECKS_H

#include <string>


//! CheckSec
/*!
    Utilities used to check the databases
*/
namespace CheckSec
{

//! Checks the structure's integrity of the Collection database
/*!
  \param db_path The path of the database, file included
  \return The result of the check
  \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
*/
bool checkCollectionDatabase( const std::string& db_path );

//! Checks the structure's integrity of the Hashes database
/*!
  \param db_path The path of the database, file included
  \return The result of the check
  \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
*/
bool checkHashesDatabase( const std::string& db_path );

} // namespace CheckSec

#endif // CHECKS_H

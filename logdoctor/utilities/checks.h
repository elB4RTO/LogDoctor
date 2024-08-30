#ifndef LOGDOCTOR__UTILITIES__CHECKS_H
#define LOGDOCTOR__UTILITIES__CHECKS_H


#include <string>

class PathHandler;

class QString;


//! CheckSec
/*!
    Utilities used to check the databases
*/
namespace CheckSec
{

//! Basic checks on a database file
/*!
  \param db_path Path of the database file
  \param db_name Database's name, used by the dialogs if necessary
  \return The result of the check
  \see IOutils::exists(), IOutils::isFile(), IOutils::checkFile()
*/
bool checkDatabaseFile( const PathHandler& db_path, const QString& db_name ) noexcept;

//! Checks the structure's integrity of the Collection database
/*!
  \param db_path The path of the database, file included
  \return The result of the check
  \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
*/
bool checkCollectionDatabase( const PathHandler& db_path ) noexcept;

//! Checks the structure's integrity of the Hashes database
/*!
  \param db_path The path of the database, file included
  \return The result of the check
  \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
*/
bool checkHashesDatabase( const PathHandler& db_path ) noexcept;

} // namespace CheckSec


#endif // LOGDOCTOR__UTILITIES__CHECKS_H

#ifndef CHECKS_H
#define CHECKS_H

#include <QString>
#include <QSqlDatabase>

#include <string>
#include <vector>


//! CheckSec
/*!
    Utilities used to check the databases
*/
class CheckSec
{

public:
    CheckSec();

    //! Checks the structure's integrity of the Collection database
    /*!
      \param db_path The path of the database, file included
      \return The result of the check
      \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
    */
    static bool checkCollectionDatabase( const std::string& db_path );

    //! Checks the structure's integrity of the Hashes database
    /*!
      \param db_path The path of the database, file included
      \return The result of the check
      \see checkDatabaseTablesNames(), newCollectionDatabase(), newHashesDatabase()
    */
    static bool checkHashesDatabase( const std::string& db_path );


private:

    //! Checks the tables' names integrity
    /*!
      \param db Database object, already initialized
      \param db_name Database's name, eventually used by the dialogs
      \return The result of the check: 0 if failed with an error, 1 if all the integrity checks passed, 2 if a rebuild is needed
      \see checkCollectionDatabase(), checkHashesDatabase(), newCollectionDatabase(), newHashesDatabase()
    */
    static int checkDatabaseTablesNames( QSqlDatabase& db, const QString& db_name );

    //! Builds a new database for the logs Collection
    /*!
      \param db Database object, already initialized
      \param db_name Database's name, eventually used by the dialogs
      \return The result of the operation
      \see checkCollectionDatabase(), checkHashesDatabase()
    */
    static bool newCollectionDatabase( QSqlDatabase& db, const QString& db_name, const std::vector<QString>& ws_names );

    //! Builds a new database for the used log files' Hashes
    /*!
      \param db Database object, already initialized
      \param db_name Database's name, eventually used by the dialogs
      \return The result of the operation
      \see checkCollectionDatabase(), checkHashesDatabase()
    */
    static bool newHashesDatabase( QSqlDatabase& db, const QString& db_name, const std::vector<QString>& ws_names );
};

#endif // CHECKS_H

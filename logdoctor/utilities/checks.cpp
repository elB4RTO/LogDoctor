
#include "checks.h"

#include "globals/db_names.h"

#include "modules/dialogs.h"

#include "modules/database/database.h"

#include "utilities/io.h"

#include <QSqlError>
#include <QVariant>

#include <vector>
#include <unordered_map>


struct MakeNewDatabase {};


namespace CheckSec
{

namespace /*private*/
{

//! Checks the tables' names integrity
/*!
    \param query Query instance from the target database
    \return Whether the database is valid or not
    \throw VoidException
    \throw MakeNewDatabase
    \see checkCollectionDatabase(), checkHashesDatabase(), newCollectionDatabase(), newHashesDatabase()
*/
bool checkDatabaseTablesNames( QueryWrapper query )
{
    query( QStringLiteral("SELECT name FROM sqlite_schema WHERE type = 'table';") );

    std::unordered_map<QString, bool> checks{
        {"apache", false},
        {"nginx",  false},
        {"iis",    false} };

    while ( query->next() ) {

        const QString table{ query[0].toString() };

        if ( const auto tbl{ checks.find( table ) }; tbl != checks.end() ) {
            tbl->second |= true;
        } else {
            // unexpected table name
            if ( DialogSec::choiceDatabaseWrongTable( query.dbName(), table ) ) {
                // agreed to renew
                throw MakeNewDatabase{};
            } else {
                // refused to renew
                return false;
            }
        }
    }

    for ( const auto& [tbl,res] : checks ) {
        if ( ! res ) {
            // a table has not been found
            if ( DialogSec::choiceDatabaseMissingTable( query.dbName(), tbl ) ) {
                // agreed to renew
                throw MakeNewDatabase{};
            } else {
                // refused to renew
                return false;
            }
        }
    }

    return true;
}


//! Builds a new database for the logs Collection
/*!
    \param db Database object
    \param db_path The database file's path
    \param ws_names Database's tables names
    \return The result of the operation
    \see checkCollectionDatabase(), checkHashesDatabase()
*/
bool newCollectionDatabase( DatabaseWrapper db, const std::string& db_path, const std::vector<QString>& ws_names ) noexcept
{
    try {

        db.openNew( db_path );

        // succesfully creted database file, now create the tables
        const QString stmt{ QStringLiteral(R"(
            CREATE TABLE "%1" (
                "year" SMALLINT,
                "month" TINYINT,
                "day" TINYINT,
                "hour" TINYINT,
                "minute" TINYINT,
                "second" TINYINT,
                "protocol" TEXT,
                "method" TEXT,
                "uri" TEXT,
                "query" TEXT,
                "response" SMALLINT,
                "time_taken" INTEGER,
                "bytes_sent" INTEGER,
                "bytes_received" INTEGER,
                "client" TEXT,
                "user_agent" TEXT,
                "cookie" TEXT,
                "referrer" TEXT
            );)")};

        for ( const QString& ws_name : ws_names ) {

            QueryWrapper query{ db.getQuery() };

            if ( ! query->exec( stmt.arg( ws_name ) ) ) {
                // error creating table
                DialogSec::errDatabaseFailedExecuting(
                    db.name(),
                    QStringLiteral(R"(CREATE TABLE "%1" (...))").arg( ws_name ),
                    query->lastError().text() );
                throw VoidException{};
            }
        }

    } catch (...) {
        DialogSec::errDatabaseFailedCreating( db.name() );
        return false;
    }

    DialogSec::msgDatabaseCreated( db.name() );
    return checkCollectionDatabase( db_path );
}


//! Builds a new database for the used log files' Hashes
/*!
    \param db Database object, already initialized
    \param db_path The database file's path
    \param ws_names Database's tables names
    \return The result of the operation
    \see checkCollectionDatabase(), checkHashesDatabase()
*/
bool newHashesDatabase( DatabaseWrapper db, const std::string& db_path, const std::vector<QString>& ws_names ) noexcept
{
    try {

        db.openNew( db_path );

        // succesfully creted database file, now create the tables
        const QString stmt{ QStringLiteral(R"(
            CREATE TABLE "%1" (
                "hash" TEXT
            );)")};

        for ( const QString& ws_name : ws_names ) {

            QueryWrapper query{ db.getQuery() };

            if ( ! query->exec( stmt.arg( ws_name ) ) ) {
                // error creating table

                DialogSec::errDatabaseFailedExecuting(
                    db.name(),
                    QStringLiteral(R"(CREATE TABLE "%1" (...))").arg( ws_name ),
                    query->lastError().text() );
                throw VoidException{};
            }
        }

    } catch (...) {
        DialogSec::errDatabaseFailedCreating( db.name() );
        return false;
    }

    DialogSec::msgDatabaseCreated( db.name() );
    return checkHashesDatabase( db_path );
}

} // namespace (private)


bool checkCollectionDatabase( const std::string& db_path ) noexcept
{
    const std::vector<QString> ws_names{ "apache", "nginx", "iis" };

    try {

        DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data ) };

        if ( ! IOutils::exists( db_path ) ) {
            // ask to create a new one
            if ( DialogSec::choiceDatabaseNotFound( db.name() ) ) {
                // choosed to create it
                throw MakeNewDatabase{};
            } else {
                // refused to create it, abort
                return false;
            }
        }

        // check file type and permissions
        if ( ! checkDatabaseFile( db_path, db.name() ) ) {
            return false;
        }

        // file seems ok, try to open
        db.open( db_path, true );

        // check the tables
        if ( ! checkDatabaseTablesNames( db.getQuery() ) ) {
            return false;
        }

        const QString stmt{ QStringLiteral("SELECT name, type FROM pragma_table_info('%1') AS tbinfo;") };

        // check every WebServer table
        for ( const QString& table : ws_names ) {

            bool has_warning_column{ false };
            // column's name:type associations
            std::unordered_map<QString, std::tuple<QString, bool>> checks {
                          {"year", { "SMALLINT", false} },
                         {"month", { "TINYINT",  false} },
                           {"day", { "TINYINT",  false} },
                          {"hour", { "TINYINT",  false} },
                        {"minute", { "TINYINT",  false} },
                        {"second", { "TINYINT",  false} },
                      {"protocol", { "TEXT",     false} },
                        {"method", { "TEXT",     false} },
                           {"uri", { "TEXT",     false} },
                         {"query", { "TEXT",     false} },
                      {"response", { "SMALLINT", false} },
                    {"time_taken", { "INTEGER",  false} },
                    {"bytes_sent", { "INTEGER",  false} },
                {"bytes_received", { "INTEGER",  false} },
                        {"client", { "TEXT",     false} },
                    {"user_agent", { "TEXT",     false} },
                        {"cookie", { "TEXT",     false} },
                      {"referrer", { "TEXT",     false} }
            };

            QueryWrapper query{ db.getQuery() };

            query( stmt.arg( table ) );

            while ( query->next() ) {
                const QString col_name{ query[0].toString() };
                const QString col_type{ query[1].toString() };

                if ( col_name == "warning" ) {
                    // provide backward compatibility, this column will be removed from the table
                    has_warning_column |= true;

                } else if ( const auto it{ checks.find( col_name ) }; it != checks.end() ) {
                    // column found, check the data-type
                    auto& type{ it->second };
                    if ( col_type == std::get<0>( type ) ) {
                        // same data-type
                        std::get<1>( type ) |= true;
                    } else {
                        // different data-type, ask to renew
                        if ( DialogSec::choiceDatabaseWrongDataType( db.name(), table, col_name, col_type ) ) {
                            // agreed to renew
                            throw MakeNewDatabase{};
                        } else {
                            // refused to renew
                            return false;
                        }
                    }

                } else {
                    // unexpected column
                    if ( DialogSec::choiceDatabaseWrongColumn( db.name(), table, col_name ) ) {
                        // agreed to renew
                        throw MakeNewDatabase{};
                    } else {
                        // refused to renew
                        return false;
                    }
                }
            }

            if ( has_warning_column ) {
                // provide backward compatibility
                query->finish();
                query( QStringLiteral(R"(ALTER TABLE "%1" DROP COLUMN "warning";)").arg( table ) );
            }

            for ( const auto& [col,type] : checks ) {
                if ( ! std::get<1>( type ) ) {
                    // a column has not been found
                    if ( DialogSec::choiceDatabaseMissingColumn( db.name(), table, col ) ) {
                        // agreed to renew
                        throw MakeNewDatabase{};
                    } else {
                        // refused to renew
                        return false;
                    }
                }
            }
        }

    } catch (const MakeNewDatabase&) {

        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            std::error_code err;
            if ( ! IOutils::renameAsCopy( db_path, err ) ) {
                // failed to rename
                QString err_msg;
                if ( err ) {
                    err_msg = QString::fromStdString( err.message() );
                }
                DialogSec::errRenaming( QString::fromStdString(db_path), err_msg );
                return false;
            }
        }
        return newCollectionDatabase( DatabaseHandler::get( DatabaseType::Data ), db_path, ws_names );

    } catch (const VoidException&) {
        return false;
    }

    return true;
}


bool checkHashesDatabase( const std::string& db_path ) noexcept
{
    const std::vector<QString> ws_names { "apache", "nginx", "iis" };

    try {

        DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Hashes ) };

        if ( ! IOutils::exists( db_path ) ) {
            // database does not exist, yet, ask to create a new one
            if ( DialogSec::choiceDatabaseNotFound( db.name() ) ) {
                // choosed to create it
                throw MakeNewDatabase{};
            } else {
                // refused to create it, abort
                return false;
            }
        }

        // check file type and permissions
        if ( ! checkDatabaseFile( db_path, db.name() ) ) {
            return false;
        }

        // file seems ok, try to open
        db.open( db_path, true );

        // check the tables
        if ( ! checkDatabaseTablesNames( db.getQuery() ) ) {
            return false;
        }

        const QString stmt{ QStringLiteral("SELECT name, type FROM pragma_table_info('%1') AS tbinfo;") };

        // check every WebServer table, both access and error
        for ( const QString& table : ws_names ) {

            QueryWrapper query{ db.getQuery() };

            query( stmt.arg( table ) );

            while ( query->next() ) {
                const QString col_name{ query[0].toString() };
                const QString col_type{ query[1].toString() };

                if ( col_name != "hash" ) {
                    // unexpected column
                    if ( DialogSec::choiceDatabaseWrongColumn( db.name(), table, col_name ) ) {
                        // agreed to renew
                        throw MakeNewDatabase{};
                    } else {
                        // refused to renew
                        return false;
                    }

                } else if ( col_type != "TEXT" ) {
                    // different data-type, ask to renew
                    if ( DialogSec::choiceDatabaseWrongDataType( db.name(), table, col_name, col_type ) ) {
                        // agreed to renew
                        throw MakeNewDatabase{};
                    } else {
                        // refused to renew
                        return false;
                    }
                }
            }
        }

    } catch (const MakeNewDatabase&) {

        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            std::error_code err;
            if ( ! IOutils::renameAsCopy( db_path, err ) ) {
                // failed to rename
                QString err_msg;
                if ( err ) {
                    err_msg = QString::fromStdString( err.message() );
                }
                DialogSec::errRenaming( QString::fromStdString(db_path), err_msg );
                return false;
            }
        }
        return newHashesDatabase( DatabaseHandler::get( DatabaseType::Hashes ), db_path, ws_names );

    } catch (const VoidException&) {
        return false;
    }

    return true;
}


bool checkDatabaseFile( const std::string& db_path, const QString& db_name ) noexcept
{
    if ( ! IOutils::exists( db_path ) ) {
        // path doesn't exists
        DialogSec::errDatabaseNotFound( db_name );
        return false;
    } else if ( ! IOutils::isFile( db_path ) ) {
        // path doesn't point to a file
        DialogSec::errDatabaseNotFile( db_name );
        return false;
    } else if ( ! IOutils::checkFile( db_path, true ) ) {
        // database not readable, abort
        DialogSec::errDatabaseNotReadable( db_name );
        return false;
    } else if ( ! IOutils::checkFile( db_path, false, true ) ) {
        // database not writable, abort
        DialogSec::errDatabaseNotWritable( db_name );
        return false;
    }
    return true;
}

} // namespace CheckSec

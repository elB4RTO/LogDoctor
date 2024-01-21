
#include "checks.h"

#include "modules/dialogs.h"

#include "utilities/io.h"

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <vector>
#include <unordered_map>


namespace CheckSec
{

namespace /*private*/
{

//! Checks the tables' names integrity
/*!
  \param db Database object, already initialized
  \param db_name Database's name, used by the dialogs if necessary
  \return The result of the check: 0 if failed with an error, 1 if all the integrity checks passed, 2 if a rebuild is needed
  \see checkCollectionDatabase(), checkHashesDatabase(), newCollectionDatabase(), newHashesDatabase()
*/
int checkDatabaseTablesNames( QSqlDatabase& db, const QString& db_name ) noexcept
{
    bool make_new{false}, ok{true};
    QSqlQuery query{ QSqlQuery( db ) };
    if ( ! query.exec("SELECT name FROM sqlite_schema WHERE type = 'table';") ) {
        // error querying database
        ok &= false;
        DialogSec::errDatabaseFailedExecuting( db_name, query.lastQuery(), query.lastError().text() );
    } else {
        std::unordered_map<QString, bool> tables_checks{
            {"apache", false},
            {"nginx",  false},
            {"iis",    false} };
        while ( query.next() ) {
            QString table_name{ query.value(0).toString() };
            if ( tables_checks.find( table_name ) == tables_checks.end() ) {
                // unexpected table name
                if ( DialogSec::choiceDatabaseWrongTable( db_name, table_name ) ) {
                    // agreed to renew
                    make_new |= true;
                } else {
                    // refused to renew
                    ok &= false;
                }
                break;

            } else {
                // table found
                tables_checks.at( table_name ) |= true;
            }
        }
        if ( ok && !make_new ) {
            for ( const auto& [ tbl, res ] : tables_checks ) {
                if ( ! res ) {
                    // a table has not been found
                    if ( DialogSec::choiceDatabaseMissingTable( db_name, tbl ) ) {
                        // agreed to renew
                        make_new |= true;
                    } else {
                        // refused to renew
                        ok &= false;
                    }
                    break;
                }
            }
        }
        tables_checks.clear();
    }
    query.finish();
    if ( ok ) {
        if ( make_new ) {
            return 2;
        } else {
            return 1;
        }
    }
    return 0;
}


//! Builds a new database for the logs Collection
/*!
  \param db Database object, already initialized
  \param db_name Database's name, used by the dialogs if necessary
  \return The result of the operation
  \see checkCollectionDatabase(), checkHashesDatabase()
*/
bool newCollectionDatabase( QSqlDatabase& db, const QString& db_name, const std::vector<QString>& ws_names ) noexcept
{
    bool successful{ true };
    // create the database
    if ( ! db.open() ) {
        // error opening database
        successful &= false;
        DialogSec::errDatabaseFailedOpening( db_name, db.lastError().text() );

    } else {
        // succesfully creted database file, now create the tables
        QSqlQuery query;
        for ( const QString& ws_name : ws_names ) {
            if ( ! successful ) { break; }
            // compose the statement with the table name for the access logs
            query.prepare( "\
                CREATE TABLE \""+ws_name+"\" (\
                    \"warning\" BOOLEAN,\
                    \"year\" SMALLINT,\
                    \"month\" TINYINT,\
                    \"day\" TINYINT,\
                    \"hour\" TINYINT,\
                    \"minute\" TINYINT,\
                    \"second\" TINYINT,\
                    \"protocol\" TEXT,\
                    \"method\" TEXT,\
                    \"uri\" TEXT,\
                    \"query\" TEXT,\
                    \"response\" SMALLINT,\
                    \"time_taken\" INTEGER,\
                    \"bytes_sent\" INTEGER,\
                    \"bytes_received\" INTEGER,\
                    \"client\" TEXT,\
                    \"user_agent\" TEXT,\
                    \"cookie\" TEXT,\
                    \"referrer\" TEXT\
                );");
            if ( ! query.exec() ) {
                // error creating table
                successful &= false;
                DialogSec::errDatabaseFailedExecuting(
                    QString( db_name ),
                    QString("CREATE TABLE \"%1\" (...)").arg( ws_name ),
                    QString( query.lastError().text() ) );

            }
            query.finish();
        }

        // inform about creation
        if ( successful ) {
            DialogSec::msgDatabaseCreated( db_name );
        } else {
            DialogSec::errDatabaseFailedCreating( db_name );
        }
    }
    return successful;
}


//! Builds a new database for the used log files' Hashes
/*!
  \param db Database object, already initialized
  \param db_name Database's name, used by the dialogs if necessary
  \return The result of the operation
  \see checkCollectionDatabase(), checkHashesDatabase()
*/
bool newHashesDatabase( QSqlDatabase& db, const QString& db_name, const std::vector<QString>& ws_names ) noexcept
{
    bool successful{ true };
    // create the database
    if ( ! db.open() ) {
        // error opening database
        successful &= false;
        DialogSec::errDatabaseFailedOpening( db_name, db.lastError().text() );

    } else {
        // succesfully creted database file, now create the tables
        QSqlQuery query;
        for ( const QString& ws_name : ws_names ) {
            if ( ! successful ) { break; }
            // compose the statement with the table name for the access logs
            query.prepare( "\
                CREATE TABLE \""+ws_name+"\" (\
                    \"hash\" TEXT\
                );");
            if ( ! query.exec() ) {
                // error creating table
                successful &= false;
                DialogSec::errDatabaseFailedExecuting(
                    QString( db_name ),
                    QString("CREATE TABLE \"%1\" (...)").arg( ws_name ),
                    QString( query.lastError().text() ) );

            }
            query.finish();
        }

        // inform about creation
        if ( successful ) {
            DialogSec::msgDatabaseCreated( db_name );
        } else {
            DialogSec::errDatabaseFailedCreating( db_name );
        }
    }
    return successful;
}

} // namespace (private)


bool checkCollectionDatabase( const std::string& db_path ) noexcept
{
    bool make_new{false}, ok{true};
    std::error_code err;
    QString err_msg;
    const QString db_name{ QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) ) };
    const std::vector<QString> ws_names{ "apache", "nginx", "iis" };

    QSqlDatabase db;
    if ( QSqlDatabase::contains("qt_sql_default_connection") ) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName( QString::fromStdString( db_path ) );

    // check the existence
    if ( IOutils::exists( db_path ) ) {
        // check file type and permissions
        if ( ! checkDatabaseFile( db_path, db_name ) ) {
            ok &= false;

        } else {
            // database file seems ok, now try to open
            if ( ! db.open() ) {
                // error opening database
                ok &= false;
                DialogSec::errDatabaseFailedOpening( db_name, db.lastError().text() );

            } else {
                // database successfully opened, now check the tables
                const int check{ checkDatabaseTablesNames( db, db_name ) };
                if ( check == 0 ) {
                    ok &= false;
                } else if ( check == 2 ) {
                    make_new |= true;
                }
                QSqlQuery query{ QSqlQuery( db ) };
                if ( ok && !make_new ) {

                    // check every WebServer table, both access and error
                    for ( const QString& table : ws_names ) {

                        if ( !ok || make_new ) { break; }
                        // column's name:type associations
                        std::unordered_map<QString, std::tuple<QString, bool>>
                            data_types {
                                       {"warning", { "BOOLEAN",  false} },
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

                        // query table's columns' infoes for access logs
                        if ( ! query.exec( "SELECT name, type FROM pragma_table_info('"+table+"') AS tbinfo;" ) ) {
                            // error opening database
                            ok &= false;
                            DialogSec::errDatabaseFailedExecuting( db_name, query.lastQuery(), query.lastError().text() );
                        }
                        // iterate over results
                        while ( query.next() ) {
                            const QString col_name{ query.value(0).toString() };
                            const QString col_type{ query.value(1).toString() };
                            if ( data_types.find( col_name ) == data_types.end() ) {
                                // unexpected column
                                if ( DialogSec::choiceDatabaseWrongColumn( db_name, table, col_name ) ) {
                                    // agreed to renew
                                    make_new |= true;
                                } else {
                                    // refused to renew
                                    ok &= false;
                                }
                                break;

                            } else {
                                // column found, check the data-type
                                const QString data_type{ std::get<0>( data_types.at( col_name ) ) };
                                if ( col_type == data_type ) {
                                    // same data-type
                                    data_types.at( col_name ) = std::tuple( data_type, true );
                                } else {
                                    // different data-type, ask to renew
                                    if ( DialogSec::choiceDatabaseWrongDataType( db_name, table, col_name, col_type ) ) {
                                        // agreed to renew
                                        make_new |= true;
                                    } else {
                                        // refused to renew
                                        ok &= false;
                                    }
                                    break;
                                }
                            }
                        }
                        if ( ok && !make_new ) {
                            for ( const auto& [ col, tup ] : data_types ) {
                                if ( ! std::get<1>( tup ) ) {
                                    // a table has not been found
                                    if ( DialogSec::choiceDatabaseMissingColumn( db_name, table, col ) ) {
                                        // agreed to renew
                                        make_new |= true;
                                    } else {
                                        // refused to renew
                                        ok &= false;
                                    }
                                    break;
                                }
                            }
                        }

                        query.finish();
                        if ( !ok || make_new ) { break; }
                    }
                }
            }
        }

    } else {
        // database does not exist, yet, ask to create a new one
        if ( DialogSec::choiceDatabaseNotFound( QString(db_name) ) ) {
            // choosed to create it
            make_new |= true;

        } else {
            // refused to create it, abort
            ok &= false;
        }
    }

    if ( ok && make_new ) {
        // rename the current db file as a 'copy'
        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            if ( ! IOutils::renameAsCopy( db_path, err ) ) {
                // failed to rename
                ok &= false;
                if ( err ) {
                    err_msg = QString::fromStdString( err.message() );
                }
                DialogSec::errRenaming( QString::fromStdString(db_path), err_msg );
            }/* else {
                // renamed successfully, make new one
            }*/
        }
        if ( ok ) {
            ok = newCollectionDatabase( db, db_name, ws_names );
        }
    }

    if ( db.isOpen() ) {
        db.close();
    }
    return ok;
}


bool checkHashesDatabase( const std::string& db_path ) noexcept
{
    bool make_new{false}, ok{true};
    std::error_code err;
    QString err_msg;
    const QString db_name{ QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) ) };
    const std::vector<QString> ws_names { "apache", "nginx", "iis" };

    QSqlDatabase db;
    if ( QSqlDatabase::contains("qt_sql_default_connection") ) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName( QString::fromStdString( db_path ) );

    // check the existence
    if ( IOutils::exists( db_path ) ) {
        // check file type and permissions
        if ( ! checkDatabaseFile( db_path, db_name ) ) {
            ok &= false;

        } else {
            // database file seems ok, now try to open
            if ( ! db.open() ) {
                // error opening database
                ok &= false;
                DialogSec::errDatabaseFailedOpening( db_name, db.lastError().text() );

            } else {
                // database successfully opened, now check the tables
                const int check = checkDatabaseTablesNames( db, db_name );
                if ( check == 0 ) {
                    ok &= false;
                } else if ( check == 2 ) {
                    make_new |= true;
                }
                QSqlQuery query{ QSqlQuery( db ) };
                if ( ok && !make_new ) {

                    // check every WebServer table, both access and error
                    for ( const QString& table : ws_names ) {

                        if ( !ok || make_new ) { break; }
                        // column's name:type associations
                        bool name_ok{ false },
                             type_ok{ false };

                        // query table's columns' infoes for access logs
                        if ( ! query.exec( "SELECT name, type FROM pragma_table_info('"+table+"') AS tbinfo;" ) ) {
                            // error opening database
                            ok &= false;
                            DialogSec::errDatabaseFailedExecuting( db_name, query.lastQuery(), query.lastError().text() );
                        }
                        // iterate over results
                        while ( query.next() ) {
                            const QString col_name{ query.value(0).toString() };
                            const QString col_type{ query.value(1).toString() };
                            if ( col_name != "hash" ) {
                                // unexpected column
                                if ( DialogSec::choiceDatabaseWrongColumn( db_name, table, col_name ) ) {
                                    // agreed to renew
                                    make_new |= true;
                                } else {
                                    // refused to renew
                                    ok &= false;
                                }
                                break;

                            } else {
                                // column found, check the data-type
                                name_ok |= true;
                                if ( col_type == "TEXT" ) {
                                    // same data-type
                                    type_ok |= true;
                                } else {
                                    // different data-type, ask to renew
                                    if ( DialogSec::choiceDatabaseWrongDataType( db_name, table, col_name, col_type ) ) {
                                        // agreed to renew
                                        make_new |= true;
                                    } else {
                                        // refused to renew
                                        ok &= false;
                                    }
                                    break;
                                }
                            }
                        }
                        if ( ok && !make_new ) {
                            if ( !name_ok || !type_ok ) {
                                ok &= false;
                            }
                        }
                        query.finish();
                    }
                }
            }
        }

    } else {
        // database does not exist, yet, ask to create a new one
        if ( DialogSec::choiceDatabaseNotFound( QString(db_name) ) ) {
            // choosed to create it
            make_new |= true;

        } else {
            // refused to create it, abort
            ok &= false;
        }
    }

    if ( ok && make_new ) {
        // rename the current db file as a 'copy'
        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            if ( ! IOutils::renameAsCopy( db_path, err ) ) {
                // failed to rename
                ok &= false;
                if ( err ) {
                    err_msg = QString::fromStdString( err.message() );
                }
                DialogSec::errRenaming( QString::fromStdString(db_path), err_msg );
            }/* else {
                // renamed successfully, make new one
            }*/
        }
        if ( ok ) {
            ok = newHashesDatabase( db, db_name, ws_names );
        }
    }

    if ( db.isOpen() ) {
        db.close();
    }
    return ok;
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

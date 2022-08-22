
#include "checks.h"

#include "modules/dialogs.h"

#include "utilities/io.h"


CheckSec::CheckSec()
{

}

int CheckSec::checkDatabaseTablesNames( QSqlDatabase& db, const std::string& db_path, const QString& db_name )
{
    bool make_new=false, ok=true;
    QSqlQuery query = QSqlQuery( db );
    if ( query.exec("SELECT name FROM sqlite_schema WHERE type = 'table';") == false ) {
        // error querying database
        ok = false;
        DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query.lastQuery(), query.lastError().text() );
    } else {
        std::unordered_map<QString, bool> tables_checks = {
            {"apache", false},
            {"nginx",  false},
            {"iis",    false} };
        while ( query.next() ) {
            QString table_name = query.value(0).toString();
            if ( tables_checks.find( table_name ) == tables_checks.end() ) {
                // unexpected table name
                if ( DialogSec::choiceDatabaseWrongTable( nullptr, db_name, table_name ) == true ) {
                    // agreed to renew
                    make_new = true;
                } else {
                    // refused to renew
                    ok = false;
                }
                break;

            } else {
                // table found
                tables_checks.at( table_name ) = true;
            }
        }
        if ( ok == true && make_new == false ) {
            for ( const auto& [ tbl, res ] : tables_checks ) {
                if ( res == false ) {
                    // a table has not been found
                    if ( DialogSec::choiceDatabaseMissingTable( nullptr, db_name, tbl ) == true ) {
                        // agreed to renew
                        make_new = true;
                    } else {
                        // refused to renew
                        ok = false;
                    }
                    break;
                }
            }
        }
        tables_checks.clear();
    }
    query.finish();
    if ( ok == true ) {
        if ( make_new == true ) {
            return 2;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

bool CheckSec::newStatsDatabase( QSqlDatabase& db, const std::string& db_path, const QString& db_name, const std::vector<QString>& ws_names )
{
    bool successful = true;
    // create the database
    if ( db.open() == false ) {
        // error opening database
        successful = false;
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, db.lastError().text() );

    } else {
        // succesfully creted database file, now create the tables
        QSqlQuery query;
        for ( const QString& ws_name : ws_names ) {
            if ( successful == false ) { break; }
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
            if ( query.exec() == false ) {
                // error creating table
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr,
                    QString( db_name ),
                    QString("CREATE TABLE \"%1\" (...)").arg( ws_name ),
                    QString( query.lastError().text() ) );

            }
            query.finish();
        }

        // inform about creation
        if ( successful == true ) {
            DialogSec::msgDatabaseCreated( nullptr, db_name );
        }
    }
    return successful;
}

bool CheckSec::checkStatsDatabase( const std::string& db_path )
{
    bool make_new=false, ok=true;
    const QString db_name = QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) );
    const std::vector<QString> ws_names = { "apache", "nginx", "iis" };

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( db_path ) );

    // check the existence
    if ( IOutils::exists( db_path ) ) {
        // database file exists
        if ( IOutils::isFile( db_path ) == false ) {
            // path doesn't point to a file
            DialogSec::errDatabaseNotFile( nullptr, QString(db_name) );
            ok = false;
        } else if ( IOutils::checkFile( db_path, true ) == false ) {
            // database not readable, abort
            DialogSec::errDatabaseNotReadable( nullptr, QString(db_name) );
            ok = false;
        } else if ( IOutils::checkFile( db_path, false, true ) == false ) {
            // database not writable, abort
            DialogSec::errDatabaseNotWritable( nullptr, QString(db_name) );
            ok = false;

        } else {
            // database file has read and write permissions, now try to open
            if ( db.open() == false ) {
                // error opening database
                ok = false;
                DialogSec::errDatabaseFailedOpening( nullptr, db_name, db.lastError().text() );

            } else {
                // database successfully opened, now check the tables
                int check = CheckSec::checkDatabaseTablesNames( db, db_path, db_name );
                if ( check == 0 ) {
                    ok = false;
                } else if ( check == 2 ) {
                    make_new = true;
                }
                QSqlQuery query = QSqlQuery( db );
                if ( ok == true && make_new == false ) {

                    // check every WebServer table, both access and error
                    for ( const QString& table : ws_names ) {

                        if ( ok == false || make_new == true ) { break; }
                        // column's name:type associations
                        std::unordered_map<QString, std::tuple<QString, bool>>
                            data_types = {
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
                        if ( query.exec( "SELECT name, type FROM pragma_table_info('"+table+"') AS tbinfo;" ) == false ) {
                            // error opening database
                            ok = false;
                            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query.lastQuery(), query.lastError().text() );
                        }
                        // iterate over results
                        while ( query.next() ) {
                            QString col_name = query.value(0).toString(),
                                    col_type = query.value(1).toString();
                            if ( data_types.find( col_name ) == data_types.end() ) {
                                // unexpected column
                                if ( DialogSec::choiceDatabaseWrongColumn( nullptr, db_name, table, col_name ) == true ) {
                                    // agreed to renew
                                    make_new = true;
                                } else {
                                    // refused to renew
                                    ok = false;
                                }
                                break;

                            } else {
                                // column found, check the data-type
                                QString data_type = std::get<0>(data_types.at( col_name ) );
                                if ( col_type == data_type ) {
                                    // same data-type
                                    data_types.at( col_name ) = std::tuple( data_type, true );
                                } else {
                                    // different data-type, ask to renew
                                    if ( DialogSec::choiceDatabaseWrongDataType( nullptr, db_name, table, col_name, col_type ) == true ) {
                                        // agreed to renew
                                        make_new = true;
                                    } else {
                                        // refused to renew
                                        ok = false;
                                    }
                                    break;
                                }
                            }
                        }
                        if ( ok == true && make_new == false ) {
                            for ( const auto& [ col, tup ] : data_types ) {
                                if ( std::get<1>( tup ) == false ) {
                                    // a table has not been found
                                    if ( DialogSec::choiceDatabaseMissingColumn( nullptr, db_name, table, col ) == true ) {
                                        // agreed to renew
                                        make_new = true;
                                    } else {
                                        // refused to renew
                                        ok = false;
                                    }
                                    break;
                                }
                            }
                        }

                        query.finish();
                        if ( ok == false || make_new == true ) { break; }
                    }
                }
            }
        }

    } else {
        // database does not exist, yet, ask to create a new one
        if ( DialogSec::choiceDatabaseNotFound( nullptr, QString(db_name) ) ) {
            // choosed to create it
            make_new = true;

        } else {
            // refused to create it, abort
            ok = false;
        }
    }

    if ( ok == true && make_new == true ) {
        // rename the current db file as a 'copy'
        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            if ( IOutils::renameAsCopy( db_path ) == false ) {
                // failed to rename
                ok = false;
                DialogSec::errRenaming( nullptr, QString::fromStdString(db_path) );
            }/* else {
                // renamed successfully, make new one
            }*/
        }
        if ( ok == true ) {
            ok = CheckSec::newStatsDatabase( db, db_path, db_name, ws_names );
        }
    }

    db.close();
    return ok;
}



bool CheckSec::newHashesDatabase( QSqlDatabase& db, const std::string& db_path, const QString& db_name, const std::vector<QString>& ws_names )
{
    bool successful = true;
    // create the database
    if ( db.open() == false ) {
        // error opening database
        successful = false;
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, db.lastError().text() );

    } else {
        // succesfully creted database file, now create the tables
        QSqlQuery query;
        for ( const QString& ws_name : ws_names ) {
            if ( successful == false ) { break; }
            // compose the statement with the table name for the access logs
            query.prepare( "\
                CREATE TABLE \""+ws_name+"\" (\
                    \"hash\" TEXT\
                );");
            if ( query.exec() == false ) {
                // error creating table
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr,
                    QString( db_name ),
                    QString("CREATE TABLE \"%1\" (...)").arg( ws_name ),
                    QString( query.lastError().text() ) );

            }
            query.finish();
        }

        // inform about creation
        if ( successful == true ) {
            DialogSec::msgDatabaseCreated( nullptr, db_name );
        }
    }
    return successful;
}

bool CheckSec::checkHashesDatabase( const std::string& db_path )
{
    bool make_new=false, ok=true;
    const QString db_name = QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) );
    const std::vector<QString> ws_names = { "apache", "nginx", "iis" };

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( db_path ) );

    // check the existence
    if ( IOutils::exists( db_path ) ) {
        // database file exists
        if ( IOutils::isFile( db_path ) == false ) {
            // path doesn't point to a file
            DialogSec::errDatabaseNotFile( nullptr, QString(db_name) );
            ok = false;
        } else if ( IOutils::checkFile( db_path, true ) == false ) {
            // database not readable, abort
            DialogSec::errDatabaseNotReadable( nullptr, QString(db_name) );
            ok = false;
        } else if ( IOutils::checkFile( db_path, false, true ) == false ) {
            // database not writable, abort
            DialogSec::errDatabaseNotWritable( nullptr, QString(db_name) );
            ok = false;

        } else {
            // database file has read and write permissions, now try to open
            if ( db.open() == false ) {
                // error opening database
                ok = false;
                DialogSec::errDatabaseFailedOpening( nullptr, db_name, db.lastError().text() );

            } else {
                // database successfully opened, now check the tables
                int check = CheckSec::checkDatabaseTablesNames( db, db_path, db_name );
                if ( check == 0 ) {
                    ok = false;
                } else if ( check == 2 ) {
                    make_new = true;
                }
                QSqlQuery query = QSqlQuery( db );
                if ( ok == true && make_new == false ) {

                    // check every WebServer table, both access and error
                    for ( const QString& table : ws_names ) {

                        if ( ok == false || make_new == true ) { break; }
                        // column's name:type associations
                        bool name_ok = false,
                             type_ok = false;

                        // query table's columns' infoes for access logs
                        if ( query.exec( "SELECT name, type FROM pragma_table_info('"+table+"') AS tbinfo;" ) == false ) {
                            // error opening database
                            ok = false;
                            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query.lastQuery(), query.lastError().text() );
                        }
                        // iterate over results
                        while ( query.next() ) {
                            QString col_name = query.value(0).toString(),
                                    col_type = query.value(1).toString();
                            if ( col_name != "hash" ) {
                                // unexpected column
                                if ( DialogSec::choiceDatabaseWrongColumn( nullptr, db_name, table, col_name ) == true ) {
                                    // agreed to renew
                                    make_new = true;
                                } else {
                                    // refused to renew
                                    ok = false;
                                }
                                break;

                            } else {
                                // column found, check the data-type
                                name_ok = true;
                                if ( col_type == "TEXT" ) {
                                    // same data-type
                                    type_ok = true;
                                } else {
                                    // different data-type, ask to renew
                                    if ( DialogSec::choiceDatabaseWrongDataType( nullptr, db_name, table, col_name, col_type ) == true ) {
                                        // agreed to renew
                                        make_new = true;
                                    } else {
                                        // refused to renew
                                        ok = false;
                                    }
                                    break;
                                }
                            }
                        }
                        if ( ok == true && make_new == false ) {
                            if ( name_ok == false || type_ok == false ) {
                                ok = false;
                            }
                        }
                        query.finish();
                    }
                }
            }
        }

    } else {
        // database does not exist, yet, ask to create a new one
        if ( DialogSec::choiceDatabaseNotFound( nullptr, QString(db_name) ) ) {
            // choosed to create it
            make_new = true;

        } else {
            // refused to create it, abort
            ok = false;
        }
    }

    if ( ok == true && make_new == true ) {
        // rename the current db file as a 'copy'
        if ( IOutils::exists( db_path ) ) {
            // a database already exists, try rename it
            if ( IOutils::renameAsCopy( db_path ) == false ) {
                // failed to rename
                ok = false;
                DialogSec::errRenaming( nullptr, QString::fromStdString(db_path) );
            }/* else {
                // renamed successfully, make new one
            }*/
        }
        if ( ok == true ) {
            ok = CheckSec::newHashesDatabase( db, db_path, db_name, ws_names );
        }
    }

    db.close();
    return ok;
}

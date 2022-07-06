
#include "checks.h"

#include "utilities/io.h"

#include <thread>


CheckSec::CheckSec()
{

}


bool CheckSec::checkStatsDatabase( std::string path )
{
    bool ok = true;
    if ( IOutils::exists( path ) ) {
        // database file exists
        if ( IOutils::checkFile( path, true ) == false ) {
            // database not readable, abort
            DialogSec::errDatabaseNotReadable( nullptr, QString("statistics") );
            ok = false;
        } else if ( IOutils::checkFile( path, false, true ) == false ) {
            // database not writable, abort
            DialogSec::errDatabaseNotWritable( nullptr, QString("statistics") );
            ok = false;

        } else {
            // database file has read and write permissions, now check the tables

        }

    } else {
        // database does not exist, yet ask to create a new one
        if ( DialogSec::choiceDatabaseNotFound( nullptr, QString("statistics") ) ) {
            // choosed to create it
            int rc;
            char *errMsg = 0;
            sqlite3 *db;

            rc = sqlite3_open( path.c_str(), &db);
            if ( rc != SQLITE_OK ) {
                // error opening database
                ok = false;
                DialogSec::errDatabaseFailedOpening( nullptr, "statistics", QString(sqlite3_errmsg(db)) );

            } else {
                // succesfully creted database file, now create the tables
                std::vector<std::string> ws_names = { "apache", "ngin", "iis" };
                std::string statement;
                int tries;
                for ( const std::string& ws_name : ws_names ) {
                    // compose the statement with the table name for the access logs
                    statement = "\
                        CREATE TABLE "+ws_name+"_access (\
                            warning BOOLEAN,\
                            year SMALLINT,\
                            month TINYINT,\
                            day TINYINT,\
                            hour TINYINT,\
                            minute TINYINT,\
                            second TINYINT,\
                            protocol TEXT,\
                            method TEXT,\
                            request TEXT,\
                            query TEXT,\
                            response SMALLINT,\
                            time_taken INTEGER,\
                            bytes_sent INTEGER,\
                            bytes_received INTEGER,\
                            client TEXT,\
                            user_agent TEXT,\
                            cookie TEXT,\
                            referrer TEXT\
                        );";
                    tries = 0;
                    while (true) {
                        tries ++;
                        if ( tries > 3 ) {
                            break;
                        }
                        rc = sqlite3_exec(db, statement.c_str(), nullptr, nullptr, &errMsg);
                        if ( rc == SQLITE_BUSY ) {
                            // database busy, wait 1 sec and retry
                            std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                        } else {
                            break;
                        }
                    }
                    if ( rc != SQLITE_OK ) {
                        // error creating table
                        ok = false;
                        DialogSec::errDatabaseFailedExecuting( nullptr,
                            QString("statistics"),
                            QString("CREATE TABLE %1_access (...)").arg( ws_name.c_str() ),
                            QString(errMsg) );
                        sqlite3_free( errMsg );
                    }

                    // compose the statement with the table name for the error logs
                    statement = "\
                        CREATE TABLE "+ws_name+"_error (\
                            warning BOOLEAN,\
                            year SMALLINT,\
                            month TINYINT,\
                            day TINYINT,\
                            hour TINYINT,\
                            minute TINYINT,\
                            second TINYINT,\
                            level TEXT,\
                            message TEXT,\
                            source_file TEXT,\
                            client TEXT,\
                            port SMALLINT\
                        );";
                    tries = 0;
                    while (true) {
                        tries ++;
                        if ( tries > 3 ) {
                            break;
                        }
                        rc = sqlite3_exec(db, statement.c_str(), nullptr, nullptr, &errMsg);
                        if ( rc == SQLITE_BUSY ) {
                            // database busy, wait 1 sec and retry
                            std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
                        } else {
                            break;
                        }
                    }
                    if ( rc != SQLITE_OK ) {
                        // error creating table
                        ok = false;
                        DialogSec::errDatabaseFailedExecuting( nullptr,
                            QString("statistics"),
                            QString("CREATE TABLE %1_error (...)").arg( ws_name.c_str() ),
                            QString(errMsg) );
                        sqlite3_free( errMsg );
                    }
                }

                // finally close the database
                sqlite3_close( db );
            }

        } else {
            // refused to create it, abort
            ok = false;
        }
    }

    return ok;
}


bool CheckSec::checkCraplog( Craplog craplog )
{
    bool ok = true;


    return ok;
}

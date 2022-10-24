
#include "hash.h"

#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"
#include "modules/craplog/modules/sha256.h"

#include <ios>


HashOps::HashOps()
{

}

void HashOps::setDialogLevel( const int& new_level )
{
    this->dialog_level = new_level;
}


// reads the database holding the already used hashes
bool HashOps::loadUsedHashesLists( const std::string& db_path )
{
    bool successful = true;
    const QString db_name = QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) );

    QSqlDatabase db;
    if ( QSqlDatabase::contains("qt_sql_default_connection") ) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName( QString::fromStdString( db_path ) );

    if ( ! db.open() ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( db_name, err_msg );

    } else {
        QSqlQuery query = QSqlQuery( db );
        for ( const auto& [wid,name] : this->ws_names ) {
            if ( ! query.exec("SELECT hash FROM "+name+";") ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( db_name, query.lastQuery(), query.lastError().text() );
                break;
            } else {
                // iterate over results
                while ( query.next() ) {
                    std::string hash = query.value(0).toString().toStdString();
                    if ( hash.size() != 64 ) {
                        // not a valid sha256 hash
                        continue;
                    }
                    this->hashes.at( wid ).push_back( hash );
                }
            }
            if ( ! successful ) { break; }
        }
    }
    db.close();
    return successful;
}


// returns the hash
std::string HashOps::digestFile( const std::string& file_path )
{
    std::string content;
    try {
        try {
            // try reading as gzip compressed file
            GZutils::readFile( file_path, content );

        } catch ( const GenericException& ) {
            // failed closing file pointer
            throw;

        } catch (...) {
            // failed as gzip, try as text file
            if ( content.size() > 0 ) {
                content.clear();
            }
            IOutils::readFile( file_path, content );
        }

    // re-catched in craplog
    } catch ( const GenericException& ) {
        // failed closing gzip file pointer
        throw GenericException( QString("%1:\n%2").arg(
            DialogSec::tr("An error accured while reading the gzipped file"),
            QString::fromStdString( file_path )
            ).toStdString() );

    } catch ( const std::ios_base::failure& ) {
        // failed reading as text
        throw GenericException( QString("%1:\n%2").arg(
            DialogSec::tr("An error accured while reading the file"),
            QString::fromStdString( file_path )
            ).toStdString() );

    } catch (...) {
        // failed somehow
        throw GenericException( QString("%1:\n%2").arg(
            DialogSec::tr("Something failed while handling the file"),
            QString::fromStdString( file_path )
            ).toStdString() );
    }

    SHA256 sha;
    sha.update( content );
    content.clear();
    uint8_t * digest = sha.digest();
    // return the hex digest
    return SHA256::toString(digest);
}


// check if the given hash is from a file which has been used already
bool HashOps::hasBeenUsed( const std::string &file_hash, const int& web_server_id)
{
    bool found = false;
    for ( const std::string &hash : this->hashes.at( web_server_id ) ) {
        if ( file_hash == hash ) {
            found = true;
            break;
        }
    }
    return found;
}


// insert the given hash/es in the relative list
bool HashOps::insertUsedHash( QSqlQuery& query, const QString& db_name, const std::string& hash, const int& web_server_id )
{
    bool successful = true;
    try {
        if( ! VecOps::contains( this->hashes.at( web_server_id ), hash ) ) {
            this->hashes.at( web_server_id ).push_back( hash );
            // insert tnto the database
            QString stmt = QString("INSERT INTO %1 ( hash ) VALUES ( '%2' );")
                .arg( this->ws_names.at(web_server_id), QString::fromStdString(hash).replace("'","''") );
            if ( ! query.exec( stmt ) ) {
                // error opening database
                successful = false;
                QString query_msg="", err_msg="";
                if ( this->dialog_level > 0 ) {
                    query_msg = "query.exec()";
                    if ( this->dialog_level == 2 ) {
                        err_msg = query.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( db_name, query_msg, err_msg );
            }
        }/* else {
            // hash already stored
        }*/
    } catch (...) {
        // failed to insert the hash
        successful = false;
    }
    query.finish();
    return successful;
}


bool HashOps::insertUsedHashes( const std::string& db_path, const std::vector<std::string> &hashes, const int& web_server_id )
{
    bool proceed = true;

    const QString db_name = QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) );
    QSqlDatabase db;
    if ( QSqlDatabase::contains("qt_sql_default_connection") ) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName( QString::fromStdString( db_path ) );

    if ( ! db.open() ) {
        // error opening database
        proceed = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( db_name, err_msg );

    } else {
        QSqlQuery query = QSqlQuery( db );
        if ( ! db.transaction() ) {
            // error opening database
            proceed = false;
            QString stmt_msg="", err_msg = "";
            if ( this->dialog_level > 0 ) {
                stmt_msg = "db.transaction()";
                if ( this->dialog_level == 2 ) {
                    err_msg = db.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );

        } else {

            try {
                for ( const std::string& hash : hashes ) {
                    proceed = this->insertUsedHash( query, db_name, hash, web_server_id );
                    if ( ! proceed ) {
                        break;
                    }
                }
                query.finish();

                if ( proceed ) {
                    // commit the transaction
                    if ( ! db.commit() ) {
                        // error opening database
                        proceed = false;
                        QString stmt_msg="", err_msg = "";
                        if ( this->dialog_level > 0 ) {
                            stmt_msg = "db.commit()";
                            if ( this->dialog_level == 2 ) {
                                err_msg= db.lastError().text();
                            }
                        }
                        DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );
                    }
                }
                if ( ! proceed ) {
                    // rollback
                    throw (std::exception());
                }

            } catch (...) {
                // wrongthing w3nt some.,.
                proceed = false;
                bool err_shown = false;
                // rollback the transaction
                if ( ! db.rollback() ) {
                    // error rolling back commits
                    QString stmt_msg="", err_msg = "";
                    if ( this->dialog_level > 0 ) {
                        stmt_msg = "db.rollback()";
                        if ( this->dialog_level == 2 ) {
                            err_msg = db.lastError().text();
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );
                    err_shown = true;
                }
                if ( ! err_shown ) {
                    // show a message
                    QString msg = DialogSec::tr("An error occured while working on the database\n\nAborting");
                    DialogSec::errGeneric( msg );
                }
            }
        }
    }
    db.close();
    return proceed;
}

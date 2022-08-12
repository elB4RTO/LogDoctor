
#include "hash.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/vectors.h"

#include "tools/craplog/modules/sha256.h"



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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( db_path ) );

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, err_msg );

    } else {
        QSqlQuery query = QSqlQuery( db );
        for ( const auto& [wid,name] : this->ws_names ) {
            if ( query.exec("SELECT hash FROM "+name+";") == false ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query.lastQuery(), query.lastError().text() );
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
            if ( successful == false ) { break; }
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
            GzipOps::readFile( file_path, content );

        } catch (GenericException& e) {
            // failed closing file pointer
            throw e;

        } catch (...) {
            // failed as gzip, try as text file
            if ( content.size() > 0 ) {
                content.clear();
            }
            IOutils::readFile( file_path, content );
        }

    } catch (GenericException& err) {
        // failed closing
        throw err;

    } catch (const std::ios_base::failure& err) {
        // failed reading
        throw err;

    } catch (...) {
        // failed somehow
        throw GenericException( "Something failed while handling file" );
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
bool HashOps::insertUsedHash( QSqlDatabase& db, QSqlQuery& query, const QString& db_name, const std::string& hash, const int& web_server_id )
{
    bool successful = true;
    try {
        if( VecOps::contains( this->hashes.at( web_server_id ), hash ) == false ) {
            this->hashes.at( web_server_id ).push_back( hash );
            // insert tnto the database
            QString stmt = QString("INSERT INTO %1 ( hash ) VALUES ( '%2' );")
                .arg( this->ws_names.at(web_server_id), QString::fromStdString(hash).replace("'","''") );
            if ( query.exec( stmt ) == false ) {
                // error opening database
                successful = false;
                QString query_msg="", err_msg="";
                if ( this->dialog_level > 0 ) {
                    query_msg = "query.exec()";
                    if ( this->dialog_level == 2 ) {
                        err_msg = query.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, query_msg, err_msg );
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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( db_path ) );

    if ( db.open() == false ) {
        // error opening database
        proceed = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, err_msg );

    } else {
        QSqlQuery query = QSqlQuery( db );
        if ( db.transaction() == false ) {
            // error opening database
            proceed = false;
            QString stmt_msg="", err_msg = "";
            if ( this->dialog_level > 0 ) {
                stmt_msg = "db.transaction()";
                if ( this->dialog_level == 2 ) {
                    err_msg = db.lastError().text();
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );

        } else {

            try {
                for ( const std::string& hash : hashes ) {
                    proceed = this->insertUsedHash( db, query, db_name, hash, web_server_id );
                    if ( proceed == false ) {
                        break;
                    }
                }
                query.finish();

                if ( proceed == true ) {
                    // commit the transaction
                    if ( db.commit() == false ) {
                        // error opening database
                        proceed = false;
                        QString stmt_msg="", err_msg = "";
                        if ( this->dialog_level > 0 ) {
                            stmt_msg = "db.commit()";
                            if ( this->dialog_level == 2 ) {
                                err_msg= db.lastError().text();
                            }
                        }
                        DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                    }
                }
                if ( proceed == false ) {
                    // rollback
                    throw (std::exception());
                }

            } catch (...) {
                // wrongthing w3nt some.,.
                proceed = false;
                bool err_shown = false;
                // rollback the transaction
                if ( db.rollback() == false ) {
                    // error rolling back commits
                    QString stmt_msg="", err_msg = "";
                    if ( this->dialog_level > 0 ) {
                        stmt_msg = "db.rollback()";
                        if ( this->dialog_level == 2 ) {
                            err_msg = db.lastError().text();
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                    err_shown = true;
                }
                if ( err_shown == false ) {
                    // show a message
                    QString msg = QMessageBox::tr("An error occured while working on the database\n\nAborting");
                    DialogSec::errGeneric( nullptr, msg );
                }
            }
        }
    }
    db.close();
    return proceed;
}



#include "hash.h"

#include "modules/dialogs.h"

#include "utilities/io.h"
#include "utilities/vectors.h"

#include "tools/craplog/modules/sha256.h"



HashOps::HashOps()
{
    this->hashes[this->APACHE_ID] = std::unordered_map<int, std::vector<std::string>>();
    this->hashes[this->NGINX_ID] = std::unordered_map<int, std::vector<std::string>>();
    this->hashes[this->IIS_ID] = std::unordered_map<int, std::vector<std::string>>();
    for ( int i=1; i<3; i++ ) {
        this->hashes[this->APACHE_ID].emplace( i, std::vector<std::string>() );
        this->hashes[this->NGINX_ID].emplace( i, std::vector<std::string>() );
        this->hashes[this->IIS_ID].emplace( i, std::vector<std::string>() );
    }
}

void HashOps::setDialogLevel( const int new_level )
{
    this->dialog_level = new_level;
}


// reads the database holding the already used hashes
bool HashOps::loadUsedHashesLists( const std::string& db_path )
{
    bool successful = true;
    const QString db_name = QString::fromStdString( db_path.substr( db_path.find_last_of( '/' ) + 1 ) );
    const std::unordered_map<int, QString> ws_names = {
        {11, "apache"},
        {12, "nginx"},
        {13, "iis"} };
    const std::unordered_map<int, QString> log_types = {
        {1, "access"},
        {2, "error"} };

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
        for ( const auto& [wid,name] : ws_names ) {
            for ( const auto& [tid,type] : log_types ) {
                if ( query.exec("SELECT hash FROM "+name+"_"+type+";") == false ) {
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
                        this->hashes.at( wid ).at( tid ).push_back( hash );
                    }
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
    std::string content = IOutils::readFile( file_path );
    SHA256 sha;
    sha.update( content );
    uint8_t * digest = sha.digest();
    // return the hex digest
    return SHA256::toString(digest);
}


// check if the given hash is from a file which has been used already
bool HashOps::hasBeenUsed(const std::string &file_hash, const int web_server_id, const int log_type )
{
    bool found = false;
    for ( const std::string &hash : this->hashes.at( web_server_id ).at( log_type ) ) {
        if ( file_hash == hash ) {
            found = true;
            break;
        }
    }
    return found;
}


// insert the given hash/es in the relative list
bool HashOps::insertUsedHash(QSqlDatabase& db, QSqlQuery& query, const QString& db_name, const std::string& hash, const int web_server_id, const int log_type )
{
    bool successful = true;
    try {
        if( VecOps::contains( this->hashes.at( web_server_id ).at( log_type ), hash ) == false ) {
            this->hashes.at( web_server_id ).at( log_type ).push_back( hash );
            // insert tnto the database
            QString stmt = QString("INSERT INTO %1_%2 ( hash ) VALUES ( '%3' );")
                .arg( this->ws_names.at(web_server_id), this->log_types.at(log_type), QString::fromStdString(hash).replace("'","''") );
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


bool HashOps::insertUsedHashes(const std::string& db_path, const std::unordered_map<int, std::vector<std::string>>& hashes, const int web_server_id )
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
                for ( const auto& [ log_type, data ] : hashes ) {
                    for ( const std::string& hash : data ) {
                        proceed = this->insertUsedHash( db, query, db_name, hash, web_server_id, log_type );
                        if ( proceed == false ) {
                            break;
                        }
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


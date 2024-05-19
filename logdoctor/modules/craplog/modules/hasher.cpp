
#include "hasher.h"

#include "hasher/sha256.h"

#include "globals/db_names.h"

#include "utilities/checks.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/database/database.h"
#include "modules/security/path.h"

#include <ios>

#include <QVariant>


void Hasher::setDialogLevel( const DialogsLevel new_level ) noexcept
{
    this->dialogs_level = new_level;
}


// reads the database holding the already used hashes
bool Hasher::loadUsedHashesLists( const PathHandler& db_path ) noexcept
{
    const auto path_exp{ db_path.getPath() };
    if ( ! path_exp.has_value() ) {
        path_exp.error().showDialogMessage();
        return false;
    }

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Hashes ) };

    db.open( db_path, this->dialogs_level==DL_EXPLANATORY );

    const QString stmt{ QStringLiteral(R"(SELECT "hash" FROM "%1";)") };

    for ( const auto& [wid,name] : this->ws_names ) {

        QueryWrapper query{ db.getQuery() };

        query( stmt.arg( name ) );

        while ( query->next() ) {
            const QString hash{ query[0].toString() };
            if ( hash.size() != 64ul ) {
                // not a valid sha256 hash
                continue;
            }
            this->hashes.at( wid ).push_back( hash.toStdString() );
        }
    }

    return true;
}


// returns the hash
void Hasher::digestFile( const PathHandler& file_path, std::string& hash )
{
    std::string content;
    try {
        const auto path_exp{ file_path.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            throw "";
        }
        const path_t& path{ path_exp.value() };

        try {
            // try reading as gzip compressed file
            GZutils::readFile( path, content );

        } catch ( const GenericException& ) {
            // failed closing file pointer
            throw;

        } catch (...) {
            // failed as gzip, try as text file
            if ( ! content.empty() ) {
                content.clear();
            }
            IOutils::readFile( path, content );
        }

    // re-catched in craplog
    } catch ( const GenericException& ) {
        // failed closing gzip file pointer
        throw GenericException( QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("An error occured while reading the gzipped file"),
            QString::fromStdString( file_path.toString() )
            ).toStdString() );

    } catch ( const std::ios_base::failure& ) {
        // failed reading as text
        throw GenericException( QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("An error occured while reading the file"),
            QString::fromStdString( file_path.toString() )
            ).toStdString() );

    } catch (...) {
        // failed somehow
        throw GenericException( QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("Something failed while handling the file"),
            QString::fromStdString( file_path.toString() )
            ).toStdString() );
    }

    SHA256 sha;
    sha.update( content );
    uint8_t* digest{ sha.digest() };
    // return the hex digest
    hash.append( SHA256::toString(digest) );
    delete digest;
}


// check if the given hash is from a file which has been used already
bool Hasher::hasBeenUsed( const std::string &file_hash, const WebServer& web_server) const noexcept
{
    const auto& ws_hashes{ this->hashes.at( web_server ) };
    return std::any_of(
        ws_hashes.cbegin(), ws_hashes.cend(),
        [&file_hash]( const std::string& hash )
                    { return file_hash == hash; } );
}


void Hasher::insertUsedHashes( const PathHandler& db_path, const std::vector<std::string>& hashes, const WebServer& web_server )
{
    const bool explain_msg{ this->dialogs_level >  DL_ESSENTIAL   };
    const bool explain_err{ this->dialogs_level == DL_EXPLANATORY };

    const auto path_exp{ db_path.getPath() };
    if ( ! path_exp.has_value() ) {
        path_exp.error().showDialogMessage();
        throw VoidException();
    }

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Hashes ) };

    db.open( db_path, explain_err );

    db.startTransaction( explain_msg, explain_err );

    try {

        for ( const std::string& hash : hashes ) {

            if ( ! VecOps::contains( this->hashes.at( web_server ), hash ) ) {

                db.getQuery()( QStringLiteral(R"(INSERT INTO "%1" ( hash ) VALUES ( '%2' );)")
                                .arg( this->ws_names.at(web_server), QString::fromStdString(hash).replace(QLatin1Char('\''),QLatin1String("''")) ) );
            }
        }

        db.commitTransaction( explain_msg, explain_err );

        auto& used_hashes{ this->hashes.at( web_server ) };
        used_hashes.insert( used_hashes.end(), hashes.begin(), hashes.end() );

    } catch (...) {
        // rollback the transaction
        db.rollbackTransaction( explain_msg, explain_err );
    }
}

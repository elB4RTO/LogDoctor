
#include "hash.h"

#include "utilities/io.h"
#include "utilities/vectors.h"

#include "tools/craplog/modules/sha256.h"

#include <sstream>
#include <iomanip>



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


// reads the database holding the already used hashes
void HashOps::readLists( const std::string& dir_path )
{
    for ( int id=11; id<14; id++ ) {

    }
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
bool HashOps::insertUsedHash( const std::string& hash, const int web_server_id, const int log_type )
{
    bool proceed = true;
    try {
        if( VecOps::contains( this->hashes.at( web_server_id ).at( log_type ), hash ) == false ) {
            this->hashes.at( web_server_id ).at( log_type ).push_back( hash );
        }
    } catch (...) {
        // failed to insert the hash
            // !!! PUT AN DIALOG ERROR MESSAGE HERE !!!
            // ask to ignore this file, retry or abort all
    }
    return proceed;
}


bool HashOps::insertUsedHashes( const std::unordered_map<int, std::vector<std::string>>& hashes, const int web_server_id )
{
    bool proceed = true;
    for ( const auto& [ log_type, data ] : hashes ) {
        for ( const std::string& hash : data ) {
            if ( this->insertUsedHash( hash, web_server_id, log_type ) == false ) {
                proceed = false;
                break;
            }
        }
    }
    return proceed;
}


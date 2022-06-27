
#include "hash.h"

#include "utilities/io.h"
#include "utilities/vectors.h"

#include <sstream>
#include <iomanip>

#include "tools/craplog/modules/sha256.h"

using std::string, std::vector;


HashOps::HashOps()
{
    this->hashes.emplace( this->APACHE_ID, vector<string>() );
    this->hashes.emplace( this->NGINX_ID, vector<string>() );
    this->hashes.emplace( this->IIS_ID, vector<string>() );
}


// reads the files holding the already used hashes
void HashOps::readLists( string dir_path )
{
    for ( int id=11; id<14; id++ ) {

    }
}


// returns the hash
string HashOps::digestFile( string file_path )
{
    string content = IOutils::readFile( file_path );
    SHA256 sha;
    sha.update( content );
    uint8_t * digest = sha.digest();
    // return the hex digest
    return SHA256::toString(digest);
}


// check if the given hash is from a file which has been used already
bool HashOps::hasBeenUsed( string file_hash, const int web_server_id )
{
    bool found = false;
    for ( const string &hash : this->hashes[ web_server_id ] ) {
        if ( file_hash == hash ) {
            found = true;
            break;
        }
    }
    return found;
}


// insert the given hash/es in the relative list
bool HashOps::insertHash( string hash, const int web_server_id )
{
    bool proceed = true;
    try {
        if( VecOps::contains( this->hashes[ web_server_id ], hash )) {
            this->hashes[ web_server_id ].push_back( hash );
        }
    } catch (...) {
        // failed to insert the hash
            // !!! PUT AN DIALOG ERROR MESSAGE HERE !!!
            // ask to ignore this file, retry or abort all
    }
    return proceed;
}


bool HashOps::insertHashes( vector<string> hashes, const int web_server_id )
{
    bool proceed = true;
    for ( string& hash : hashes ) {
        if ( this->insertHash( hash, web_server_id ) == false ) {
            proceed = false;
            break;
        }
    }
    return proceed;
}


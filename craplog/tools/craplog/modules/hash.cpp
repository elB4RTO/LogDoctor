
#include "hash.h"

#include "utilities/io.h"
#include "utilities/vectors.h"

#include <openssl/sha.h>
#include <sstream>
#include <iomanip>


HashOps::HashOps()
{
    this->hashes.emplace( this->APACHE_ID, new std::vector<std::string> );
    this->hashes.emplace( this->NGINX_ID, new std::vector<std::string> );
    this->hashes.emplace( this->IIS_ID, new std::vector<std::string> );
}


// reads the files holding the already used hashes
void HashOps::readLists( std::string dir_path )
{
    for ( int id=11; id<14; id++ ) {

    }
}


// returns the hash
std::string HashOps::digestFile( std::string file_path )
{
    std::string content = IOutils::readFile( file_path );
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, content.c_str(), content.size());
    SHA256_Final(hash, &sha256);
    std::stringstream hex_digest;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        hex_digest << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return hex_digest.str();
}


// check if the given hash is from a file which has been used already
bool HashOps::hasBeenUsed( std::string file_hash, const int web_server_id )
{
    bool found = false;
    for ( std::string hash : this->hashes[ web_server_id ] ) {
        if ( file_hash == hash ) {
            found = true;
            break;
        }
    }
    return found;
}


// insert the given hash/es in the relative list
bool HashOps::insertHash( std::string hash, const int web_server_id )
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


bool HashOps::insertHashes( std::vector<std::string> hashes, const int web_server_id )
{
    bool proceed = true;
    for ( std::string& hash : hashes ) {
        if ( this->insertHash( hash, web_server_id ) == false ) {
            proceed = false;
            break;
        }
    }
    return proceed;
}


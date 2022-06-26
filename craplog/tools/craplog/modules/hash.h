#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <unordered_map>


class HashOps
{
public:
    HashOps();

    // retrieve the lists from file
    void readLists( std::string dir_path );

    // returns the hash
    std::string digestFile( std::string file_path );
    // check if the given hash is from a file which has been used already
    bool hasBeenUsed( std::string file_hash, int web_server_id );
    // insert the given hash/es in the relative list
    bool insertHash( std::string hash, int web_server_id );
    bool insertHashes( std::vector<std::string> hashes, int web_server_id );

private:
    // id constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;
    // lists of used files' hashes
    std::unordered_map<int, std::vector<std::string>> hashes;

};

#endif // HASH_H

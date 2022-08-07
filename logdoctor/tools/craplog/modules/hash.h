#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <unordered_map>

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


class HashOps
{
public:
    HashOps();

    void setDialogLevel( const int& new_level );

    // retrieve the lists from the database file
    bool loadUsedHashesLists( const std::string& db_path );

    // returns the hash
    std::string digestFile( const std::string& file_path );
    // check if the given hash is from a file which has been used already
    bool hasBeenUsed( const std::string& file_hash, const int& web_server_id );
    // insert the given hash/es in the relative list
    bool insertUsedHash( QSqlDatabase& db, QSqlQuery& query, const QString& db_name, const std::string& hash, const int& web_server_id );
    bool insertUsedHashes( const std::string& db_path, const std::vector<std::string>& hashes, const int& web_server_id );

private:
    int dialog_level = 2;

    // id constants
    const int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    const std::unordered_map<int, QString> ws_names = {
        {this->APACHE_ID, "apache"},
        {this->NGINX_ID,  "nginx"},
        {this->IIS_ID,    "iis"} };


    // lists of used files' hashes
    // { web_server_id : { hashes } }
    std::unordered_map<int, std::vector<std::string>> hashes = {
        {this->APACHE_ID, {}},
        {this->NGINX_ID,  {}},
        {this->IIS_ID,    {}}
    };

};

#endif // HASH_H

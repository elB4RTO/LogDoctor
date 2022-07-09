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

    void setDialogLevel( const int new_level );

    // retrieve the lists from the database file
    bool loadUsedHashesLists( const std::string& db_path );

    // returns the hash
    std::string digestFile( const std::string& file_path );
    // check if the given hash is from a file which has been used already
    bool hasBeenUsed( const std::string& file_hash, const int web_server_id, const int log_type );
    // insert the given hash/es in the relative list
    bool insertUsedHash( QSqlDatabase& db, QSqlQuery& query, const QString& db_name, const std::string& hash, const int web_server_id, const int log_type );
    bool insertUsedHashes( const std::string& db_path, const std::unordered_map<int, std::vector<std::string>>& hashes, const int web_server_id );

private:
    int dialog_level = 2;

    // id constants
    const int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    const std::unordered_map<int, QString> ws_names = {
        {11, "apache"},
        {12, "nginx"},
        {13, "iis"} };
    const std::unordered_map<int, QString> log_types = {
        {1, "access"},
        {2, "error"} };


    // lists of used files' hashes
    std::unordered_map<int, std::unordered_map<int, std::vector<std::string>>> hashes;

};

#endif // HASH_H

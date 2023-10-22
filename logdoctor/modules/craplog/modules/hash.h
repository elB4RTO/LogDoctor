#ifndef LOGDOCTOR__CRAPLOG__HASH_H
#define LOGDOCTOR__CRAPLOG__HASH_H


#include "defines/web_servers.h"

#include <vector>
#include <unordered_map>

#include <QString>

class QSqlQuery;


//! HashOps
/*!
    Operations for the hashes
*/
class HashOps
{
public:

    //! Sets the new Dialogs level
    void setDialogLevel( const int new_level );

    //! Retrieves the lists of hashes from the database file
    /*!
        \param db_path The path of the log files' Hashes database
        \return Whether the operation has been successful or not
    */
    bool loadUsedHashesLists( const std::string& db_path );

    //! Returns the hash resulting from the content of the given file
    /*!
        \param file_path The path of the file to process
        \return The sha256 hash
        \throw GenericException
        \see SHA256
    */
    static void digestFile( const std::string& file_path, std::string& hash );

    //! Checks if the given hash equals one which is already in the list
    /*!
        \param file_hash The sha256 hash to compare
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the hash is already in the list or not
    */
    bool hasBeenUsed( const std::string& file_hash, const unsigned& web_server_id ) const;

    //! Inserts multiple hashes in the corresponding database table
    /*!
        \param db_path The path of the Hashes database
        \param hashes The list of hashes to insert
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the operation has been successful or not
    */
    bool insertUsedHashes( const std::string& db_path, const std::vector<std::string>& hashes, const unsigned& web_server_id );

private:

    // Quantity of information to display throught dialogs
    int dialog_level{ 2 };

    // List of Web Servers names for database tables
    const std::unordered_map<unsigned, QString> ws_names{
        {APACHE_ID, "apache"},
        {NGINX_ID,  "nginx"},
        {IIS_ID,    "iis"}
    };

    // Lists of used files' hashes
    // { web_server_id : { hashes } }
    std::unordered_map<unsigned, std::vector<std::string>> hashes{
        {APACHE_ID, {}},
        {NGINX_ID,  {}},
        {IIS_ID,    {}}
    };


    // Called by insertUsedHashes()
    // Inserts a hash in the corresponding database table
    bool insertUsedHash( QSqlQuery& query, const QString& db_name, const std::string& hash, const unsigned& web_server_id );

};


#endif // LOGDOCTOR__CRAPLOG__HASH_H

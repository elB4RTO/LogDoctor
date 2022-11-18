#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <unordered_map>

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


//! HashOps
/*!
    Operations for the hashes
*/
class HashOps
{
public:
    HashOps();


    //! Sets the new Dialogs level
    void setDialogLevel( const int& new_level );

    //! Retrieves the lists of hashes from the database file
    /*!
        \param db_path The path of the log files' Hashes database
        \return Whether the operation has been successful or not
        \see hashes
    */
    bool loadUsedHashesLists( const std::string& db_path );

    //! Returns the hash resulting from the content of the given file
    /*!
        \param file_path The path of the file to process
        \return The sha256 hash
        \throw GenericException
        \see SHA256
    */
    std::string digestFile( const std::string& file_path );

    //! Checks if the given hash equals one which is already in the list
    /*!
        \param file_hash The sha256 hash to compare
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the hash is already in the list or not
        \see hashes
    */
    bool hasBeenUsed( const std::string& file_hash, const int& web_server_id );

    //! Inserts a hashe in the corresponding database table
    /*!
        \param db_query Query instance, already initialized
        \param db_name The name of the database, eventually used by dialogs
        \param hash The hash to insert
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the operation has been successful or not
        \see insertUsedHashes()
    */
    bool insertUsedHash( QSqlQuery& query, const QString& db_name, const std::string& hash, const int& web_server_id );

    //! Inserts multiple hashes in the corresponding database table
    /*!
        \param db_path The path of the Hashes database
        \param hashes The list of hashes to insert
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the operation has been successful or not
        \see insertUsedHash()
    */
    bool insertUsedHashes( const std::string& db_path, const std::vector<std::string>& hashes, const int& web_server_id );

private:

    // Quantity of information to display throught dialogs
    int dialog_level = 2;

    // id constants
    const int APACHE_ID = 11;
    const int NGINX_ID  = 12;
    const int IIS_ID    = 13;

    // List of Web Servers names
    const std::unordered_map<int, QString> ws_names = {
        {this->APACHE_ID, "apache"},
        {this->NGINX_ID,  "nginx"},
        {this->IIS_ID,    "iis"} };


    // Lists of used files' hashes
    // { web_server_id : { hashes } }
    std::unordered_map<int, std::vector<std::string>> hashes = {
        {this->APACHE_ID, {}},
        {this->NGINX_ID,  {}},
        {this->IIS_ID,    {}}
    };

};

#endif // HASH_H

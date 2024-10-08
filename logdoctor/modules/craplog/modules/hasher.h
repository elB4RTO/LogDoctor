#ifndef LOGDOCTOR__CRAPLOG__MODULES__HASHER_H
#define LOGDOCTOR__CRAPLOG__MODULES__HASHER_H


#include "main_lib.h"

#include <vector>
#include <unordered_map>

#include <QString>


enum class WebServer;

class PathHandler;
class QSqlQuery;


//! Hasher
/*!
    Operations on the logs hashes
*/
class Hasher final
{
public:

    //! Sets the new Dialogs level
    void setDialogLevel( const DialogsLevel new_level ) noexcept;

    //! Retrieves the lists of hashes from the database file
    /*!
        \param db_path The path of the log files' Hashes database
        \return Whether the operation has been successful or not
        \throw VoidException
    */
    bool loadUsedHashesLists( const PathHandler& db_path ) noexcept;

    //! Returns the hash resulting from the content of the given file
    /*!
        \param file_path The path of the file to process
        \return The sha256 hash
        \throw GenericException
        \see SHA256
    */
    static void digestFile( const PathHandler& file_path, std::string& hash );

    //! Checks if the given hash equals one which is already in the list
    /*!
        \param file_hash The sha256 hash to compare
        \param web_server_id The ID of the Web Server which generated the file
        \return Whether the hash is already in the list or not
    */
    bool hasBeenUsed( const std::string& file_hash, const WebServer& web_server ) const noexcept;

    //! Inserts multiple hashes in the corresponding database table
    /*!
        \param db_path The path of the Hashes database
        \param hashes The list of hashes to insert
        \param web_server_id The ID of the Web Server which generated the file
        \throw VoidException
    */
    void insertUsedHashes( const PathHandler& db_path, const std::vector<std::string>& hashes, const WebServer& web_server );

private:

    // Quantity of information to display throught dialogs
    DialogsLevel dialogs_level{ DL_NORMAL };

    // List of Web Servers names for database tables
    const std::unordered_map<WebServer, QString> ws_names{
        {WS_APACHE, "apache"},
        {WS_NGINX,  "nginx"},
        {WS_IIS,    "iis"}
    };

    // Lists of used files' hashes
    // { web_server_id : { hashes } }
    std::unordered_map<WebServer, std::vector<std::string>> hashes{
        {WS_APACHE, {}},
        {WS_NGINX,  {}},
        {WS_IIS,    {}}
    };

};


#endif // LOGDOCTOR__CRAPLOG__MODULES__HASHER_H

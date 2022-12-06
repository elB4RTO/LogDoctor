#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include <filesystem>


//! IOUtils
/*!
    Utilities to work on files
*/
class IOutils
{
public:
    IOutils();

    //! Checks the existence of a file/folder
    /*!
        \param path The path of the entry
        \return The result of the check
    */
    static const bool exists( const std::string& path );

    //! Checks if a path exists and points to a file
    /*!
        \param path The path of the entry
        \return The result of the check
    */
    static const bool isFile( const std::string& path );

    //! Checks if a path exists and points to a directory
    /*!
        \param path The path of the entry
        \return The result of the checks
    */
    static const bool isDir( const std::string& path );

    //! Checks if a path exists, if it points to a file and if the user has read and/or write permissions on it
    /*!
        \param path The path of the entry
        \param readable Set to true to check for readability
        \param writable Set to true to check for writability
        \return The result of the checks
    */
    static const bool checkFile( const std::string& path, const bool& readable=false, const bool& writable=false );

    //! Checks if a path exists, if it points to a folder and if the user has read and/or write permissions on it
    /*!
        \param path The path of the entry
        \param readable Set to true to check for readability
        \param writable Set to true to check for writability
        \return The result of the checks
    */
    static const bool checkDir( const std::string& path, const bool& readable=false, const bool& writable=false );

    //! Creates a directory
    /*!
        \param path The path of the new entry
        \return Wheter the operation was successful or not
    */
    static const bool makeDir( const std::string& path, std::error_code& err ) noexcept(true);

    //! Renames an entry with a trailing '.copy'
    /*!
        \param path The path of the entry to rename
        \param err Will hold the error, if any
        \return Wheter the operation was successful or not
    */
    static const bool renameAsCopy( const std::string& path, std::error_code& err ) noexcept(true);

    //! Randomly pick lines from a file
    /*!
        \param path The path of the entry
        \param lines Will hold the picked lines
        \param n_lines The number of lines to pick
        \param strip_lines Whether to strip control-characters away from the lines
    */
    static void randomLines( const std::string& path, std::vector<std::string>& lines, const int& n_lines=16, const bool& strip_lines=true );

    //! Reads the content of a file
    /*!
        \param path The path of the file to read from
        \param content Will hold the content of the file
    */
    static void readFile( const std::string& path, std::string& content );

    //! Writes a string on file
    /*!
        \param path The path of the file to write on
        \param content The string to write
    */
    static void writeOnFile( const std::string& path, const std::string& content );
};

#endif // IO_H

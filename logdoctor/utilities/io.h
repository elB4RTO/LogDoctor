#ifndef LOGDOCTOR__UTILITIES__IO_H
#define LOGDOCTOR__UTILITIES__IO_H


#include <string>
#include <vector>
#include <filesystem>


//! IOUtils
/*!
    Utilities to work on files
*/
namespace IOutils
{

//! Checks the existence of a file/folder
/*!
    \param path The path of the entry
    \return The result of the check
*/
inline bool exists( std::string_view path ) noexcept
{
    return path.empty()
         ? false
         : std::filesystem::exists( path );
}

//! Checks if a path exists and points to a file
/*!
    \param path The path of the entry
    \return The result of the check
*/
inline bool isFile( std::string_view path ) noexcept
{
    return exists( path )
         ? std::filesystem::is_regular_file( path )
         : false;
}

//! Checks if a path exists and points to a directory
/*!
    \param path The path of the entry
    \return The result of the checks
*/
inline bool isDir( std::string_view path ) noexcept
{
    return exists( path )
         ? std::filesystem::is_directory( path )
         : false;
}

//! Checks if a path exists, if it points to a file and if the user has read and/or write permissions on it
/*!
    \param path The path of the entry
    \param readable Set to true to check for readability
    \param writable Set to true to check for writability
    \return The result of the checks
*/
bool checkFile( std::string_view path, const bool readable=false, const bool writable=false ) noexcept;

//! Checks if a path exists, if it points to a folder and if the user has read and/or write permissions on it
/*!
    \param path The path of the entry
    \param readable Set to true to check for readability
    \param writable Set to true to check for writability
    \return The result of the checks
*/
bool checkDir( std::string_view path, const bool readable=false, const bool writable=false ) noexcept;

//! Creates a directory
/*!
    \param path The path of the new entry
    \return Wheter the operation was successful or not
*/
bool makeDir( std::string_view path, std::error_code& err ) noexcept;

//! Renames an entry with a trailing '.copy'
/*!
    \param path The path of the entry to rename
    \param err Will hold the error, if any
    \return Wheter the operation was successful or not
*/
bool renameAsCopy( std::string_view path, std::error_code& err ) noexcept;

//! Randomly pick lines from a file
/*!
    \param path The path of the entry
    \param lines Will hold the picked lines
    \param n_lines The number of lines to pick
    \param strip_lines Whether to strip control-characters away from the lines
*/
void randomLines( const std::string& path, std::vector<std::string>& lines, const size_t n_lines=16ul, const bool strip_lines=true );

//! Reads the content of a file
/*!
    \param path The path of the file to read from
    \param content Will hold the content of the file
*/
void readFile( const std::string& path, std::string& content );

//! Writes a string on file
/*!
    \param path The path of the file to write on
    \param content The string to write
*/
void writeOnFile( const std::string& path, std::string_view content );

} // namespace IOutils


#endif // LOGDOCTOR__UTILITIES__IO_H

#ifndef IO_H
#define IO_H

#include <string>
#include <vector>


class IOutils
{
public:
    IOutils();

    // checks the existence of a path
    static const bool exists( const std::string& path );
    // checks if a path exists and it points to a file
    static const bool isFile( const std::string& path );
    // checks if a path exists and it points to a folder
    static const bool isDir( const std::string& path );
    // checks if a path exists and it points to a file and the file is readable/writable
    static const bool checkFile( const std::string& path, const bool& readable=false, const bool& writable=false );
    // checks if a path exists and it points to a folder and the folder is readable/writable
    static const bool checkDir( const std::string& path, const bool& readable=false, const bool& writable=false );

    // rename an entry with a trailing '.copy'
    static const bool renameAsCopy( const std::string& path ) noexcept(true);

    // read a defined amount of lines (from the start)
    static void randomLines( const std::string& path, std::vector<std::string>& lines, const int& n_lines=16, const bool& strip_lines=true );
    // read the entire content
    static void readFile( const std::string& path, std::string& content );

    // write a string on file
    static void writeOnFile( const std::string& path, const std::string& content );
};

#endif // IO_H

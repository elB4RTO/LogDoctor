#ifndef IO_H
#define IO_H

#include <string>
#include <vector>


class IOutils
{
public:
    IOutils();

    // checks the existence of a path
    static bool exists( const std::string& path );
    // checks if a path exists and it points to a file
    static bool isFile( const std::string& path );
    // checks if a path exists and it points to a folder
    static bool isDir( const std::string& path );
    // checks if a path exists and it points to a file and the file is readable/writable
    static bool checkFile( const std::string& path, const bool readable=false, const bool writable=false );
    // checks if a path exists and it points to a folder and the folder is readable/writable
    static bool checkDir( const std::string& path, const bool readable=false, const bool writable=false );

    // read a defined amount of lines (from the start)
    static std::vector<std::string> readLines( const std::string& path, const int n_lines=16 ) throw();
    // read the entire content
    static std::string readFile( const std::string& path ) throw();
};

#endif // IO_H

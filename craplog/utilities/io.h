#ifndef IO_H
#define IO_H

#include <string>
#include <vector>


class IOutils
{
public:
    IOutils();

    // checks the existence of a path
    static bool exists( std::string path );
    // checks if a path exists and it points to a file
    static bool isFile( std::string path );
    // checks if a path exists and it points to a folder
    static bool isDir( std::string path );
    // checks if a path exists and it points to a file and the file is readable/writable
    static bool checkFile( std::string path, bool readable=false, bool writable=false );
    // checks if a path exists and it points to a folder and the folder is readable/writable
    static bool checkDir( std::string path, bool readable=false, bool writable=false );

    // read a defined amount of lines (from the start)
    static std::vector<std::string> readLines( std::string path, int n_lines=10 );
    // read the entire content
    static std::string readFile( std::string path );
};

#endif // IO_H

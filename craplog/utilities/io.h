#ifndef IO_H
#define IO_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>


class IOutils
{
public:
    IOutils();
    
    static std::vector<std::string> readLines( std::string path, int n_lines=10 );
    static std::string readFile( std::string path );
};

#endif // IOUTILS_H

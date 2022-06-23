#ifndef IO_H
#define IO_H

#include <string>
#include <iostream>
#include <fstream>


class IOutils
{
public:
    IOutils();
    
    static std::string readFile( std::string path );
};

#endif // IOUTILS_H

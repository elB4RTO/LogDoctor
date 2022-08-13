#ifndef GZIP_H
#define GZIP_H

#include <string>


class GZutils
{
public:
    GZutils();

    static void readFile( const std::string& path, std::string& content );
};

#endif // GZIP_H

#ifndef GZIP_H
#define GZIP_H

#include <string>


class GzipOps
{
public:
    GzipOps();

    static void readFile( const std::string& path, std::string& content );
};

#endif // GZIP_H

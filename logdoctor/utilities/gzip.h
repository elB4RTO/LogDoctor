#ifndef GZIP_H
#define GZIP_H

#include <string>


//! GZUtils
/*!
    Utilities to work on GZipped files
*/
class GZutils
{
public:
    GZutils();

    //! Reads a GZipped file
    /*!
        \param path The path of the file
        \param content Will hold the content of the file
    */
    static void readFile( const std::string& path, std::string& content );
};

#endif // GZIP_H

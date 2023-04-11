#ifndef GZIP_H
#define GZIP_H

#include <string>


//! GZUtils
/*!
    Utilities to work on GZipped files
*/
namespace GZutils
{

//! Reads a GZipped file
/*!
    \param path The path of the file
    \param content Will hold the content of the file
*/
void readFile( const std::string& path, std::string& content );

} // namespace GZutils

#endif // GZIP_H

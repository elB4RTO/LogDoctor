#ifndef LOGDOCTOR__GLOBALS__DB_NAMES_H
#define LOGDOCTOR__GLOBALS__DB_NAMES_H


class QString;


//! DatabasesNames
/*!
    Names of the databases
*/
namespace DatabasesNames {

extern const char* data;

extern const char* hashes;

} // namespace DatabasesNames


//! DatabasesConnections
/*!
    Names of the databases connections
*/
namespace DatabasesConnections {

    extern const QString data;

    extern const QString hashes;

    extern const char* cdata;

} // namespace DatabasesConnections


#endif // LOGDOCTOR__GLOBALS__DB_NAMES_H

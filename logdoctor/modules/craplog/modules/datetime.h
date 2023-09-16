#ifndef LOGDOCTOR__CRAPLOG__DATETIME_H
#define LOGDOCTOR__CRAPLOG__DATETIME_H


#include <string>
#include <vector>


//! DateTimeOps
/*!
    Operations for the dates
*/
namespace DateTimeOps
{

//! Returns a standardized list of items representing the given date and time
/*!
    \param datetime The given date and time string
    \param  format The format of the given string
    \throw DateTimeException
    \return The list of items
*/
const std::vector<std::string> processDateTime( std::string_view datetime, std::string_view format );

} // namespace DateTimeOps


#endif // LOGDOCTOR__CRAPLOG__DATETIME_H

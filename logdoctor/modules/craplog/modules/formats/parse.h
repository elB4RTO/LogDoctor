#ifndef LOGDOCTOR__CRAPLOG__MODULES__FORMATS__PARSE_H
#define LOGDOCTOR__CRAPLOG__MODULES__FORMATS__PARSE_H


#include "modules/craplog/lib.h"

#include <string>


struct LogsFormat;


namespace FormatOps::Private
{

//! Parses the given string to extrapolate the format for Apache2
/*!
    \param format_string The format string to process
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat parseApacheFormatString( const std::string& format_string );

//! Parses the given string to extrapolate the format for Nginx
/*!
    \param format_string The format string to process
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat parseNginxFormatString( const std::string& format_string );

//! Parses the given string to extrapolate the format for the IIS
/*!
    \param format_string The format string to process
    \param log_module The ID of the log module to use
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat parseIisFormatString( const std::string& format_string, const IISLogsModule log_module );

} // namespace FormatOps::Private


#endif // LOGDOCTOR__CRAPLOG__MODULES__FORMATS__PARSE_H

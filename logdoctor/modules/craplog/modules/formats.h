#ifndef LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H
#define LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H


#include "modules/craplog/lib.h"

#include <string>


enum class WebServer;

struct LogsFormat;

class QString;


//! FormatOps
/*!
    Operations for the logs formats
*/
namespace FormatOps
{

//! Processes the given string to extrapolate the format for Apache2
/*!
    \param format_string The format string to process
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat processApacheFormatString( const std::string& format_string );

//! Processes the given string to extrapolate the format for Nginx
/*!
    \param format_string The format string to process
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat processNginxFormatString( const std::string& format_string );

//! Processes the given string to extrapolate the format for IIS
/*!
    \param format_string The format string to process
    \param log_module The ID of the log module to use
    \return The logs format
    \throw LogFormatException
    \see LogsFormat
*/
LogsFormat processIisFormatString( const std::string& format_string, const IISLogsModule logs_module );


//! Returns a log line sample based on the given format
/*!
    \param web_server The target web server
    \param logs_format The logs format to use to build the sample
    \return The sample line
    \throw DoNotCatchException
    \see LogsFormat
*/
QString getLogLineSample( const WebServer& web_server, const LogsFormat& logs_format );

} // namespace FormatOps


#endif // LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H

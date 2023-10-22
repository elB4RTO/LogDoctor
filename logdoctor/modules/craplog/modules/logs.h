#ifndef LOGDOCTOR__CRAPLOG__LOGS_H
#define LOGDOCTOR__CRAPLOG__LOGS_H


#include "lib.h"


//! LogOps
/*!
    Operations for the logs
*/
namespace LogOps
{

//! Defines the type of a file
/*!
    \param lines A list of (randomly picked) lines from the file to examine
    \param format The logs format to use to determine if the file is valid or not
    \return The resulting file type
    \see LogType, deepTypeCheck(), FormatOps::LogsFormat
*/
LogType defineFileType(
    const std::vector<std::string>& lines,
    const LogsFormat& format
);

} // namespace LogOps


#endif // LOGDOCTOR__CRAPLOG__LOGS_H

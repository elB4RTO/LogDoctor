#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "lib.h"


//! LogOps
/*!
    Operations for the logs
*/
class LogOps
{
public:
    explicit LogOps();


    //! Defines the type of a file
    /*!
        \param lines A list of (randomly picked) lines from the file to examine
        \param format The logs format to use to determine if the file is valid or not
        \return The resulting file type
        \see LogType, deepTypeCheck(), FormatOps::LogsFormat
    */
    static const LogType defineFileType(
        const std::vector<std::string>& lines,
        const LogsFormat& format
    );


private:

    //! Parse the given line using the given format
    /*!
        \param line The log line to check
        \param format The logs format to use
        \return Whether the line respects the format or not
        \see defineFileType(), FormatOps::LogsFormat
    */
    static const bool deepTypeCheck(
        const std::string& line,
        const LogsFormat& format
    );

};


#endif // LOGS_H

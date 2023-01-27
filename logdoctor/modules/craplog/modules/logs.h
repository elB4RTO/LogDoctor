#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "modules/craplog/modules/formats.h"


//! LogOps
/*!
    Operations for the logs
*/
class LogOps
{
public:
    explicit LogOps();


    //! Enumerates log file types
    /*!
        File types used to decide whether a file should be considered valid or not
        \see defineFileType()
    */
    enum LogType {
        Failed    = -1, //!< Failed to determine the type
        Discarded =  0, //!< Not a valid file, will be discarded
        Access    =  1  //!< Valid access logs file type
    };

    //! Defines the type of a file
    /*!
        \param lines A list of (randomly picked) lines from the file to examine
        \param format The logs format to use to determine if the file is valid or not
        \return The resulting file type
        \see LogType, deepTypeCheck(), FormatOps::LogsFormat
    */
    const LogType defineFileType(
        const std::vector<std::string>& lines,
        const FormatOps::LogsFormat& format
    ) const;


private:

    //! Parse the given line using the given format
    /*!
        \param line The log line to check
        \param format The logs format to use
        \return Whether the line respects the format or not
        \see defineFileType(), FormatOps::LogsFormat
    */
    const bool deepTypeCheck(
        const std::string& line,
        const FormatOps::LogsFormat& format
    ) const;

};

#endif // LOGS_H

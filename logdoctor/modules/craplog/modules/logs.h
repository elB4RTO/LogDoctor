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


    //! Receives the mutex to be shared with Craplog
    /*!
        \param craplog_mutex The mutex from Craplog
    */
    void setMutex( std::mutex* craplog_mutex=nullptr );


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

    //! Removes commented lines from the given list
    /*!
        \param lines The lines to clean
    */
    void cleanLines(
        std::vector<std::string>& lines
    ) const;

    //! Parses log lines to extract data
    /*!
        \param data The data collection which will hold the data
        \param lines The list of lines to parse
        \param format The logs format to use
        \throw LogParserException
        \see parseLine(), Craplog::parseLogLines(), FormatOps::LogsFormat
    */
    void parseLines(
        std::vector<std::unordered_map<int, std::string>>& data,
        const std::vector<std::string>& lines,
        const FormatOps::LogsFormat& format
    );

    //! Resets the performances data
    void resetPerfData();

    // share perf data with craplog
    const unsigned getTotalSize();   //!< Returns the total size of the logs lines. \see total_size
    const unsigned getParsedSize();  //!< Returns the parsed logs size. \see parsed_size
    const unsigned getParsedLines(); //!< Returns the number of parsed log lines. \see parsed_lines

private:

    // Map to convert log fields to field IDs
    const std::unordered_map<std::string, int> field2id = {
        // date-time
        {"date_time_year",     1},
        {"date_time_month",    2},
        {"date_time_day",      3},
        {"date_time_hour",     4},
        {"date_time_minute",   5},
        {"date_time_second",   6},
            {"date_time_ncsa",        0},
            {"date_time_iso",         0},
            {"date_time_mcs",         0},
            {"date_time_gmt",         0},
            {"date_time_utc_d",       0},
            {"date_time_utc_t",       0},
            {"date_time_epoch_s",     0},
            {"date_time_epoch_s.ms",  0},
            {"date_time_epoch_ms",    0},
            {"date_time_epoch_us",    0},
            {"date_time_YYYYMMDD",    0},
            {"date_time_MMDDYY",      0},
            {"date_time_MDYY",        0},
            {"date_time_year_short",  0},
            {"date_time_month_str",   0},
            {"date_time_clock_12",    0},
            {"date_time_clock_24",    0},
            {"date_time_clock_short", 0},
        // request
        {"request_protocol",   10},
        {"request_method",     11},
        {"request_uri",        12},
        {"request_query",      13},
        {"response_code",      14},
            {"request_full",       0},
        // performance
        {"time_taken_ms",      15},
            {"time_taken_us",      0},
            {"time_taken_s.ms",    0},
            {"time_taken_s",       0},
        {"bytes_sent",         16},
        {"bytes_received",     17},
        // referer
        {"referer",            18},
        // client data
        {"client",             20},
        {"user_agent",         21},
        {"cookie",             22}
    };

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

    //! Parses a line to extract data
    /*!
        \param line The log line to parse
        \param format The logs format to use
        \return A data collection item
        \throw LogParserException
        \see parseLines(), Craplog::data_collection, FormatOps::LogsFormat
    */
    const std::unordered_map<int, std::string> parseLine(
        const std::string& line,
        const FormatOps::LogsFormat& format
    );

    // a mutex shared with craplog
    std::mutex* mutex = nullptr;

    // temporary vars
    unsigned total_size=0;   //!< Total size of the parsed logs. \see getTotalSize()
    unsigned parsed_size=0;  //!< Size of the parsed logs. \see getParsedSize()
    unsigned parsed_lines=0; //!< Number of parsed logs lines. \see getParsedLines()


};

#endif // LOGS_H

#ifndef LOGS_H
#define LOGS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "utilities/strings.h"
#include "tools/craplog/modules/formats.h"


class LogOps
{
public:
    LogOps();

    // log file types
    enum LogType {
        Failed    = -1,
        Discarded =  0,
        Access    =  1
    };

    // define if really access and/or error logs
    LogType defineFileType(
        const std::string& name,
        const std::vector<std::string>& lines,
        const FormatOps::LogsFormat& format );

    // remove commented lines
    void cleanLines(
        std::vector<std::string>& lines );

    // parse log lines to get data
    void parseLines(
        std::vector<std::unordered_map<int, std::string>>& data,
        const std::vector<std::string>& lines,
        const FormatOps::LogsFormat& format );

    void resetPerfData();
    // share perf data with craplog
    const unsigned getSize(),
                   getLines();

private:

    // from fields to IDs
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

    bool deepTypeCheck(
        const std::string& line,
        const FormatOps::LogsFormat& format );

    // job related
    const std::unordered_map<int, std::string> parseLine(
        const std::string& line,
        const FormatOps::LogsFormat& format
        );
    // temporary vars
    unsigned size=0, lines=0;


};

#endif // LOGS_H

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
        Failed=0,
        Access=1,
        Error=2
    };

    // define if really access and/or error logs
    LogType defineFileType(
        const std::string& name,
        const std::vector<std::string>& lines,
        std::unordered_map<int, FormatOps::LogsFormat>& format
        );

    // parse log lines to get data
    std::vector<std::unordered_map<int, std::string>> parseLines(
        const std::vector<std::string>& lines,
        const FormatOps::LogsFormat& format
        );

    void resetPerfData();
    // share perf data with craplog
    int getSize(),
        getLines();

private:
    // types constants
    const unsigned int ACCESS_LOGS=1, ERROR_LOGS=2;

    std::unordered_map<std::string, int> field2id;

    bool deepTypeCheck(
        const std::string& line,
        const FormatOps::LogsFormat& format );

    LogType comparativeTypeCheck(
        const std::string& line,
        const FormatOps::LogsFormat& access_format,
        const FormatOps::LogsFormat& error_format );

    // job related
    std::unordered_map<int, std::string> parseLine(
        const std::string& line,
        const FormatOps::LogsFormat& format
        );
    // temporary vars
    int size, lines;


};

#endif // LOGS_H

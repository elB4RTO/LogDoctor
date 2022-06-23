#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <filesystem>

#include <QMainWindow>


class Craplog
{
public:
    Craplog();

    // logs formats
    enum LogsFormat {
        AccessCommon,
        AccessCombined,
        ErrorDefault
    };
    LogsFormat access_logs_format;
    LogsFormat error_logs_format;

    // log file infoes
    enum LogType {
        Failed,
        Access,
        Error
    };

    struct LogFile {
        bool selected;
        int size;
        QString name;
        std::string path;
        LogType type;
    };
    // logs list related methods
    std::vector<LogFile> getLogsList( bool fresh=false );
    int getLogsListSize();
    std::string getLogFilePath( QString file_name );
    int setLogFileSelected( QString file_name );

private:
    // logs related
    std::string logs_path;
    std::vector<LogFile> logs_list;
    // logs list related
    void scanLogsDir();

    void readConfigs();

    void loadFileContent();
};

#endif // CRAPLOG_H

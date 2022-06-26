#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <QMainWindow>

#include "tools/craplog/modules/hash.h"
#include "tools/craplog/modules/logs.h"


class Craplog
{
public:
    Craplog();

    // logs formats
    enum Format{
        Default=10,
        Common=11,
        Combined=12,
        Custom=13
    };
    struct LogsFormat {
        Format format;
        std::string string;
        std::string initial;
        std::string final;
        std::vector<std::string> separators;
        std::vector<std::string> fields;
    };
    void setAccessLogsFormat( std::string format_string );
    void setErrorLogsFormat( std::string format_string );
    LogsFormat getAccessLogsFormat();
    LogsFormat getErrorLogsFormat();


    // log file infoes
    enum LogType {
        Failed=0,
        Access=1,
        Error=2
    };
    // log file item
    struct LogFile {
        bool selected;
        int size;
        QString name;
        std::string hash;
        std::string path;
        LogType type;
    };
    // logs list related methods
    std::vector<LogFile> getLogsList( bool fresh=false );
    int getLogsListSize();
    // log file related
    LogFile getLogFileItem( QString file_name );
    std::string getLogFilePath( QString file_name );
    // set a file in the list as selected, to be used for the stats
    int setLogFileSelected( QString file_name );
    // check if a file name respects the one set
    bool isFileNameValid( std::string name );
    // define if really access and/or error logs
    Craplog::LogType defineFileType( std::string name, std::vector<std::string> lines );

    // logs usage control
    HashOps hashOps;

    // operations on logs
    LogOps logOps;

private:
    // configs location
    std::string configs_path="~/.config/craplog";

    // logs related
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_base_names;
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_paths;
    std::vector<LogFile> logs_list;
    // logs list related
    void scanLogsDir();

    // logs format related
    LogsFormat access_logs_format;
    LogsFormat error_logs_format;

    void readConfigs();

    void loadFileContent();
};

#endif // CRAPLOG_H

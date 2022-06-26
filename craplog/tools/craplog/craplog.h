#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <QMainWindow>

#include "tools/craplog/modules/formats.h"
#include "tools/craplog/modules/hash.h"
#include "tools/craplog/modules/logs.h"


class Craplog
{
public:
    Craplog();

    // web servers
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    // logs formats
    // for settings
    void setAccessLogsFormat( int web_server_id, std::string format_string );
    void setErrorLogsFormat( int web_server_id, std::string format_string );
    FormatOps::LogsFormat getAccessLogsFormat( int web_server_id );
    FormatOps::LogsFormat getErrorLogsFormat( int web_server_id );
    // currently used
    void setCurrentWSID( int web_server_id );
    int  getCurrentWSID();
    FormatOps::LogsFormat getCurrentALF();
    FormatOps::LogsFormat getCurrentELF();


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
    std::string configs_path;

    // web server related
    int current_WS = 11;

    // logs related
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_base_names;
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_paths;
    std::vector<LogFile> logs_list;
    // logs list related
    void scanLogsDir();

    // logs format related
    std::unordered_map<int, std::unordered_map<int, FormatOps::LogsFormat>> logs_formats;
    void setCurrentALF();
    void setCurrentELF();
    FormatOps::LogsFormat current_ALF;
    FormatOps::LogsFormat current_ELF;

    void readConfigs();

    void loadFileContent();
};

#endif // CRAPLOG_H

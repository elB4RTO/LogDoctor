#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <QMainWindow>

#include "modules/dialogs.h"

#include "tools/craplog/modules/formats.h"
#include "tools/craplog/modules/hash.h"
#include "tools/craplog/modules/logs.h"


class Craplog : public QWidget
{
public:
    Craplog();

    // web servers ID constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    // logs formats web server specific settings
    void setAccessLogsFormat( int web_server_id, std::string format_string );
    void setErrorLogsFormat( int web_server_id, std::string format_string );
    FormatOps::LogsFormat getAccessLogsFormat( int web_server_id );
    FormatOps::LogsFormat getErrorLogsFormat( int web_server_id );
    // currently used WS and LF
    void setCurrentWSID( int web_server_id );
    int  getCurrentWSID();
    FormatOps::LogsFormat getCurrentALF();
    FormatOps::LogsFormat getCurrentELF();

    // log type constants
    const unsigned int FAILED=0, ACCESS_LOGS=1, ERROR_LOGS=2;
    // log file item's infoes
    struct LogFile {
        bool selected;
        int size;
        QString name;
        std::string hash;
        std::string path;
        LogOps::LogType type;
    };
    // logs list related methods
    std::vector<LogFile> getLogsList( bool fresh=false );
    int getLogsListSize();
    // log file related
    LogFile getLogFileItem( QString file_name );
    std::string getLogFilePath( QString file_name );
    // set a file in the list as selected, to be used for the stats
    bool setLogFileSelected( QString file_name );
    // check if a file name respects the one set
    bool isFileNameValid( std::string name );

    // warning file size (in Bytes)
    int getWarningSize();
    void setWarningSize( int new_size );
    // logs usage control
    HashOps hashOps;

    // operations on logs
    LogOps logOps;

private:
    // quantoty of informational dialogs to display
    int dialogs_Level = 1; // 0: essential, 1: usefull, 2: explanatory

    // configs location
    std::string configs_path;

    // web server related
    int current_WS = 11;

    // logs related
    int warning_size = 1'048'576; //104'857'600; // in Bytes ( => 100 MiB )
    // web server specific paths and file names
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_base_names;
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_paths;
    // logs list related
    std::vector<LogFile> logs_list;
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

#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>
#include "sqlite3.h"

#include <QMainWindow>

#include "modules/dialogs.h"
#include "tools/craplog/modules/formats.h"
#include "tools/craplog/modules/hash.h"
#include "tools/craplog/modules/logs.h"


class Craplog
{
public:
    Craplog();

    void run();

    // web servers ID constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    // logs formats web server specific settings
    void setAccessLogsFormat( const int web_server_id, const std::string& format_string );
    void setErrorLogsFormat( const int web_server_id, const std::string& format_string );
    FormatOps::LogsFormat getAccessLogsFormat( const int web_server_id ) &;
    FormatOps::LogsFormat getErrorLogsFormat( const int web_server_id ) &;
    // currently used WS and LF
    void setCurrentWSID( const int web_server_id );
    int  getCurrentWSID() const;
    FormatOps::LogsFormat getCurrentALF() const&;
    FormatOps::LogsFormat getCurrentELF() const&;

    // log type constants
    const unsigned int FAILED=0, ACCESS_LOGS=1, ERROR_LOGS=2;
    // log file item's infoes
    struct LogFile {
        bool selected;
        bool used_already;
        int size;
        QString name;
        std::string hash;
        std::string path;
        LogOps::LogType type;
    };
    // logs list related methods
    std::vector<LogFile> getLogsList( const bool fresh=false );
    int getLogsListSize();
    // log file related
    LogFile getLogFileItem( const QString& file_name );
    std::string getLogFilePath( const QString& file_name );
    // set a file in the list as selected, to be used for the stats
    bool setLogFileSelected( const QString& file_name );
    // check if a file name respects the one set
    bool isFileNameValid( const std::string& name, const LogOps::LogType& log_type );

    // warning file size (in Bytes)
    int getWarningSize();
    void setWarningSize( const int new_size );
    // logs usage control
    HashOps hashOps;

    // operations on logs
    LogOps logOps;

    // blacklist / warnlist related
    struct BWlist {
        bool used;
        std::vector<std::string> list;
    };
    bool isBlacklistUsed( const int web_server_id, const int log_type, const int log_field_id ),
         isWarnlistUsed( const int web_server_id, const int log_type, const int log_field_id );
    const std::vector<std::string>
        & getBlacklist( const int web_server_id, const int log_type, const int log_field_id ),
        & getWarnlist( const int web_server_id, const int log_type, const int log_field_id );

    // job related
    void startWorking(),
         stopWorking();
    bool isWorking(),
         isParsing();
    // job performancea
    void collectPerfData(),
         sumIgnoredSize( const int size, const int log_type );
    int getPerfSize(),
        getTotalSize(),
        getParsedSize(),
        getParsedLines(),
        getAccessSize(),
        getErrorSize();

private:
    // job related
    bool working = false,
         parsing = false,
         proceed = false;
    // perf related
    int perf_size = 0,
        total_size = 0,
        parsed_size = 0,
        access_size = 0,
        error_size = 0,
        ignored_access_size = 0,
        ignored_error_size = 0,
        total_lines = 0,
        parsed_lines = 0,
        access_lines = 0,
        error_lines = 0;
    // data collection
    /* structure
     *      { log_type_id : [ { log_field_id : "data" } ] }
     * log_type_ids
     *      1: access_logs, 2: error_logs
     * log_field_ids
     *      99: warning,
     *      1: year, 2: month, 3: day, 4: hour, 5: minute, 6:second,
     *      10: request_protocol, 11: request_method, 12: request_page, 13: request_query, 14: response_code,
     *      15: time_taken, 16: bytes_sent, 17: bytes_received, 18: referrer,
     *      20: client, 21: user_agent, 22: cookie,
     *      30: port, 31: error_level, 32: error_message, 33: source_file
    */
    std::unordered_map<int, std::vector<std::unordered_map<int, std::string>>> data_collection;
    std::vector<std::string> access_logs_lines, error_logs_lines;
    void joinLogLines(),
         parseLogLines(),
         storeLogLines();
    // used files
    std::unordered_map<int, std::vector<std::string>> used_files_hashes;

    // quantoty of informational dialogs to display
    int dialogs_Level = 1; // 0: essential, 1: usefull, 2: explanatory

    // configs location
    std::string configs_path;

    // web server related
    int current_WS = 11;

    // logs related
    bool delete_old_hashes = false;
    int  old_hashes_months = 12;
    int warning_size = 1'048'576; //104'857'600; // in Bytes ( => 100 MiB )
    // black/warn-list
    // { web_server_id : { log_type : { log_field_id : BWlist } } }
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, BWlist>>>
        blacklists,
        warnlists;


    // web server specific paths
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_paths;
    // web server specific file names criterions
    struct LogName {
        std::string starts;
        std::string contains;
        std::string ends;
    };
    std::unordered_map<int, std::unordered_map<int, LogName>> logs_base_names;
    // logs list related
    std::vector<LogFile> logs_list;
    void scanLogsDir();

    // logs format related
    FormatOps formatOps;
    std::unordered_map<int, std::unordered_map<int, std::string>> logs_format_stings;
    std::unordered_map<int, std::unordered_map<int, FormatOps::LogsFormat>> logs_formats;
    void setCurrentALF();
    void setCurrentELF();
    FormatOps::LogsFormat current_ALF;
    FormatOps::LogsFormat current_ELF;

    void readConfigs();

    void loadFileContent();
};

#endif // CRAPLOG_H

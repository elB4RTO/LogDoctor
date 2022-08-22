#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <QMainWindow>
#include <QtCharts>

#include "tools/craplog/modules/formats.h"
#include "tools/craplog/modules/hash.h"
#include "tools/craplog/modules/logs.h"


class Craplog
{
public:
    Craplog();

    void run();

    const int& getDialogsLevel();
    void setDialogsLevel( const int& new_level );

    const std::string
        & getStatsDatabasePath(),
        & getHashesDatabasePath();
    void setStatsDatabasePath( const std::string& path ),
         setHashesDatabasePath( const std::string& path );

    // logs formats web server specific settings
    void setApacheLogFormat( const std::string& format_string );
    void setNginxLogFormat(  const std::string& format_string );
    void setIisLogFormat(    const std::string& format_string, const int& log_module );
    const std::string& getLogsFormatString( const int& web_server_id );
    const FormatOps::LogsFormat& getLogsFormat( const int& web_server_id );
    const QString getLogsFormatSample( const int& web_server_id );
    // currently used WS and LF
    void setCurrentWSID( const int& web_server_id );
    const int& getCurrentWSID();
    const FormatOps::LogsFormat& getCurrentLogFormat();

    // logs paths
    const std::string& getLogsPath( const int& web_server );
    void setLogsPath( const int& web_server, const std::string& new_path );

    // log file item's infoes
    struct LogFile {
        bool selected;
        bool used_already;
        int size;
        QString name;
        std::string hash;
        std::string path;
    };
    // logs list related methods
    const std::vector<LogFile>& getLogsList( const bool& fresh=false );
    const int getLogsListSize();
    // log file related
    const LogFile& getLogFileItem( const QString& file_name );
    const std::string& getLogFilePath( const QString& file_name );
    // set a file in the list as selected, to be used for the stats
    const bool setLogFileSelected( const QString& file_name );
    // check if a file name respects the one set
    const bool isFileNameValid( const std::string& name );

    // warning file size (in Bytes)
    const long& getWarningSize();
    void setWarningSize( const long& new_size );
    // logs usage control
    HashOps hashOps;

    // operations on logs
    LogOps logOps;

    // blacklist / warnlist related
    struct BWlist {
        bool used;
        std::vector<std::string> list;
    };
    const bool& isBlacklistUsed( const int& web_server_id, const int& log_field_id ),
              & isWarnlistUsed( const int& web_server_id, const int& log_field_id );
    void setBlacklistUsed( const int& web_server_id, const int& log_field_id, const bool& used ),
         setWarnlistUsed( const int& web_server_id, const int& log_field_id, const bool& used );
    const std::vector<std::string>
        &getBlacklist( const int& web_server_id, const int& log_field_id ),
        &getWarnlist( const int& web_server_id, const int& log_field_id );
    void setBlacklist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list ),
         setWarnlist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list );
    void blacklistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item ),
         warnlistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item ),
         blacklistRemove( const int& web_server_id, const int& log_field_id, const std::string& new_item ),
         warnlistRemove( const int& web_server_id, const int& log_field_id, const std::string& item );
    int blacklistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item ),
        warnlistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item ),
        blacklistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item ),
        warnlistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item );

    // job related
    const bool checkStuff();
    void startWorking(),
         stopWorking(),
         makeCharts( const QChart::ChartTheme& theme, const std::unordered_map<std::string, QFont>& fonts, QChartView* size_chart ),
         clearDataCollection();
    const bool& isWorking(),
              & isParsing();
    // job performancea
    void collectPerfData(),
         sumWarningsSize( const int& size ),
         sumBlacklistededSize( const int& size ),
         sumPerfSize( const int& size );
    const int& getPerfSize(),
             & getTotalSize(),
             & getParsedSize(),
             & getParsedLines();
             /*& getAccessSize(),
             & getErrorSize(),
             & getBlacklistedSize( const int& log_type );*/

private:
    // quantity of informational dialogs to display
    int dialogs_level = 2; // 0: essential, 1: usefull, 2: explanatory

    // web servers ID constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    // databases paths
    std::string db_stats_path,
                db_hashes_path;

    // job related
    bool working = false,
         parsing = false,
         proceed = false;
    // perf related
    int total_lines  = 0,
        parsed_lines = 0,
        perf_size        = 0,
        total_size       = 0,
        parsed_size      = 0,
        warnlisted_size    = 0,
        blacklisted_size = 0;
    // chart related
    const QString printableSize( const int& bytes );

    // data collection, each item results from a log line
    /* structure
     *      [ { log_field_id : "data" } ]
     *
     * log_field_ids
     *      99: warning,
     *      1: year, 2: month, 3: day, 4: hour, 5: minute, 6:second,
     *      10: request_protocol, 11: request_method, 12: request_uri, 13: request_query, 14: response_code,
     *      15: time_taken, 16: bytes_sent, 17: bytes_received, 18: referrer,
     *      20: client, 21: user_agent, 22: cookie
    */
    std::vector<std::unordered_map<int, std::string>> data_collection;
    std::vector<LogFile> log_files_to_use;
    std::vector<std::string> logs_lines;
    void joinLogLines(),
         parseLogLines(),
         storeLogLines();
    // used files
    std::vector<std::string> used_files_hashes;

    // configs location
    std::string configs_path;

    // control related
    long warning_size = (1'048'576 * 50) +1; // in Bytes ( => 1 MiB * x )
    // black/warn-list
    // { web_server_id : { log_field_id : BWlist } }
    std::unordered_map<int, std::unordered_map<int, BWlist>>
        blacklists,
        warnlists;

    // web server related
    int current_WS = this->APACHE_ID;
    // web server specific paths
    std::unordered_map<int, std::string> logs_paths;
    // web server specific file names criterions
    struct LogName {
        std::string starts;
        std::string contains;
        std::string ends;
    };
    std::unordered_map<int, LogName> logs_base_names;
    // change base name contained string, only needed for IIS format modules
    void changeIisLogsBaseNames( const int& module_id );

    // logs list related
    std::vector<LogFile> logs_list;
    void scanLogsDir();

    // logs format related
    FormatOps formatOps;
    std::unordered_map<int, std::string> logs_format_strings;
    std::unordered_map<int, FormatOps::LogsFormat> logs_formats;
    void setCurrentLogFormat();
    FormatOps::LogsFormat current_LF;

    void readConfigs();

    void loadFileContent();
};

#endif // CRAPLOG_H

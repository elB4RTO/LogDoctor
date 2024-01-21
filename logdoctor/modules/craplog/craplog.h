#ifndef LOGDOCTOR__CRAPLOG_H
#define LOGDOCTOR__CRAPLOG_H


#include <QtCharts>

#include "defines/web_servers.h"

#include "modules/craplog/modules/lib.h"
#include "modules/craplog/modules/hash.h"
#include "modules/craplog/modules/formats.h"

#include "modules/craplog/modules/workers/lib.h"


//! Craplog
/*!
    Performs operations related to the logs
*/
class Craplog final : public QObject
{
    Q_OBJECT

public:
    explicit Craplog();


    /////////////////
    //// DIALOGS ////

    //! Returns the Dialogs level
    int getDialogsLevel() const noexcept;

    //! Sets the new Dialogs level
    void setDialogsLevel( const int new_level ) noexcept;

    ///////////////////
    //// DATABASES ////

    //! Returns the path of the logs Collection database
    const std::string& getStatsDatabasePath() const noexcept;

    //! Returns the path of the log files' Hashes database
    const std::string& getHashesDatabasePath() const noexcept;

    //! Sets the new path for the logs Collection database
    /*!
        \param The new path of the database file
    */
    void setStatsDatabasePath( const std::string& path ) noexcept;

    //! Sets the new path for the log files' Hashes database
    /*!
        \param The new path of the database file
    */
    void setHashesDatabasePath( const std::string& path ) noexcept;


    ////////////////////////
    //// CURRENTLY USED ////

    //! Sets the currently used Web Server ID
    /*!
        \param web_server The new currently used Web Server
    */
    void setCurrentWebServer( const WebServer web_server ) noexcept;

    //! Returns the currently used Web Server ID
    /*!
        \return The Web Server ID
    */
    WebServer getCurrentWSID() const noexcept;

    //! Uses the current Web Server to set the relative logs format
    /*!
        \see LogsFormat
    */
    void setCurrentLogFormat() noexcept;

    //! Returns the currently used LogsFormat
    /*!
        \return The LogsFormat
        \see LogsFormat
    */
    const LogsFormat& getCurrentLogFormat() const noexcept;


    ////////////////////
    //// LOGS PATHS ////

    //! Returns the logs' path for the given web Server
    /*!
        \param web_server The Web Server
        \return The path of the logs' folder
    */
    const std::string& getLogsPath( const WebServer& web_server ) const noexcept;

    //! Sets a new path for the given Web Server to search the logs in
    /*!
        \param web_server The Web Server
        \param new_path The new path
    */
    void setLogsPath( const WebServer& web_server, const std::string& new_path ) noexcept;


    ///////////////////
    //// LOG FILES ////

    //! Checks if a file name respects the relative criterions
    /*!
        \param name The name of the file
        \return Wheter it does respect the criterions or not
        \see LogName
    */
    bool isFileNameValid( const std::string& name ) const;


    ///////////////////
    //// LOGS LIST ////

    //! Returns the list of log files for the current WebServer
    /*!
        \return The list of log files
        \see LogFile, logs_list
    */
    const std::vector<LogFile>& getLogsList() const noexcept;

    //! Returns the amount of log files in the list
    /*!
        \return The number of files actually in the list
        \see logs_list
    */
    size_t getLogsListSize() const noexcept;

    //! Returns the LogFile instance of the given file
    /*!
        \param file_name The name of the file
        \return The LogFile instance
        \throw GenericException
        \see LogFile, logs_list
    */
    const LogFile& getLogFileItem( const QString& file_name ) const;

    /*const std::string& getLogFilePath( const QString& file_name );*/

    //! Sets a file in the list as selected
    /*!
        \param file_name The name of the file
        \return Wheter the given file name has been found in the list
        \see LogFile, logs_list
    */
    bool setLogFileSelected( const QString& file_name ) noexcept;

    //! Sets all files in the list as unselected
    /*!
        \return Wheter the given file name has been found in the list
        \see LogFile, logs_list
    */
    void clearLogFilesSelection() noexcept;


    //////////////////////
    //// LOGS FORMATS ////

    //! Sets the Apache2 LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processApacheFormatString()
    */
    bool setApacheLogFormat( const std::string& format_string ) noexcept;

    //! Sets the Nginx LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processNginxFormatString()
    */
    bool setNginxLogFormat( const std::string& format_string ) noexcept;

    //! Sets the IIS LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \param log_module The IIS logs module to be used to parse the format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processIisFormatString()
    */
    bool setIisLogFormat( const std::string& format_string, const int log_module ) noexcept;

    //! Returns the logs format string for the given Web Server
    /*!
        \param web_server ID of the Web Server
        \return The format string
        \see FormatOps::LogsFormat
    */
    const std::string& getLogsFormatString( const WebServer& web_server ) const noexcept;

    //! Returns the LogsFormat currently set for the given Web Server
    /*!
        \param web_server ID of the Web Server
        \return The LogsFormat instance
        \see LogsFormat
    */
    const LogsFormat& getLogsFormat( const WebServer& web_server ) const noexcept;

    //! Returns a sample log line for the given Web Server using the relative LogsFormat
    /*!
        \param web_server ID of the Web Server
        \return The sample of a log line
        \throw WebServerException
        \see FormatOps::getApacheLogSample(), FormatOps::getNginxLogSample(), FormatOps::getIisLogSample()
    */
    QString getLogsFormatSample( const WebServer& web_server ) const;

    //! Checks whether the current Logs Format is valid or not
    bool checkCurrentLogsFormat() const noexcept;



    //////////////////////
    //// WARNING SIZE ////

    //! Returns the currently set warning size for the log files
    size_t getWarningSize() const noexcept;

    //! Sets the new warning size for the log files
    void setWarningSize( const size_t new_size ) noexcept;


    ////////////////////
    //// OPERATIONS ////

    // logs usage control
    HashOps hashOps;

    //////////////////////////////
    //// BLACKLIST / WARNLIST ////

    //! Returns whether the relative blacklist is set to be used or not
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \return Whether the list is used or not
        \see BWlist
    */
    bool isBlacklistUsed( const WebServer& web_server, const int& log_field_id ) const noexcept;

    //! Returns whether the relative warnlist is set to be used or not
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \return Whether the list is used or not
        \see BWlist
    */
    bool isWarnlistUsed( const WebServer& web_server, const int& log_field_id ) const noexcept;

    //! Sets the relative blacklist to be used or not
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param used Whether the list is to be used or not
        \see BWlist
    */
    void setBlacklistUsed( const WebServer& web_server, const int& log_field_id, const bool used ) noexcept;

    //! Sets the relative warnlist to be used or not
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param used Whether the list is to be used or not
        \see BWlist
    */
    void setWarnlistUsed( const WebServer& web_server, const int& log_field_id, const bool used ) noexcept;

    //! Returns the relative items list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \return The list of items in the given blacklist
        \see BWlist
    */
    const std::vector<std::string>& getBlacklist( const WebServer& web_server, const int& log_field_id ) const noexcept;

    //! Returns the relative items list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \return The list of items in the givenwarnlist
        \see BWlist
    */
    const std::vector<std::string>& getWarnlist( const WebServer& web_server, const int& log_field_id ) const noexcept;

    //! Sets the relative items list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void setBlacklist( const WebServer& web_server, const int& log_field_id, const std::vector<std::string>& new_list );

    //! Sets the relative items list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void setWarnlist( const WebServer& web_server, const int& log_field_id, const std::vector<std::string>& new_list );

    //! Adds an item to the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void blacklistAdd( const WebServer& web_server, const int& log_field_id, const std::string& new_item );

    //! Adds an item to the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param new_item The new item to add to the list
        \see BWlist
    */
    void warnlistAdd( const WebServer& web_server, const int& log_field_id, const std::string& new_item );

    //! Removes an item from the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to remove from the list
        \see BWlist
    */
    void blacklistRemove( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;

    //! Removes an item from the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to remove from the list
        \see BWlist
    */
    void warnlistRemove( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;

    //! Moves an item one position up in the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    int blacklistMoveUp( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;

    //! Moves an item one position up in the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    int warnlistMoveUp( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;

    //! Moves an item one position down in the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    int blacklistMoveDown( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;

    //! Moves an item one position down in the relative list
    /*!
        \param web_server The Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    int warnlistMoveDown( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept;


    //////////////
    //// WORK ////

    //! Various checks to be made before starting a new process
    /*!
        Checks the databases, the selected files and their size
        \return Whether the checks has been successful or not
        \throw GenericException
    */
    bool checkStuff();

    //! Returns whether the database has been edited or not during the process
    bool isParsing() const noexcept;

    //! Returns whether the database has been edited or not during the process
    bool editedDatabase() const noexcept;


    //////////////////////
    //// PERFORMANCES ////

    //! Returns the total logs size
    size_t getParsedSize() noexcept;

    //! Returns the parsed logs lines
    size_t getParsedLines() noexcept;

    //! Returns the speed on parsing logs
    QString getParsingSpeed() noexcept;

    //! Builds and draws the chart to be displayed in the main window
    /*!
        \param theme The base theme to use for the Chart
        \param fonts The map holding the fonts
        \param size_chart The widget which will display the chart
        \see DonutBreakdown
    */
    void makeChart( const QChart::ChartTheme& theme, const std::unordered_map<std::string, QFont>& fonts, QChartView* size_chart ) const;


signals:

    void pushLogFile( const LogFile& log_file );

    void finishedRefreshing();

    void finishedWorking();


public slots:

    void scanLogsDir();

    void appendLogFile( const LogFile log_file ) noexcept;

    void logsDirScanned() noexcept;

    void startWorking();

    void workerStartedParsing() noexcept;

    void workerFinishedParsing() noexcept;

    void stopWorking( const bool successful );

    void updatePerfData( const size_t parsed_size,
                         const size_t parsed_lines ) noexcept;

    void updateChartData( const size_t total_size,
                          const size_t total_lines,
                          const size_t warnlisted_size,
                          const size_t blacklisted_size ) noexcept;

    void showWorkerDialog( const WorkerDialog dialog_type,
                           const QStringList args ) const noexcept;


private:

    /////////////////
    //// DIALOGS ////

    // quantity of information to display throught dialogs
    int dialogs_level{ 2 }; // 0: essential, 1: usefull, 2: explanatory


    ///////////////////
    //// DATABASES ////

    std::string db_stats_path;
    std::string db_hashes_path;


    //////////////
    //// WORK ////

    bool proceed    { false };
    bool db_edited  { false };
    bool is_parsing { false };

    std::mutex mutex;

    //! Hires a worker to parse the selected logs
    void hireWorker() const;


    //////////////////////
    //// PERFORMANCES ////

    size_t total_lines      { 0ul }; // total number of logs lines
    size_t parsed_lines     { 0ul }; // number of parsed logs lines
    size_t total_size       { 0ul }; // total size of the logs
    size_t parsed_size      { 0ul }; // size of the logs which have been used
    size_t warnlisted_size  { 0ul }; // size of the logs which caused a warning
    size_t blacklisted_size { 0ul }; // size of the logs which has been blacklisted

    std::chrono::system_clock::time_point parsing_time_start,
                                          parsing_time_stop;

    ////////////////////
    //// LOGS ITEMS ////

    // the selected log files to be parsed during the process
    std::vector<std::tuple<std::string,std::string>> log_files_to_use;

    // used files hashes
    std::vector<std::string> used_files_hashes;


    //////////////////////
    //// LOGS CONTROL ////

    // warning size, in Bytes
    size_t warning_size{ (1'048'576u * 50u) +1u }; // => 1 MiB * x


    //////////////////////////////
    //// BLACKLIST / WARNLIST ////

    // { web_server : { log_field_id : BWlist } }
    std::unordered_map<WebServer, std::unordered_map<int, BWlist>> blacklists;
    std::unordered_map<WebServer, std::unordered_map<int, BWlist>> warnlists;

    //! Sanitizes an item removing the unwanted elements
    /*!
        Called when adding a new item to a list
        \param log_field_id The ID of the log field
        \param new_item The item to be sanitized
        \return The sanitized item
        \throw BWlistException, GenericException
        \see BWlist
    */
    std::string sanitizeBWitem( const int& log_field_id, const std::string& new_item ) const;


    ////////////////////
    //// WEB SERVER ////

    // currently used web server
    WebServer current_web_server{ WS_APACHE };

    std::unordered_map<WebServer, std::string> logs_paths;

    //! Web Server specific file names criterions
    /*!
        The rules to be used to decide whether a file name is valid or not
        \see isFileNameValid()
    */
    struct LogName final {
        std::string starts;   //!< What should be the initial part of the name
        std::string contains; //!< What should be contained in the middle of the name
        std::string ends;     //!< What should be the final part of the name
    };

    std::unordered_map<WebServer, LogName> logs_base_names;

    //! Changes the name criterions for IIS logs files names depending on the given module
    /*!
        \param module_id The ID of the module to use to set the criterions
        \throw GenericException
        \see LogName
    */
    void changeIisLogsBaseNames( const int module_id );


    ///////////////////
    //// LOGS LIST ////

    // list of the log files found in the logs path
    std::vector<LogFile> logs_list;


    /////////////////////
    //// LOGS FORMAT ////

    FormatOps formatOps;

    std::unordered_map<WebServer, std::string> logs_format_strings;

    std::unordered_map<WebServer, LogsFormat> logs_formats;

    // currently used logs format
    LogsFormat current_log_format;

};


#endif // LOGDOCTOR__CRAPLOG_H

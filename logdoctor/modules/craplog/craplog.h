#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <QMainWindow>
#include <QtCharts>

#include "modules/craplog/modules/formats.h"
#include "modules/craplog/modules/hash.h"
#include "modules/craplog/modules/logs.h"


//! Craplog
/*!
    Performs operations related to the logs
*/
class Craplog : public QObject
{
    Q_OBJECT

public:
    explicit Craplog();


    /////////////////
    //// DIALOGS ////

    //! Returns the Dialogs level
    const int& getDialogsLevel() const;

    //! Sets the new Dialogs level
    void setDialogsLevel( const int& new_level );

    ///////////////////
    //// DATABASES ////

    //! Returns the path of the logs Collection database
    const std::string& getStatsDatabasePath() const;

    //! Returns the path of the log files' Hashes database
    const std::string& getHashesDatabasePath() const;

    //! Sets the new path for the logs Collection database
    /*!
        \param The new path of the database file
    */
    void setStatsDatabasePath( const std::string& path );

    //! Sets the new path for the log files' Hashes database
    /*!
        \param The new path of the database file
    */
    void setHashesDatabasePath( const std::string& path );


    ////////////////////////
    //// CURRENTLY USED ////

    //! Sets the currently used Web Server ID
    /*!
        \param web_server_id The new currently used Web Server
    */
    void setCurrentWSID( const int& web_server_id );

    //! Returns the currently used Web Server ID
    /*!
        \return The Web Server ID
    */
    const int& getCurrentWSID() const;

    //! Returns the currently used LogsFormat
    /*!
        \return The LogsFormat
        \see FormatOps::LogsFormat
    */
    const FormatOps::LogsFormat& getCurrentLogFormat() const;


    ////////////////////
    //// LOGS PATHS ////

    //! Returns the logs' path for the given web Server
    /*!
        \param web_server The ID of the Web Server
        \return The path of the logs' folder
    */
    const std::string& getLogsPath( const int& web_server ) const;

    //! Sets a new path for the given Web Server to search the logs in
    /*!
        \param web_server The ID of the Web Server
        \param new_path The new path
    */
    void setLogsPath(
        const int& web_server,
        const std::string& new_path
    );


    ///////////////////
    //// LOG FILES ////

    //! Structure which holds informations about a log file
    struct LogFile {
        bool selected;     //!< Wheter the file has been selected to be use or not
        bool used_already; //!< Wheter the file has been used already or not
        unsigned size;     //!< The size of the file
        QString name;      //!< The name of the file, to be displayed in the list
        std::string hash;  //!< The sha256 hash of the content
        std::string path;  //!< The path of the file, including the file name
    };

    //! Checks if a file name respects the relative criterions
    /*!
        \param name The name of the file
        \return Wheter it does respect the criterions or not
        \see LogName
    */
    const bool isFileNameValid( const std::string& name ) const;


    ///////////////////
    //// LOGS LIST ////

    //! Returns the list of log files
    /*!
        \param fresh Whether to refresh the list before to return it or not
        \return The list of log files
        \see LogFile, logs_list, scanLogsDir()
    */
    const std::vector<LogFile>& getLogsList( const bool fresh=false );

    //! Returns the amount of log files in the list
    /*!
        \return The number of files actually in the list
        \see logs_list
    */
    const int getLogsListSize() const;

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
    const bool setLogFileSelected( const QString& file_name );


    //////////////////////
    //// LOGS FORMATS ////

    //! Sets the Apache2 LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processApacheFormatString()
    */
    const bool setApacheLogFormat( const std::string& format_string );

    //! Sets the Nginx LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processNginxFormatString()
    */
    const bool setNginxLogFormat( const std::string& format_string );

    //! Sets the IIS LogsFormat from the given format string
    /*!
        \param format_string The logs format string
        \param log_module The IIS logs module to be used to parse the format string
        \return Whether the process was successful or not
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processIisFormatString()
    */
    const bool setIisLogFormat( const std::string& format_string, const int& log_module );

    //! Returns the logs format string for the given Web Server
    /*!
        \param web_server_id ID of the Web Server
        \return The format string
        \see FormatOps::LogsFormat
    */
    const std::string& getLogsFormatString( const int& web_server_id ) const;

    //! Returns the LogsFormat currently set for the given Web Server
    /*!
        \param web_server_id ID of the Web Server
        \return The LogsFormat instance
        \see FormatOps::LogsFormat
    */
    const FormatOps::LogsFormat& getLogsFormat( const int& web_server_id ) const;

    //! Returns a sample log line for the given Web Server using the relative LogsFormat
    /*!
        \param web_server_id ID of the Web Server
        \return The sample of a log line
        \throw WebServerException
        \see FormatOps::getApacheLogSample(), FormatOps::getNginxLogSample(), FormatOps::getIisLogSample()
    */
    const QString getLogsFormatSample( const int& web_server_id ) const;



    //////////////////////
    //// WARNING SIZE ////

    //! Returns the currently set warning size for the log files
    const unsigned& getWarningSize() const;

    //! Sets the new warning size for the log files
    void setWarningSize( const unsigned& new_size );


    ////////////////////
    //// OPERATIONS ////

    // logs usage control
    HashOps hashOps;

    // operations on logs
    LogOps logOps;

    //////////////////////////////
    //// BLACKLIST / WARNLIST ////

    //! Structure to hold the items of a blacklist/warnlist
    struct BWlist {
        bool used;                     //!< Whether the list is set to be used or not
        std::vector<std::string> list; //!< The list of items
    };

    //! Returns whether the relative blacklist is set to be used or not
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \return Whether the list is used or not
        \see BWlist
    */
    const bool& isBlacklistUsed( const int& web_server_id, const int& log_field_id ) const;

    //! Returns whether the relative warnlist is set to be used or not
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \return Whether the list is used or not
        \see BWlist
    */
    const bool& isWarnlistUsed( const int& web_server_id, const int& log_field_id ) const;

    //! Sets the relative blacklist to be used or not
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param used Whether the list is to be used or not
        \see BWlist
    */
    void setBlacklistUsed( const int& web_server_id, const int& log_field_id, const bool& used );

    //! Sets the relative warnlist to be used or not
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param used Whether the list is to be used or not
        \see BWlist
    */
    void setWarnlistUsed( const int& web_server_id, const int& log_field_id, const bool& used );

    //! Returns the relative items list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \return The list of items in the given blacklist
        \see BWlist
    */
    const std::vector<std::string>& getBlacklist( const int& web_server_id, const int& log_field_id ) const;

    //! Returns the relative items list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \return The list of items in the givenwarnlist
        \see BWlist
    */
    const std::vector<std::string>& getWarnlist( const int& web_server_id, const int& log_field_id ) const;

    //! Sets the relative items list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void setBlacklist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list );

    //! Sets the relative items list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void setWarnlist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list );

    //! Adds an item to the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param new_list The new items list
        \see BWlist
    */
    void blacklistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item );

    //! Adds an item to the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param new_item The new item to add to the list
        \see BWlist
    */
    void warnlistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item );

    //! Removes an item from the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to remove from the list
        \see BWlist
    */
    void blacklistRemove( const int& web_server_id, const int& log_field_id, const std::string& item );

    //! Removes an item from the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to remove from the list
        \see BWlist
    */
    void warnlistRemove( const int& web_server_id, const int& log_field_id, const std::string& item );

    //! Moves an item one position up in the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    const int blacklistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item );

    //! Moves an item one position up in the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    const int warnlistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item );

    //! Moves an item one position down in the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    const int blacklistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item );

    //! Moves an item one position down in the relative list
    /*!
        \param web_server_id The ID of the Web Server
        \param log_field_id The ID of the log field
        \param item The item to move
        \see BWlist
    */
    const int warnlistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item );


    //////////////
    //// WORK ////

    //! Various checks to be made before starting a new process
    /*!
        Checks the databases, the selected files and their size
        \return Whether the checks has been successful or not
        \throw GenericException
    */
    const bool checkStuff();

    //! Returns whether the database has been edited or not during the process
    const bool isParsing() const;

    //! Returns whether the database has been edited or not during the process
    const bool editedDatabase() const;


    //////////////////////
    //// PERFORMANCES ////

    //! Returns the total logs size
    const unsigned getParsedSize();

    //! Returns the parsed logs lines
    const unsigned getParsedLines();

    //! Returns the speed on parsing logs
    const QString getParsingSpeed();

    //! Builds and draws the chart to be displayed in the main window
    /*!
        \param theme The theme to use for the Chart
        \param fonts The map holding the fonts
        \param size_chart The widget which will display the chart
        \see DonutBreakdown
    */
    void makeChart( const QChart::ChartTheme& theme, const std::unordered_map<std::string, QFont>& fonts, QChartView* size_chart ) const;


signals:

    void finishedWorking();


public slots:

    void startWorking();

    void workerStartedParsing();

    void workerFinishedParsing();

    void stopWorking( const bool successful );

    void updatePerfData( const unsigned parsed_size,
                         const unsigned parsed_lines );

    void updateChartData( const unsigned total_size,
                          const unsigned total_lines,
                          const unsigned warnlisted_size,
                          const unsigned blacklisted_size );


private:

    /////////////////
    //// DIALOGS ////

    // quantity of information to display throught dialogs
    int dialogs_level = 2; // 0: essential, 1: usefull, 2: explanatory

    /////////////////////////
    //// WEB SERVERS IDs ////

    const unsigned APACHE_ID = 11; //!< ID of the Apache2 Web Server
    const unsigned NGINX_ID  = 12; //!< ID of the Nginx Web Server
    const unsigned IIS_ID    = 13; //!< ID of the IIS Web Server


    ///////////////////
    //// DATABASES ////

    std::string db_stats_path;
    std::string db_hashes_path;


    //////////////
    //// WORK ////

    bool proceed = false;
    bool db_edited = false;
    bool is_parsing = false;

    std::mutex mutex;


    //////////////////////
    //// PERFORMANCES ////

    unsigned total_lines  = 0; // total number of logs lines
    unsigned parsed_lines = 0; // number of parsed logs lines
    unsigned total_size       = 0; // total size of the logs
    unsigned parsed_size      = 0; // size of the logs which have been used
    unsigned warnlisted_size  = 0; // size of the logs which caused a warning
    unsigned blacklisted_size = 0; // size of the logs which has been blacklisted

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
    unsigned warning_size = (1'048'576 * 50) +1; // => 1 MiB * x


    //////////////////////////////
    //// BLACKLIST / WARNLIST ////

    // { web_server_id : { log_field_id : BWlist } }
    std::unordered_map<int, std::unordered_map<int, BWlist>> blacklists;
    std::unordered_map<int, std::unordered_map<int, BWlist>> warnlists;

    //! Ssnitizes an item removing the unwanted elements
    /*!
        Called when adding a new item to a list
        \param log_field_id The ID of the log field
        \param new_item The item to be sanitized
        \return The sanitized item
        \throw BWlistException, GenericException
        \see BWlist
    */
    const std::string sanitizeBWitem( const int& log_field_id, const std::string& new_item ) const;


    ////////////////////
    //// WEB SERVER ////

    // currently used web server
    int current_WS = this->APACHE_ID;

    std::unordered_map<int, std::string> logs_paths;

    //! Web Server specific file names criterions
    /*!
        The rules to be used to decide whether a file name is valid or not
        \see isFileNameValid()
    */
    struct LogName {
        std::string starts;   //!< What should be initial part of the name
        std::string contains; //!< What should be contained in the middle of the name
        std::string ends;     //!< What should be final part of the name
    };

    std::unordered_map<int, LogName> logs_base_names;

    //! Changes the name criterions for IIS logs files names depending on the given module
    /*!
        \param module_id The ID of the module to use to set the criterions
        \throw GenericException
        \see LogName
    */
    void changeIisLogsBaseNames( const int& module_id );


    ///////////////////
    //// LOGS LIST ////

    // list of the log files found in the logs path
    std::vector<LogFile> logs_list;

    //! Scans the logs directory to get a list of log files
    void scanLogsDir();


    /////////////////////
    //// LOGS FORMAT ////

    FormatOps formatOps;

    std::unordered_map<int, std::string> logs_format_strings;

    std::unordered_map<int, FormatOps::LogsFormat> logs_formats;

    //! Uses the current Web Server to set the relative logs format
    /*!
        \see LogOps::LogsFormat
    */
    void setCurrentLogFormat();

    // currently used logs format
    FormatOps::LogsFormat current_LF;

};

#endif // CRAPLOG_H

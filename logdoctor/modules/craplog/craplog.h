#ifndef LOGDOCTOR__CRAPLOG_H
#define LOGDOCTOR__CRAPLOG_H


#include "lib.h"

#include "modules/craplog/modules/lib.h"
#include "modules/craplog/modules/hasher.h"

#include "modules/craplog/workers/lib.h"

#include <QtCharts>


struct LogFile;
struct Blacklists;

class QWaitCondition;


//! Craplog
/*!
    Performs operations related to the logs
*/
class Craplog final : public QObject
{
    Q_OBJECT

public:

    explicit Craplog();
    Q_DISABLE_COPY_MOVE(Craplog)


    /////////////////
    //// DIALOGS ////

    //! Returns the Dialogs level
    DialogsLevel getDialogsLevel() const noexcept;

    //! Sets the new Dialogs level
    void setDialogsLevel( const DialogsLevel new_level ) noexcept;

    ///////////////////
    //// DATABASES ////

    //! Returns the path of the logs Collection database
    const PathHandler& getStatsDatabasePath() const noexcept;

    //! Returns the path of the log files' Hashes database
    const PathHandler& getHashesDatabasePath() const noexcept;

    //! Sets the new path for the logs Collection database
    /*!
        \param The new path of the database file
    */
    void setStatsDatabasePath( const PathHandler& path ) noexcept;

    //! Sets the new path for the log files' Hashes database
    /*!
        \param The new path of the database file
    */
    void setHashesDatabasePath( const PathHandler& path ) noexcept;


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
    WebServer getCurrentWebServer() const noexcept;

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
    const PathHandler& getLogsPath( const WebServer& web_server ) const noexcept;

    //! Sets a new path for the given Web Server to search the logs in
    /*!
        \param web_server The Web Server
        \param new_path The new path
    */
    void setLogsPath( const WebServer& web_server, const PathHandler& new_path ) noexcept;


    ///////////////////
    //// LOG FILES ////

    //! Checks if a file name respects the relative criterions
    /*!
        \param name The name of the file
        \return Wheter it does respect the criterions or not
        \throw DoNotCatchException
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
    std::size_t getLogsListSize() const noexcept;

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
        \throw DoNotCatchException
        \see FormatOps, FormatOps::LogsFormat, FormatOps::processIisFormatString()
    */
    bool setIisLogFormat( const std::string& format_string, const IISLogsModule log_module ) noexcept;

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
        \throw DoNotCatchException
        \see FormatOps::getApacheLogSample(), FormatOps::getNginxLogSample(), FormatOps::getIisLogSample()
    */
    QString getLogsFormatSample( const WebServer& web_server ) const;

    //! Checks whether the current Logs Format is valid or not
    bool checkCurrentLogsFormat() const noexcept;



    //////////////////////
    //// WARNING SIZE ////

    //! Returns the currently set warning size for the log files
    std::size_t getWarningSize() const noexcept;

    //! Sets the new warning size for the log files
    void setWarningSize( const std::size_t new_size ) noexcept;


    ////////////////////
    //// OPERATIONS ////

    // logs usage control
    Hasher hasher;


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
    std::size_t getParsedSize() noexcept;

    //! Returns the parsed logs lines
    std::size_t getParsedLines() noexcept;

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

    void doneStoringFilesHashes( const bool successful );

    void finishedRefreshing();

    void finishedWorking();


public slots:

    void scanLogsDir();

    void appendLogFile( const LogFile log_file ) noexcept;

    void logsDirScanned() noexcept;

    void startWorking( const Blacklists& blacklists );

    void workerStartedParsing() noexcept;

    void workerFinishedParsing() noexcept;

    void stopWorking( const bool successful );

    void updatePerfData( const std::size_t parsed_size,
                         const std::size_t parsed_lines ) noexcept;

    void updateChartData( const std::size_t total_size,
                          const std::size_t total_lines,
                          const std::size_t blacklisted_size ) noexcept;

    void showWorkerDialog( const WorkerDialog dialog_type,
                           const QStringList args ) const noexcept;

    void storeFilesHashes( QWaitCondition* wc, bool* successful ) noexcept;


private:

    /////////////////
    //// DIALOGS ////

    // quantity of information to display throught dialogs
    DialogsLevel dialogs_level{ DL_NORMAL };


    ///////////////////
    //// DATABASES ////

    PathHandler db_stats_path;
    PathHandler db_hashes_path;


    //////////////
    //// WORK ////

    bool proceed    { false };
    bool db_edited  { false };
    bool is_parsing { false };

    std::mutex mutex;

    //! Hires a worker to parse the selected logs
    void hireWorker( const Blacklists& blacklists ) const;


    //////////////////////
    //// PERFORMANCES ////

    std::size_t total_lines      { 0ul }; // total number of logs lines
    std::size_t parsed_lines     { 0ul }; // number of parsed logs lines
    std::size_t total_size       { 0ul }; // total size of the logs
    std::size_t parsed_size      { 0ul }; // size of the logs which have been used
    std::size_t blacklisted_size { 0ul }; // size of the logs which has been blacklisted

    std::chrono::system_clock::time_point parsing_time_start,
                                          parsing_time_stop;

    ////////////////////
    //// LOGS ITEMS ////

    // the selected log files to be parsed during the process
    std::vector<std::tuple<PathHandler,std::string>> log_files_to_use;

    // used files hashes
    std::vector<std::string> used_files_hashes;


    //////////////////////
    //// LOGS CONTROL ////

    // warning size, in Bytes
    std::size_t warning_size{ (1'048'576u * 50u) +1u }; // => 1 MiB * x


    ////////////////////
    //// WEB SERVER ////

    // currently used web server
    WebServer current_web_server{ WS_APACHE };

    std::unordered_map<WebServer, PathHandler> logs_paths;

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
        \param log_module The ID of the module to use to set the criterions
        \throw DoNotCatchException
        \see LogName
    */
    void changeIisLogsBaseNames( const IISLogsModule log_module );


    ///////////////////
    //// LOGS LIST ////

    // list of the log files found in the logs path
    std::vector<LogFile> logs_list;


    /////////////////////
    //// LOGS FORMAT ////

    std::unordered_map<WebServer, std::string> logs_format_strings;

    std::unordered_map<WebServer, LogsFormat> logs_formats;

    // currently used logs format
    LogsFormat current_log_format;

};


#endif // LOGDOCTOR__CRAPLOG_H

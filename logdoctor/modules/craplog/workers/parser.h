#ifndef LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H
#define LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H


#include "main_lib.h"

#include "modules/security/path.h"

#include "modules/craplog/modules/lib.h"

#include "modules/blacklists/modules/blacklist.h"

#include <QObject>

#include <unordered_map>


struct LogLineData;

class DatabaseWrapper;

enum class WorkerDialog;

class QWaitCondition;

class QSqlDatabase;


class CraplogParser final : public QObject
{
    Q_OBJECT

    using worker_files_t  = std::vector<PathHandler>;

public:

    explicit CraplogParser(
        const WebServer web_server,
        const DialogsLevel dialogs_level,
        const LogsFormat& logs_format,
        const Blacklist& blacklist,
        worker_files_t&& log_files,
        const PathHandler& data_db_path,
        QObject* parent=nullptr
    );

    ~CraplogParser();

signals:

    void perfData(
        const size_t parsed_size,
        const size_t parsed_lines );

    void chartData(
        const size_t total_size,
        const size_t total_lines,
        const size_t blacklisted_size );

    void showDialog(
        const WorkerDialog dialog_type,
        const QStringList arg );

    void readyStoringData( QWaitCondition* wc, bool* successful );

    void startedParsing();

    void finishedParsing();

    void done( const bool successful );

    void retire();

public slots:

    void work();

    void sendPerfData() noexcept;

    void sendChartData() noexcept;

private:

    const WebServer web_server;

    const DialogsLevel dialogs_level;

    bool proceed{ true };

    ///////////////////
    //// DATABASES ////

    bool db_edited{ false };

    PathHandler db_path;

    const QString db_name;
    const QString db_conn_name{ QStringLiteral("Parser_LogsData") };

    bool checkDatabaseFile() noexcept;

    //////////////////////
    //// PERFORMANCES ////

    size_t total_lines      { 0ul };
    size_t parsed_lines     { 0ul };
    size_t total_size       { 0ul };
    size_t parsed_size      { 0ul };
    size_t blacklisted_size { 0ul };

    ///////////////////
    //// BLACKLIST ////

    const Blacklist blacklist;

    //////////////
    //// LOGS ////

    LogsFormat logs_format;

    // the selected log files to be parsed during the process
    const worker_files_t files_to_use;

    // the entire stack of lines which have been read from the log files
    std::vector<std::string> logs_lines;

    // collection of logs data, each item represents a log line
    std::vector<LogLineData> data_collection;

    //! Reads the selected files and append the resulting lines to the list
    /*!
        \throw GenericException
    */
    void joinLogLines();

    //! Parses the lines in the list and stores their data in the data collection
    /*!
        \throw LogParserException
    */
    void parseLogLines();

    //! Handles the process of storing data in the database
    /*!
        \param db The database instance, not initialized already
        \see storeData()
    */
    void storeLogLines( QSqlDatabase& db );

    //! Stores the data collection in the logs Collection database
    /*!
        \param db The database instance, already initialized
        \return Whether the operation has been successful or not
        \throw WebServerException
    */
    bool storeData( QSqlDatabase& db );
};


#endif // LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H

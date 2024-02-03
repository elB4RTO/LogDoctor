#ifndef LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H
#define LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H


#include "main_lib.h"

#include "modules/craplog/modules/lib.h"

#include <QObject>

#include <unordered_map>


struct BWlist;
struct LogLineData;

class DatabaseWrapper;

enum class WorkerDialog;


class CraplogParser final : public QObject
{
    Q_OBJECT

    using logs_file_t     = std::tuple<std::string,std::string>;
    using worker_files_t  = std::vector<logs_file_t>;
    using bw_lists_t      = std::unordered_map<int, BWlist>;

public:

    explicit CraplogParser(
        const WebServer web_server,
        const DialogsLevel dialogs_level,
        const std::string& db_data_path,
        const std::string& db_hashes_path,
        const LogsFormat& logs_format,
        const bw_lists_t& blacklists,
        const worker_files_t& log_files,
        QObject* parent=nullptr
    );

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
    std::string db_data_path;
    std::string db_hashes_path;

    bool checkDatabaseFile( const QString& db_name ) noexcept;

    //////////////////////
    //// PERFORMANCES ////

    size_t total_lines      { 0ul };
    size_t parsed_lines     { 0ul };
    size_t total_size       { 0ul };
    size_t parsed_size      { 0ul };
    size_t blacklisted_size { 0ul };

    ///////////////////
    //// BLACKLIST ////

    // { log_field_id : BWlist }
    const bw_lists_t blacklists;

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
        \see storeData()
    */
    void storeLogLines();

    //! Stores the data collection in the logs Collection database
    /*!
        \param db A database instance, already initizlized
        \param db_name The database in use, to be shown in the dialogs
        \return Whether the operation has been successful or not
        \throw WebServerException
    */
    bool storeData( DatabaseWrapper& db , const QString& db_name );

};


#endif // LOGDOCTOR__CRAPLOG__WORKERS__PARSER_H

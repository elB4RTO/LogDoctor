#ifndef CraplogParserAsync_H
#define CraplogParserAsync_H

#include <QObject>

#include "modules/craplog/modules/lib.h"

#include <atomic>
#include <future>
#include <unordered_map> // leave this here for clang

enum class WorkerDialog;

class QSqlDatabase;

typedef std::unordered_map<int, BWlist> bw_lists_t; // leave this here for clang


class CraplogParserAsync : public QObject
{
    Q_OBJECT

private:

    using logs_file_t     = std::tuple<std::string,std::string>;
    using worker_files_t  = std::vector<logs_file_t>;
    using logs_lines_t    = std::vector<std::string>;
    using log_line_data_t = std::map<int, std::string>;


public:

    explicit CraplogParserAsync(
        const unsigned web_server_id,
        const unsigned dialogs_level,
        const std::string& db_data_path,
        const std::string& db_hashes_path,
        const LogsFormat& logs_format,
        const bw_lists_t& blacklists,
        const bw_lists_t& warnlists,
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
        const size_t warnlisted_size,
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

    void sendPerfData();

    void sendChartData();

private:

    const unsigned wsID;

    const unsigned dialogs_level{ 2 };


    //////////////////////////////
    //// BLACKLIST / WARNLIST ////

    // { log_field_id : BWlist }
    bw_lists_t blacklists;
    bw_lists_t warnlists;

    LogsFormat logs_format;

    std::atomic<bool> proceed{ true };

    bool db_edited{ false };
    std::string db_data_path;
    std::string db_hashes_path;

    // the selected log files to be parsed during the process
    worker_files_t files_to_use;

    // collection of logs items, each item results from a log line
    /* structure
            [ { log_field_id : "data" } ]

       log_field_ids
            99: warning,
            1: year, 2: month, 3: day, 4: hour, 5: minute, 6:second,
            10: request_protocol, 11: request_method, 12: request_uri, 13: request_query, 14: response_code,
            15: time_taken, 16: bytes_sent, 17: bytes_received, 18: referrer,
            20: client, 21: user_agent, 22: cookie
    */
    std::vector<log_line_data_t> data_collection;

    //! Reads the selected files and append the resulting lines to the list
    /*!
        \throw GenericException
    */
    void joinLogLines( std::promise<logs_lines_t>& log_lines, const logs_file_t& logs_file );

    //! Parses the lines in the list and stores their data in the data collection
    /*!
        \throw LogParserException
    */
    void parseLogLines( std::future<logs_lines_t>& log_lines );

    //! Handles the process of storing data in the database
    /*!
        \see storeData()
    */
    void storeLogLines();

    //! Stores the data collection in the logs Collection database
    /*!
        \param db A database instance, already initizlized
        \return Whether the operation has been successful or not
        \throw WebServerException
    */
    const bool storeData( QSqlDatabase& db );


    //////////////////////
    //// PERFORMANCES ////

    size_t total_lines      { 0 };
    size_t parsed_lines     { 0 };
    size_t total_size       { 0 };
    size_t parsed_size      { 0 };
    size_t warnlisted_size  { 0 };
    size_t blacklisted_size { 0 };


    // Map to convert log fields to field IDs
    const std::unordered_map<std::string, int> field2id{
        // date-time
        {"date_time_year",     1},
        {"date_time_month",    2},
        {"date_time_day",      3},
        {"date_time_hour",     4},
        {"date_time_minute",   5},
        {"date_time_second",   6},
            {"date_time_ncsa",        0},
            {"date_time_iso",         0},
            {"date_time_mcs",         0},
            {"date_time_gmt",         0},
            {"date_time_utc_d",       0},
            {"date_time_utc_t",       0},
            {"date_time_epoch_s",     0},
            {"date_time_epoch_s.ms",  0},
            {"date_time_epoch_ms",    0},
            {"date_time_epoch_us",    0},
            {"date_time_YYYYMMDD",    0},
            {"date_time_MMDDYY",      0},
            {"date_time_MDYYYY",      0},
            {"date_time_year_short",  0},
            {"date_time_month_str",   0},
            {"date_time_clock_12",    0},
            {"date_time_clock_24",    0},
            {"date_time_clock_short", 0},
        // request
        {"request_protocol",   10},
        {"request_method",     11},
        {"request_uri",        12},
        {"request_query",      13},
        {"response_code",      14},
            {"request_full",       0},
        // performance
        {"time_taken_ms",      15},
            {"time_taken_us",      0},
            {"time_taken_s.ms",    0},
            {"time_taken_s",       0},
        {"bytes_sent",         16},
        {"bytes_received",     17},
        // referer
        {"referer",            18},
        // client data
        {"client",             20},
        {"user_agent",         21},
        {"cookie",             22}
    };

};


#endif // CraplogParserAsync_H

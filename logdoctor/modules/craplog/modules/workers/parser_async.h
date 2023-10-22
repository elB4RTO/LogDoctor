#ifndef LOGDOCTOR__CRAPLOG__WORKERS__PARSER_ASYNC_H
#define LOGDOCTOR__CRAPLOG__WORKERS__PARSER_ASYNC_H


#include "parser_interface.h"

#include <atomic>
#include <future>


class CraplogParserAsync : public CraplogParserInterface
{
    using logs_lines_t = std::vector<std::string>;

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

public slots:

    void work() override;

private:

    std::atomic<bool> proceed{ true };

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
    bool storeData( QSqlDatabase& db );

};


#endif // LOGDOCTOR__CRAPLOG__WORKERS__PARSER_ASYNC_H

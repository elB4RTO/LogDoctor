
#include "parser_interface.h"

CraplogParserInterface::CraplogParserInterface( const unsigned web_server_id, const unsigned dialogs_level, const std::string& db_data_path, const std::string& db_hashes_path, const LogsFormat& logs_format, const bw_lists_t& blacklists, const bw_lists_t& warnlists, const worker_files_t& log_files, QObject* parent )
    : QObject        { parent         }
    , wsID           { web_server_id  }
    , dialogs_level  { dialogs_level  }
    , db_data_path   { db_data_path   }
    , db_hashes_path { db_hashes_path }
    , logs_format    { logs_format    }
    , blacklists     { blacklists     }
    , warnlists      { warnlists      }
    , files_to_use   { log_files      }
{

}


void CraplogParserInterface::sendPerfData()
{
    emit this->perfData(
        this->parsed_size,
        this->parsed_lines
    );
}

void CraplogParserInterface::sendChartData()
{
    emit this->chartData(
        this->total_size,
        this->total_lines,
        this->warnlisted_size,
        this->blacklisted_size
    );
}

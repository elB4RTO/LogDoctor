
#include "parser.h"

#include "defines/web_servers.h"

#include "utilities/checks.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/craplog/modules/workers/lib.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


CraplogParser::CraplogParser( const unsigned web_server_id, const unsigned dialogs_level, const std::string& db_data_path, const std::string& db_hashes_path, const LogsFormat& logs_format, const bw_lists_t& blacklists, const bw_lists_t& warnlists, const worker_files_t& log_files, QObject* parent )
    : QObject        { parent         }
    , wsID           { web_server_id  }
    , dialogs_level  { dialogs_level  }
    , db_data_path   { db_data_path   }
    , db_hashes_path { db_hashes_path }
    , blacklists     { blacklists     }
    , warnlists      { warnlists      }
    , logs_format    { logs_format    }
    , files_to_use   { log_files      }
{

}


void CraplogParser::sendPerfData()
{
    emit this->perfData(
        this->parsed_size,
        this->parsed_lines
    );
}

void CraplogParser::sendChartData()
{
    emit this->chartData(
        this->total_size,
        this->total_lines,
        this->warnlisted_size,
        this->blacklisted_size
    );
}


void CraplogParser::work()
{
    this->proceed |= true;
    this->db_edited &= false;
    try {
        if ( this->proceed ) [[likely]] {
            // collect log lines
            this->joinLogLines();
        }
        if ( this->proceed ) [[likely]]  {
            // parse the log lines to fill the collection
            emit this->startedParsing();
            this->parseLogLines();
            emit this->finishedParsing();
        }
        // clear log lines data
        this->logs_lines.clear();

        if ( this->proceed && !this->data_collection.empty() ) [[likely]]  {
            // store the new data
            this->storeLogLines();
            this->db_edited |= this->proceed;
        }

    } catch ( GenericException& e ) {
        emit this->showDialog( WorkerDialog::errGeneric,
                               {e.what()} );
        this->proceed &= false;

    } catch ( LogParserException& e ) {
        emit this->showDialog( WorkerDialog::errFailedParsingLogs,
                               {e.what()} );
        this->proceed &= false;
    }
    // send the final data
    if ( ! this->proceed ) {
        this->total_lines  = 0ul;
        this->parsed_lines = 0ul;
        this->total_size   = 0ul;
        this->parsed_size  = 0ul;
        this->warnlisted_size  = 0ul;
        this->blacklisted_size = 0ul;
    }
    this->sendPerfData();
    this->sendChartData();
    emit this->done( this->db_edited );
    emit this->retire();
}


void CraplogParser::joinLogLines()
{
    const auto cleanLines = [](std::vector<std::string>& lines) {
        std::vector<std::string> aux;
        aux.reserve( lines.size() );
        for ( const std::string& line : lines ) {
            if ( line.front() != '#' ) [[likely]] {
                // not a commented line
                aux.push_back( line );
            }
        }
        aux.shrink_to_fit();
        lines = std::move( aux );
    };


    std::string aux;
    std::vector<std::string> content;
    for ( const auto& file : this->files_to_use ) {

        if ( ! this->proceed ) { break; }

        const std::string& file_path = std::get<0>( file );

        // collect lines
        try {
            // try reading
            content.clear();
            aux.clear();
            try {
                // try as gzip compressed archive first
                GZutils::readFile( file_path, aux );

            } catch ( const GenericException& ) {
                // failed closing file pointer
                throw;

            } catch (...) {
                // fallback on reading as normal file
                if ( ! aux.empty() ) {
                    aux.clear();
                }
                IOutils::readFile( file_path, aux );
            }
            StringOps::splitrip( content, aux );

            this->total_lines += content.size();
            this->total_size  += aux.size();

            if ( this->wsID == IIS_ID ) {
                cleanLines( content );
            }

        // re-catched in run()
        } catch ( const GenericException& ) {
            // failed closing gzip file pointer
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("An error occured while reading the gzipped file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch ( const std::ios_base::failure& ) {
            // failed reading as text
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("An error occured while reading the file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch (...) {
            // failed somehow
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("Something failed while handling the file"),
                    QString::fromStdString( file_path )
                ).toStdString() );
        }

        // append to the list
        this->logs_lines.insert( this->logs_lines.end(), content.begin(), content.end() );
    }
    if ( this->logs_lines.empty() ) {
        this->proceed &= false;
    }
}


void CraplogParser::parseLogLines()
{
    const auto parseLine = [this]( const std::string& line, const LogsFormat& logs_format ) {
        this->data_collection.emplace_back( LogLineData(line, logs_format) );
        this->parsed_size += line.size();
        this->parsed_lines ++;
    };


    // parse all the lines
    if ( this->proceed ) {
        const size_t n_lines{ this->logs_lines.size() };
        const size_t nl{ this->logs_format.new_lines };
        size_t send{ 0ul };
        if ( nl == 0ul ) {
            const size_t send_gap{ n_lines>1000ul ? n_lines/100 : n_lines>100ul ? n_lines/10 : 10 };
            const LogsFormat& lf {this->logs_format};
            this->data_collection.reserve( n_lines );
            for ( const std::string& line : this->logs_lines ) {
                parseLine( line, lf );
                if (send == send_gap) {
                    this->sendPerfData();
                    send = 0ul;
                }
                ++send;
            }
        } else {
            const size_t real_lines{ n_lines / (nl+1ul) };
            const size_t send_gap{ real_lines>1000ul ? real_lines/100 : real_lines>100ul ? real_lines/10 : 10 };
            const LogsFormat& lf {this->logs_format};
            this->data_collection.reserve( real_lines );
            for ( size_t i{0ul}; i<n_lines; i++ ) {
                std::string line = this->logs_lines.at( i );
                for ( size_t n{0ul}; n<nl; n++ ) {
                    i++;
                    line += "\n" + this->logs_lines.at( i );
                }
                parseLine( line, lf );
                if (send == send_gap) {
                    this->sendPerfData();
                    send = 0ul;
                }
                ++send;
            }
        }
        this->sendPerfData();
    }
}



void CraplogParser::storeLogLines()
{
    QString db_path{ QString::fromStdString( this->db_data_path ) };
    QString db_name{ QString::fromStdString( this->db_data_path.substr( this->db_data_path.find_last_of( '/' ) + 1ul ) ) };

    QSqlDatabase db{ QSqlDatabase::addDatabase("QSQLITE") };
    db.setDatabaseName( db_path );

    if ( ! CheckSec::checkDatabaseFile( this->db_data_path, db_name ) ) {
        this->proceed &= false;

    } else if ( ! db.open() ) {
        // error opening database
        this->proceed &= false;
        QString err_msg;
        if ( this->dialogs_level == 2 ) {
            err_msg = db.lastError().text();
        }
        emit this->showDialog( WorkerDialog::errDatabaseFailedOpening,
                               {db_name, err_msg} );

    } else {

        try {
            // ACID transaction
            if ( ! db.transaction() ) {
                // error opening database
                this->proceed &= false;
                QString stmt_msg, err_msg;
                if ( this->dialogs_level > 0 ) {
                    stmt_msg = "db.transaction()";
                    if ( this->dialogs_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                       {db_name, stmt_msg, err_msg} );
            }

            if ( this->proceed ) {
                this->proceed &= this->storeData( db );
            }

            if ( this->proceed ) {
                // commit the transaction
                if ( ! db.commit() ) {
                    // error opening database
                    this->proceed &= false;
                    QString stmt_msg, err_msg;
                    if ( this->dialogs_level > 0 ) {
                        stmt_msg = "db.commit()";
                        if ( this->dialogs_level == 2 ) {
                            err_msg= db.lastError().text();
                        }
                    }
                    emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                           {db_name, stmt_msg, err_msg} );
                }
            }
            if ( ! this->proceed ) {
                // rollback
                throw (std::exception());
            }

        } catch (...) {
            // wrongthing w3nt some.,.
            this->proceed &= false;
            bool err_shown = false;
            // rollback the transaction
            if ( ! db.rollback() ) {
                // error rolling back commits
                QString stmt_msg, err_msg;
                if ( this->dialogs_level > 0 ) {
                    stmt_msg = "db.rollback()";
                    if ( this->dialogs_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                       {db_name, stmt_msg, err_msg} );
                err_shown = true;
            }
            if ( ! err_shown ) {
                // show a message
                emit this->showDialog(
                    WorkerDialog::errGeneric,
                    {QString("%1\n\n%2").arg(
                        DialogSec::tr("An error occured while working on the database"),
                        DialogSec::tr("Aborting") )} );
            }
        }

        if ( db.isOpen() ) {
            db.close();
        }
    }
}

#define APPEND_TO_QUERY_AS_NUMBER(LOG_FIELD)\
    query_stmt += QStringLiteral(", ");\
    if ( LOG_FIELD ) {\
        query_stmt += QString::fromStdString( *LOG_FIELD ).replace("'","''");\
    } else {\
        query_stmt += QStringLiteral("NULL");\
    }

#define APPEND_TO_QUERY_AS_STRING(LOG_FIELD)\
    query_stmt += QStringLiteral(", ");\
    if ( LOG_FIELD ) {\
        query_stmt += QString("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace("'","''") );\
    } else {\
            query_stmt += QStringLiteral("NULL");\
    }

// in IIS logs the user-agent is logged with '+' instead of ' ' (whitespace)
#define APPEND_TO_QUERY_USER_AGENT(LOG_FIELD)\
    query_stmt += QStringLiteral(", ");\
    if ( LOG_FIELD ) {\
        if ( this->wsID == IIS_ID ) {\
            query_stmt += QString("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace("+"," ").replace("'","''") );\
        } else {\
            query_stmt += QString("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace("'","''") );\
        }\
    } else {\
            query_stmt += QStringLiteral("NULL");\
    }

bool CraplogParser::storeData( QSqlDatabase& db )
{
    const QString db_name{ QString::fromStdString(
        this->db_data_path.substr(
            this->db_data_path.find_last_of( '/' ) + 1ul ) ) };

    // get blacklist/warnlist items
    const bool check_bl_cli { this->blacklists.at( 20 ).used };
    const bool check_wl_met { this->warnlists.at( 11 ).used  };
    const bool check_wl_uri { this->warnlists.at( 12 ).used  };
    const bool check_wl_cli { this->warnlists.at( 20 ).used  };
    const bool check_wl_ua  { this->warnlists.at( 21 ).used  };

    const std::vector<std::string> empty;
    const std::vector<std::string>& bl_cli_list{ (check_bl_cli)
        ? this->blacklists.at( 20 ).list
        : empty };

    const std::vector<std::string>& wl_met_list{ (check_wl_met)
        ? this->warnlists.at( 11 ).list
        : empty };

    const std::vector<std::string>& wl_req_list{ (check_wl_uri)
        ? this->warnlists.at( 12 ).list
        : empty };

    const std::vector<std::string>& wl_cli_list{ (check_wl_cli)
        ? this->warnlists.at( 20 ).list
        : empty };

    const std::vector<std::string>& wl_ua_list{ (check_wl_ua)
        ? this->warnlists.at( 21 ).list
        : empty };

    // prepare the database related studd
    QString table;
    switch ( this->wsID ) {
        case APACHE_ID:
            table += "apache";
            break;
        case NGINX_ID:
            table += "nginx";
            break;
        case IIS_ID:
            table += "iis";
            break;
        default:
            // wrong WebServerID, but should be unreachable because of the previous operations
            throw WebServerException( "Unexpected WebServerID: " + std::to_string(this->wsID) );
    }


    /*int perf_size;*/
    bool warning{ false };
    QSqlQuery query{ db };
    // parse every row of data
    for ( const LogLineData& line_data : this->data_collection ) {

        // check blacklisted clients
        if ( check_bl_cli && line_data.client ) {
            if ( VecOps::contains( bl_cli_list, *line_data.client ) ) {
                this->blacklisted_size += line_data.size();
                continue;
            }
        }

        // check warnlisted clients
        if ( check_wl_cli && line_data.client ) {
            if ( VecOps::contains( wl_cli_list, *line_data.client ) ) {
                warning |= true;
                goto end_of_warnings_check;
            }
        }
        // check warnlisted user-agents
        if ( check_wl_ua && line_data.user_agent ) {
            if ( VecOps::contains( wl_ua_list, *line_data.user_agent ) ) {
                // match found! skip this line
                warning |= true;
                goto end_of_warnings_check;
            }
        }
        // check warnlisted methods
        if ( check_wl_met && line_data.method ) {
            if ( VecOps::contains( wl_met_list, *line_data.method ) ) {
                // match found! skip this line
                warning |= true;
                goto end_of_warnings_check;
            }
        }
        // check warnlisted requests URIs
        if ( check_wl_uri && line_data.uri ) {
            if ( VecOps::contains( wl_req_list, *line_data.uri ) ) {
                // match found! skip this line
                warning |= true;
            }
        }
        end_of_warnings_check:


        // initialize the SQL statement
        QString query_stmt{ "INSERT INTO \""+table+"\" (\"warning\", \"year\", \"month\", \"day\", \"hour\", \"minute\", \"second\", \"protocol\", \"method\", \"uri\", \"query\", \"response\", \"time_taken\", \"bytes_sent\", \"bytes_received\", \"referrer\", \"client\", \"user_agent\", \"cookie\") "
                            "VALUES (" };

        // complete and execute the statement, binding NULL if not found

        // warning
        if ( warning ) {
            warning &= false;
            this->warnlisted_size += line_data.size();
            query_stmt += "1";
        } else {
            query_stmt += "0";
        }

        // date and time
        APPEND_TO_QUERY_AS_NUMBER(line_data.year) // 1
        APPEND_TO_QUERY_AS_NUMBER(line_data.month) // 2
        APPEND_TO_QUERY_AS_NUMBER(line_data.day) // 3
        APPEND_TO_QUERY_AS_NUMBER(line_data.hour) // 4
        APPEND_TO_QUERY_AS_NUMBER(line_data.minute) // 5
        APPEND_TO_QUERY_AS_NUMBER(line_data.second) // 6

        // request
        APPEND_TO_QUERY_AS_STRING(line_data.protocol) // 10
        APPEND_TO_QUERY_AS_STRING(line_data.method) // 11
        APPEND_TO_QUERY_AS_STRING(line_data.uri) // 12
        APPEND_TO_QUERY_AS_STRING(line_data.query) // 13

        APPEND_TO_QUERY_AS_NUMBER(line_data.response_code) // 14
        APPEND_TO_QUERY_AS_NUMBER(line_data.time_taken) // 15
        APPEND_TO_QUERY_AS_NUMBER(line_data.bytes_sent) // 16
        APPEND_TO_QUERY_AS_NUMBER(line_data.bytes_received) // 17

        // client data and referrer
        APPEND_TO_QUERY_AS_STRING(line_data.referrer) // 18
        APPEND_TO_QUERY_AS_STRING(line_data.client) // 20
        APPEND_TO_QUERY_USER_AGENT(line_data.user_agent) // 21
        APPEND_TO_QUERY_AS_STRING(line_data.cookie) // 22

        query_stmt += ");";

        // encode the statement
        if ( ! query.prepare( query_stmt ) ) {
            // error opening database
            QString query_msg, err_msg;
            if ( this->dialogs_level > 0 ) {
                query_msg = "query.prepare()";
                if ( this->dialogs_level == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                   {db_name, query_msg, err_msg} );
            return false;
        }

        // finalize this statement
        if ( ! query.exec() ) {
            // error finalizing step
            QString query_msg, err_msg;
            if ( this->dialogs_level > 0 ) {
                query_msg = "query.exec()";
                if ( this->dialogs_level == 2 ) {
                    err_msg = query.lastError().text();
                }
            }
            emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                   {db_name, query_msg, err_msg} );
            return false;
        }

        // reset the statement to prepare for the next one
        query.finish();
    }

    return true;
}


#include "parser.h"

#include "globals/db_names.h"

#include "utilities/checks.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/craplog/modules/workers/lib.h"

#include <QWaitCondition>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>


CraplogParser::CraplogParser( const WebServer web_server, const DialogsLevel dialogs_level, const LogsFormat& logs_format, const Blacklist& blacklist, worker_files_t&& log_files, const std::string& data_db_path, QObject* parent )
    : QObject        { parent                     }
    , web_server     { web_server                 }
    , dialogs_level  { dialogs_level              }
    , db_path        { data_db_path               }
    , db_name        { DatabasesConnections::data }
    , blacklist      { blacklist                  }
    , logs_format    { logs_format                }
    , files_to_use   { std::move( log_files )     }
{

}

CraplogParser::~CraplogParser()
{
    if ( QSqlDatabase::contains( this->db_conn_name ) ) {
        QSqlDatabase::removeDatabase( this->db_conn_name );
    }
}


void CraplogParser::sendPerfData() noexcept
{
    emit this->perfData(
        this->parsed_size,
        this->parsed_lines
    );
}

void CraplogParser::sendChartData() noexcept
{
    emit this->chartData(
        this->total_size,
        this->total_lines,
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
        this->proceed = !this->data_collection.empty();

        if ( this->proceed ) [[likely]] {
            // tell craplog to store the hashes before to proceed
            QWaitCondition wc;
            emit this->readyStoringData( &wc, &this->proceed );
            QMutex mutex; mutex.lock();
            wc.wait( &mutex );
            mutex.unlock();
        }
        if ( this->proceed ) [[likely]] {
            // store the new data
            auto db{ QSqlDatabase::addDatabase( "QSQLITE", this->db_conn_name ) };
            this->storeLogLines( db );
            this->db_edited |= this->proceed;
            if ( db.isOpen() ) {
                db.close();
            }
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
    for ( const auto& file_path : this->files_to_use ) {

        if ( ! this->proceed ) { break; }

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

            if ( this->web_server == WS_IIS ) {
                cleanLines( content );
            }

        // re-catched in run()
        } catch ( const GenericException& ) {
            // failed closing gzip file pointer
            throw GenericException( QStringLiteral("%1:\n%2").arg(
                    DialogSec::tr("An error occured while reading the gzipped file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch ( const std::ios_base::failure& ) {
            // failed reading as text
            throw GenericException( QStringLiteral("%1:\n%2").arg(
                    DialogSec::tr("An error occured while reading the file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch (...) {
            // failed somehow
            throw GenericException( QStringLiteral("%1:\n%2").arg(
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
    const auto parseLine{ [this]( const std::string& line, const LogsFormat& logs_format ) {
        this->data_collection.emplace_back( LogLineData(line, logs_format) );
        this->parsed_size += line.size();
        ++ this->parsed_lines;
    }};

    const auto signal_emission_gap{ [](const size_t n_lines)->size_t{
        return n_lines>10000ul ? n_lines/1000ul
                               : n_lines>1000ul ? n_lines/100ul
                                                : n_lines>100ul ? n_lines/10ul
                                                                : 10ul;
    }};


    // parse all the lines
    if ( this->proceed ) {
        const size_t n_lines{ this->logs_lines.size() };
        const size_t nl{ this->logs_format.new_lines };
        size_t send{ 0ul };
        if ( nl == 0ul ) {
            const size_t send_gap{ signal_emission_gap(n_lines) };
            const LogsFormat& lf {this->logs_format};
            this->data_collection.reserve( n_lines );
            for ( const std::string& line : this->logs_lines ) {
                parseLine( line, lf );
                if (++send == send_gap) {
                    this->sendPerfData();
                    send = 0ul;
                }
            }
        } else {
            const size_t real_lines{ n_lines / (nl+1ul) };
            const size_t send_gap{ signal_emission_gap(real_lines) };
            const LogsFormat& lf {this->logs_format};
            this->data_collection.reserve( real_lines );
            for ( size_t i{0ul}; i<n_lines; ++i ) {
                std::string line = this->logs_lines.at( i );
                for ( size_t n{0ul}; n<nl; ++n ) {
                    ++i;
                    line += "\n" + this->logs_lines.at( i );
                }
                parseLine( line, lf );
                if (++send == send_gap) {
                    this->sendPerfData();
                    send = 0ul;
                }
            }
        }
        this->sendPerfData();
    }
}



void CraplogParser::storeLogLines( QSqlDatabase& db )
{
    db.setDatabaseName( QString::fromStdString( this->db_path ) );

    if ( ! this->checkDatabaseFile() ) [[unlikely]] {
        this->proceed &= false;
        return;
    }

    if ( ! db.open() ) [[unlikely]] {
        this->proceed &= false;
        QString err_msg;
        if ( this->dialogs_level == DL_EXPLANATORY ) {
            err_msg = db.lastError().text();
        }
        emit this->showDialog( WorkerDialog::errDatabaseFailedOpening,
                              {this->db_name, err_msg} );
        return;
    }

    try {

        if ( ! db.transaction() ) [[unlikely]] {
            this->proceed &= false;
            QString stmt_msg, err_msg;
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                stmt_msg.append( "db.transaction()" );
                if ( this->dialogs_level == DL_EXPLANATORY ) {
                    err_msg = db.lastError().text();
                }
            }
            emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                   {this->db_name, stmt_msg, err_msg} );
            return;

        } else if ( this->storeData( db ) ) [[likely]] {

            if ( ! db.commit() ) [[unlikely]] {
                this->proceed &= false;
                QString stmt_msg, err_msg;
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    stmt_msg.append( "db.commit()" );
                    if ( this->dialogs_level == DL_EXPLANATORY ) {
                        err_msg.append( db.lastError().text() );
                    }
                }
                emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                       {this->db_name, stmt_msg, err_msg} );
            }
        }

        if ( ! this->proceed ) [[unlikely]] {
            // rollback the transaction
            if ( ! db.rollback() ) {
                QString stmt_msg, err_msg;
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    stmt_msg = "db.rollback()";
                    if ( this->dialogs_level == DL_EXPLANATORY ) {
                        err_msg = db.lastError().text();
                    }
                }
                emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                       {this->db_name, stmt_msg, err_msg} );
                return;
            }
        }

    } catch (...) {
        // wrongthing w3nt some.,.
        emit this->showDialog(
            WorkerDialog::errGeneric,
            {QStringLiteral("%1\n\n%2").arg(
                DialogSec::tr("An error occured while working on the database"),
                DialogSec::tr("Aborting") )} );
    }
}

#define APPEND_TO_QUERY_AS_NUMBER(LOG_FIELD)\
    if ( LOG_FIELD ) {\
        stmt.append( QString::fromStdString( *LOG_FIELD ).replace(QLatin1Char('\''),QLatin1String("''")) );\
    } else {\
        stmt.append( QStringLiteral("NULL") );\
    }
#define CONCAT_TO_QUERY_AS_NUMBER(LOG_FIELD)\
    stmt.append( QStringLiteral(", ") );\
    APPEND_TO_QUERY_AS_NUMBER(LOG_FIELD)

#define CONCAT_TO_QUERY_AS_STRING(LOG_FIELD)\
    stmt.append( QStringLiteral(", ") );\
    if ( LOG_FIELD ) {\
        stmt.append( QStringLiteral("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace(QLatin1Char('\''),QLatin1String("''")) ) );\
    } else {\
        stmt.append( QStringLiteral("NULL") );\
    }

// in IIS logs the user-agent is logged with '+' instead of ' ' (whitespace)
#define CONCAT_TO_QUERY_USERAGENT(LOG_FIELD)\
    stmt.append( QStringLiteral(", ") );\
    if ( LOG_FIELD ) {\
        if ( this->web_server == WS_IIS ) {\
            stmt.append( QStringLiteral("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace(QLatin1Char('+'),QLatin1Char(' ')).replace(QLatin1Char('\''),QLatin1String("''")) ) );\
        } else {\
            stmt.append( QStringLiteral("'%1'").arg( QString::fromStdString( *LOG_FIELD ).replace(QLatin1Char('\''),QLatin1String("''")) ) );\
        }\
    } else {\
        stmt.append( QStringLiteral("NULL") );\
    }

bool CraplogParser::storeData( QSqlDatabase& db )
{
    // get blacklist items
    const bool check_bl_cli { this->blacklist.client.used };

    const std::vector<std::string> empty;
    const std::vector<std::string>& bl_cli_list{ (check_bl_cli)
        ? this->blacklist.client.list
        : empty };

    // prepare the database related studd
    QString table;
    switch ( this->web_server ) {
        case WS_APACHE:
            table.append( "apache" );
            break;
        case WS_NGINX:
            table.append( "nginx" );
            break;
        case WS_IIS:
            table.append( "iis" );
            break;
        default:
            // wrong WebServerID, but should be unreachable because of the previous operations
            throw WebServerException( "Unexpected WebServer: " + toString(this->web_server) );
    }

    const QString stmt_template{
        QStringLiteral(R"(INSERT INTO "%1" ("year", "month", "day", "hour", "minute", "second", "protocol", "method", "uri", "query", "response", "time_taken", "bytes_sent", "bytes_received", "referrer", "client", "user_agent", "cookie") )"
                         "VALUES (")
    };

    /*int perf_size;*/
    // parse every row of data
    for ( const LogLineData& line_data : this->data_collection ) {

        // check blacklisted clients
        if ( check_bl_cli && line_data.client ) {
            if ( VecOps::contains( bl_cli_list, *line_data.client ) ) {
                this->blacklisted_size += line_data.size();
                continue;
            }
        }

        QString stmt{ stmt_template.arg( table ) };

        // complete and execute the statement, binding NULL if not found

        // date and time
        APPEND_TO_QUERY_AS_NUMBER(line_data.year) // 1
        CONCAT_TO_QUERY_AS_NUMBER(line_data.month) // 2
        CONCAT_TO_QUERY_AS_NUMBER(line_data.day) // 3
        CONCAT_TO_QUERY_AS_NUMBER(line_data.hour) // 4
        CONCAT_TO_QUERY_AS_NUMBER(line_data.minute) // 5
        CONCAT_TO_QUERY_AS_NUMBER(line_data.second) // 6

        // request
        CONCAT_TO_QUERY_AS_STRING(line_data.protocol) // 10
        CONCAT_TO_QUERY_AS_STRING(line_data.method) // 11
        CONCAT_TO_QUERY_AS_STRING(line_data.uri) // 12
        CONCAT_TO_QUERY_AS_STRING(line_data.query) // 13

        CONCAT_TO_QUERY_AS_NUMBER(line_data.response_code) // 14
        CONCAT_TO_QUERY_AS_NUMBER(line_data.time_taken) // 15
        CONCAT_TO_QUERY_AS_NUMBER(line_data.bytes_sent) // 16
        CONCAT_TO_QUERY_AS_NUMBER(line_data.bytes_received) // 17

        // client data and referrer
        CONCAT_TO_QUERY_AS_STRING(line_data.referrer) // 18
        CONCAT_TO_QUERY_AS_STRING(line_data.client) // 20
        CONCAT_TO_QUERY_USERAGENT(line_data.user_agent) // 21
        CONCAT_TO_QUERY_AS_STRING(line_data.cookie) // 22

        stmt.append( ");" );

        if ( QSqlQuery query(db); !query.exec( stmt ) ) [[unlikely]] {
            // error finalizing step
            QString query_msg, err_msg;
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                query_msg.append( "query.exec()" );
                if ( this->dialogs_level == DL_EXPLANATORY ) {
                    err_msg = query.lastError().text();
                }
            }
            emit this->showDialog( WorkerDialog::errDatabaseFailedExecuting,
                                   {this->db_name, query_msg, err_msg} );
            return false;
        }
    }

    return true;
}


bool CraplogParser::checkDatabaseFile() noexcept
{
    if ( ! IOutils::exists( this->db_path ) ) {
        emit this->showDialog( WorkerDialog::errDatabaseFileNotFound, {this->db_name} );
        return false;
    } else if ( ! IOutils::isFile( this->db_path ) ) {
        emit this->showDialog( WorkerDialog::errDatabaseFileNotFile, {this->db_name} );
        return false;
    } else if ( ! IOutils::checkFile( this->db_path, true ) ) {
        emit this->showDialog( WorkerDialog::errDatabaseFileNotReadable, {this->db_name} );
        return false;
    } else if ( ! IOutils::checkFile( this->db_path, false, true ) ) {
        emit this->showDialog( WorkerDialog::errDatabaseFileNotWritable, {this->db_name} );
        return false;
    }
    return true;
}

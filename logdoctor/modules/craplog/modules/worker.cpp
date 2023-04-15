
#include "worker.h"

#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/strings.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/craplog/craplog.h"
#include "datetime.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


CraplogWorker::CraplogWorker( const unsigned web_server_id, const unsigned dialogs_level, const std::string& db_data_path, const std::string& db_hashes_path, const LogsFormat& logs_format, const bw_lists_t& blacklists, const bw_lists_t& warnlists, const worker_files_t& log_files, QObject* parent )
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


void CraplogWorker::sendPerfData()
{
    emit this->perfData(
        static_cast<unsigned>( this->parsed_size ),
        this->parsed_lines
    );
}

void CraplogWorker::sendChartData()
{
    emit this->chartData(
        static_cast<unsigned>( this->total_size ),
        this->total_lines,
        static_cast<unsigned>( this->warnlisted_size ),
        static_cast<unsigned>( this->blacklisted_size )
    );
}


void CraplogWorker::work()
{
    this->proceed |= true;
    this->db_edited &= false;
    try {
        if ( this->proceed ) {
            // collect log lines
            this->joinLogLines();
        }
        if ( this->proceed ) {
            // parse the log lines to fill the collection
            emit this->startedParsing();
            this->parseLogLines();
            emit this->finishedParsing();
        }
        // clear log lines data
        this->logs_lines.clear();

        if ( this->proceed && this->parsed_size > 0 ) {
            // store the new data
            this->storeLogLines();
            this->db_edited |= this->proceed;
        }

    } catch ( GenericException& e ) {
        DialogSec::errGeneric( e.what() );
        this->proceed &= false;

    } catch ( LogParserException& e ) {
        DialogSec::errFailedParsingLogs( e.what() );
        this->proceed &= false;
    }
    // send the final data
    if ( ! this->proceed ) {
        this->total_lines  = 0;
        this->parsed_lines = 0;
        this->total_size   = 0;
        this->parsed_size  = 0;
        this->warnlisted_size  = 0;
        this->blacklisted_size = 0;
    }
    this->sendPerfData();
    this->sendChartData();
    emit this->done( this->db_edited );
    emit this->retire();
}


void CraplogWorker::joinLogLines()
{
    const auto cleanLines = [](std::vector<std::string>& lines) {
        std::vector<std::string> aux;
        aux.reserve( lines.size() );
        for ( const std::string& line : lines ) {
            if ( line.front() != '#' ) {
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

            if ( this->wsID == this->IIS_ID ) {
                cleanLines( content );
            }

        // re-catched in run()
        } catch ( const GenericException& ) {
            // failed closing gzip file pointer
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("An error accured while reading the gzipped file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch ( const std::ios_base::failure& ) {
            // failed reading as text
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("An error accured while reading the file"),
                    QString::fromStdString( file_path )
                ).toStdString() );

        } catch (...) {
            // failed somehow
            throw GenericException( QString("%1:\n%2").arg(
                    DialogSec::tr("Something failed while handling the file"),
                    QString::fromStdString( file_path )
                ).toStdString() );
        }

        // append to the relative list
        this->logs_lines.insert( this->logs_lines.end(), content.begin(), content.end() );
    }
    this->files_to_use.clear();
    if ( this->logs_lines.empty() ) {
        this->proceed &= false;
    }
}


void CraplogWorker::parseLogLines()
{
    const auto parseLine = [this]( const std::string& line ) {
        log_line_data_t data;
        std::string_view sep;
        std::string fld_str;
        bool add_pm{false}, ok{true};
        size_t start, stop{this->logs_format.initial.size()},
               sep_i{0};
        const size_t line_size{ line.size()-1ul },
                     n_seps{ this->logs_format.separators.size()-1ul };

        while (true) {
            // split fields
            start = stop; // stop updated at the end of the loop
            if ( sep_i <= n_seps ) {
                sep = this->logs_format.separators.at( sep_i );
                stop = line.find( sep, start );
            } else if ( sep_i == n_seps+1ul ) {
                // final separator
                sep = this->logs_format.final;
                if ( sep.empty() ) {
                    stop = line_size+1ul;
                } else {
                    stop = line.find( sep, start );
                    if ( stop == std::string::npos ) {
                        stop = line_size+1ul;
                    }
                }
            } else {
                // no more separators
                break;
            }
            if ( stop == std::string::npos ) {
                // separator not found, abort
                throw LogParserException( "Separator not found", std::string{sep} );
            }
            const size_t sep_size = sep.size();

            // get the field
            const std::string& fld = this->logs_format.fields.at( sep_i );
            if ( fld != "NONE" ) {
                // only parse the considered fields
                fld_str = StringOps::strip( line.substr(start, stop-start), ' ' );

                if ( sep_i+1ul <= n_seps ) {
                    // not the last separator, check for mistakes
                    ok |= true;
                    size_t aux_stop = stop;

                    if ( sep == " " ) {
                        // whitespace-separated-values fields
                        size_t c{ static_cast<size_t>( std::count( fld_str.cbegin(), fld_str.cend(), ' ' ) ) },
                               n{ 0 };
                        if ( fld == "request_full" ) {
                            n += 2ul;
                        } else if ( fld == "date_time_mcs" ) {
                            n += 4ul;
                        } else if ( fld == "date_time_ncsa" ) {
                            n += 1ul;
                        } else if ( fld == "date_time_gmt" ) {
                            n += 3ul;
                        }
                        if ( n > 0ul && c < n ) {
                            // loop until the correct number of whitespaces is reached
                            size_t aux_start = stop+1ul;
                            while ( c < n ) {
                                aux_stop = line.find( sep, aux_start );
                                if ( aux_stop == std::string::npos ) {
                                    // not found
                                    ok &= false;
                                    break;
                                }
                                aux_start = aux_stop+1ul;
                                c++;
                            }
                        }

                    } else if ( sep.front() == '"' && fld == "user_agent" ) {
                        // atm the only support is for escaped quotes
                        if ( fld_str.back() == '\\' ) {
                            size_t aux_start = stop + sep_size;
                            while (true) {
                                aux_stop = line.find( sep, aux_start );
                                if ( aux_stop == std::string::npos ) {
                                    // not found
                                    break;
                                } else if ( line.at( aux_stop-1ul ) != '\\' ) {
                                    // non-backslashed quotes
                                    break;
                                }
                                aux_start = aux_stop + sep_size;
                            }
                        }
                    }

                    // finally update if needed
                    if ( ok && aux_stop >= stop ) {
                        stop = aux_stop;
                        fld_str = StringOps::strip( line.substr(start, stop-start), ' ' );
                    }
                }

                if ( ! fld_str.empty() ) {
                    // process the field
                    const int& fld_id{ this->field2id.at(fld) };
                    if ( fld_id > 0 ) {
                        // no need to process, append directly if non-empty
                        if ( fld_id == 13 && fld_str == "-" ) {
                            continue;
                        }
                        data.emplace( fld_id, fld_str );

                    } else {
                        // process the field

                        // process the date to get year, month, day, hour and minute
                        if ( fld.find("date_time") == 0ul ) {
                            const auto dt = DateTimeOps::processDateTime( fld_str, fld.substr( 10 ) ); // cut away the "date_time_" part
                            if ( ! dt.at( 0 ).empty() ) {
                                // year
                                data.emplace( this->field2id.at("date_time_year"), dt.at( 0 ) );
                            }
                            if ( ! dt.at( 1 ).empty() ) {
                                // month
                                data.emplace( this->field2id.at("date_time_month"), dt.at( 1 ) );
                            }
                            if ( ! dt.at( 2 ).empty() ) {
                                // day
                                data.emplace( this->field2id.at("date_time_day"), dt.at( 2 ) );
                            }
                            if ( ! dt.at( 3 ).empty() ) {
                                // hour
                                if ( dt.at( 3 ) == "PM" ) {
                                    add_pm |= true;
                                } else {
                                    data.emplace( this->field2id.at("date_time_hour"), dt.at( 3 ) );
                                }
                            }
                            if ( ! dt.at( 4 ).empty() ) {
                                // minute
                                data.emplace( this->field2id.at("date_time_minute"), dt.at( 4 ) );
                            }
                            if ( ! dt.at( 5 ).empty() ) {
                                // second
                                data.emplace( this->field2id.at("date_time_second"), dt.at( 5 ) );
                            }


                        // process the request to get the protocol, method, resource and query
                        } else if ( fld == "request_full" ) {
                            size_t aux;
                            std::string protocol, method, uri, query,
                                        aux_fld{ fld_str };
                            // method
                            aux = aux_fld.find( ' ' );
                            if ( aux != std::string::npos ) {
                                method  = aux_fld.substr( 0ul, aux );
                                aux_fld = StringOps::lstrip( aux_fld.substr( aux+1ul ) );

                                // page & query
                                aux = aux_fld.find( ' ' );
                                if ( aux != std::string::npos ) {
                                    const std::string aux_str{ aux_fld.substr( 0ul, aux ) };
                                    // search for the query
                                    const size_t aux_{ aux_str.find( '?' ) };
                                    if ( aux_ != std::string::npos ) {
                                        uri   = aux_str.substr( 0ul, aux_ );
                                        query = aux_str.substr( aux_+1ul  );
                                    } else {
                                        // query not found
                                        uri = aux_str;
                                    }
                                    // protocol
                                    protocol = StringOps::lstrip( aux_fld.substr( aux+1ul ) );
                                }
                            }
                            // append non-empty data
                            if ( ! protocol.empty() ) {
                                data.emplace( this->field2id.at("request_protocol"), protocol );
                            }
                            if ( ! method.empty() ) {
                                data.emplace( this->field2id.at("request_method"), method );
                            }
                            if ( ! uri.empty() ) {
                                data.emplace( this->field2id.at("request_uri"), uri );
                            }
                            if ( ! query.empty() ) {
                                data.emplace( this->field2id.at("request_query"), query );
                            }


                        // process the request to get uri and query
                        } else if ( fld == "request_uri_query" ) {
                            // search for the query
                            std::string uri, query;
                            const size_t aux_{ fld_str.find( '?' ) };
                            if ( aux_ != std::string::npos ) {
                                uri   = fld_str.substr( 0ul, aux_ );
                                query = fld_str.substr( aux_+1ul  );
                            } else {
                                // query not found
                                uri = fld_str;
                            }
                            if ( ! uri.empty() ) {
                                data.emplace( this->field2id.at("request_uri"), uri );
                            }
                            if ( ! query.empty() ) {
                                data.emplace( this->field2id.at("request_query"), query );
                            }


                        // process the time taken to convert to milliseconds
                        } else if ( fld.find("time_taken_") == 0ul ) {
                            float t{ std::stof( fld_str ) };
                            const std::string u{ fld.substr( 11ul ) };
                            if ( u == "us" ) {
                                // from microseconds
                                t /= 1000.0f;
                            } else if ( u == "s" || u == "s.ms" ) {
                                // from seconds
                                t *= 1000.0f;
                            }
                            data.emplace( this->field2id.at("time_taken"), std::to_string( static_cast<int>( t ) ) );


                        // something went wrong
                        } else {
                            // hmmm.. no...
                            throw LogParserException( "Unexpected LogField", fld );
                        }
                    }
                }
            }

            // update the stop for the next start
            stop += sep_size;
            sep_i++;
            if ( stop > line_size ) {
                // this was the final separator
                break;
            }

        }

        if ( add_pm ) {
            try {
                // add +12 hours for PM
                data.at( 4 ) = std::to_string( 12 + std::stoi(data.at( 4 )) );
            } catch (...) {
                // no hour data
            }
        }

        this->data_collection.push_back( data );
        // update performance data
        this->parsed_size += line_size;
        this->parsed_lines ++;
        this->sendPerfData();
    };


    // parse all the lines
    if ( this->proceed ) {
        const size_t n_lines{ this->logs_lines.size() };
        const size_t nl{ this->logs_format.new_lines };
        if ( nl == 0ul ) {
            this->data_collection.reserve( n_lines );
            for ( const std::string& line : this->logs_lines ) {
                parseLine( line );
            }
        } else {
            this->data_collection.reserve( n_lines / (nl+1ul) );
            for ( size_t i{0ul}; i<n_lines; i++ ) {
                std::string line = this->logs_lines.at( i );
                for ( size_t n{0ul}; n<nl; n++ ) {
                    i++;
                    line += "\n" + this->logs_lines.at( i );
                }
                parseLine( line );
            }
        }
    }
}



void CraplogWorker::storeLogLines()
{
    QString db_path{ QString::fromStdString( this->db_data_path ) };
    QString db_name{ QString::fromStdString( this->db_data_path.substr( this->db_data_path.find_last_of( '/' ) + 1ul ) ) };

    QSqlDatabase db{ QSqlDatabase::addDatabase("QSQLITE") };
    db.setDatabaseName( db_path );

    if ( ! db.open() ) {
        // error opening database
        this->proceed &= false;
        QString err_msg;
        if ( this->dialogs_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( db_name, err_msg );

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
                DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );
            }

            if ( this->proceed && !this->data_collection.empty() ) {
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
                    DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );
                }
            }
            if ( ! proceed ) {
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
                DialogSec::errDatabaseFailedExecuting( db_name, stmt_msg, err_msg );
                err_shown = true;
            }
            if ( ! err_shown ) {
                // show a message
                DialogSec::errGeneric( QString("%1\n\n%2").arg(
                    DialogSec::tr("An error occured while working on the database"),
                    DialogSec::tr("Aborting") ) );
            }
        }

        db.close();
    }

}

const bool CraplogWorker::storeData( QSqlDatabase& db )
{
    const QString db_name{ QString::fromStdString(
        this->db_data_path.substr(
            this->db_data_path.find_last_of( '/' ) + 1ul ) ) };

    // get blacklist/warnlist items
    const bool check_bl_cli { this->blacklists.at( 20 ).used };
    const bool check_wl_met { this->warnlists.at( 11 ).used  };
    const bool check_wl_req { this->warnlists.at( 12 ).used  };
    const bool check_wl_cli { this->warnlists.at( 20 ).used  };
    const bool check_wl_ua  { this->warnlists.at( 21 ).used  };

    const std::vector<std::string> empty;
    const std::vector<std::string>& bl_cli_list{ (check_bl_cli)
        ? this->blacklists.at( 20 ).list
        : empty };

    const std::vector<std::string>& wl_met_list{ (check_wl_met)
        ? this->warnlists.at( 11 ).list
        : empty };

    const std::vector<std::string>& wl_req_list{ (check_wl_req)
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
        case 11:
            table += "apache";
            break;
        case 12:
            table += "nginx";
            break;
        case 13:
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
    for ( const log_line_data_t& row : this->data_collection ) {

        // check blacklisted clients
        if ( check_bl_cli ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row does contain this row item, check if they match
                const std::string& target{ row.at( 20 ) };
                if ( std::any_of( bl_cli_list.cbegin(), bl_cli_list.cend(),
                                  [&target]( const std::string& item )
                                           { return target.find( item ) == 0ul; }) ) {
                    // append every field to ignored size
                    this->blacklisted_size += std::accumulate( row.cbegin(), row.cend(), 0ul,
                                                               []( size_t size, const auto& item )
                                                                 { return size + item.second.size(); });
                    continue;
                }
            }
        }

        // check warnlisted clients
        if ( check_wl_cli ) {
            if ( row.find( 20 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target{ row.at( 20 ) };
                if ( std::any_of( wl_cli_list.cbegin(), wl_cli_list.cend(),
                                  [&target]( const std::string& item )
                                           { return target.find( item ) == 0ul; }) ) {
                    // match found! put a warning on this line
                    warning |= true;
                }
            }
        }
        // check warnlisted user-agents
        if ( check_wl_ua && !warning ) {
            if ( row.find( 21 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target{ row.at( 21 ) };
                if ( std::any_of( wl_ua_list.cbegin(), wl_ua_list.cend(),
                                  [&target]( const std::string& item )
                                           { return target.find( item ) == 0ul; }) ) {
                    // match found! skip this line
                    warning |= true;
                }
            }
        }
        // check warnlisted methods
        if ( check_wl_met && !warning ) {
            if ( row.find( 11 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target{ row.at( 11 ) };
                if ( std::any_of( wl_met_list.cbegin(), wl_met_list.cend(),
                                  [&target]( const std::string& item )
                                           { return item == target; }) ) {
                    // match found! skip this line
                    warning |= true;
                }
            }
        }
        // check warnlisted requests URIs
        if ( check_wl_req && !warning ) {
            if ( row.find( 12 ) != row.end() ) {
                // this row do contains this row item, check if they match
                const std::string& target{ row.at( 12 ) };
                if ( std::any_of( wl_req_list.cbegin(), wl_req_list.cend(),
                                  [&target]( const std::string& item )
                                           { return target.find( item ) == 0ul; }) ) {
                    // match found! skip this line
                    warning |= true;
                }
            }
        }


        // initialize the SQL statement
        QString query_stmt{ "INSERT INTO \""+table+"\" (\"warning\", \"year\", \"month\", \"day\", \"hour\", \"minute\", \"second\", \"protocol\", \"method\", \"uri\", \"query\", \"response\", \"time_taken\", \"bytes_sent\", \"bytes_received\", \"referrer\", \"client\", \"user_agent\", \"cookie\") "
                            "VALUES (" };

        // complete and execute the statement, binding NULL if not found

        // warning
        if ( warning ) {
            warning &= false;
            this->warnlisted_size += std::accumulate( row.cbegin(), row.cend(), 0ul,
                                                      []( size_t size, const auto& item )
                                                        { return size + item.second.size(); });
            query_stmt += "1";
        } else {
            query_stmt += "0";
        }

        // date and time
        for ( int i{1}; i<7; i++ ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.cend() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                query_stmt += QString::fromStdString( row.at( i ) ).replace("'","''");
            }
        }

        // request
        for ( int i{10}; i<14; i++ ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.cend() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
            }
        }

        for ( int i{14}; i<18; i++ ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.cend() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                query_stmt += QString::fromStdString( row.at( i ) ).replace("'","''");
            }
        }

        // client data and referrer
        for ( const int& i : std::vector<int>{18,20,21,22} ) {
            query_stmt += ", ";
            if ( row.find( i ) == row.cend() ) {
                // no value found in the collection, bind NULL
                query_stmt += "NULL";
            } else {
                // value found, bind it
                if ( i == 21 && this->wsID == this->IIS_ID ) {
                    // iis logs the user-agent using '+' instead of ' ' (spaces)
                    QString str = QString::fromStdString( row.at( i ) ).replace("+"," ");
                    query_stmt += QString("'%1'").arg( str.replace("'","''") );
                } else {
                    query_stmt += QString("'%1'").arg( QString::fromStdString( row.at( i ) ).replace("'","''") );
                }
            }
        }

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
            DialogSec::errDatabaseFailedExecuting( db_name, query_msg, err_msg );
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
            DialogSec::errDatabaseFailedExecuting( db_name, query_msg, err_msg );
            return false;
        }

        // reset the statement to prepare for the next one
        query.finish();
    }

    return true;
}

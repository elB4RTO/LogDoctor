
#include "craplog.h"

#include "utilities.h"
#include "tools/craplog/modules/store.h"

#include <filesystem>

#include <iostream>
#include <thread>

using std::string, std::vector, std::unordered_map;


Craplog::Craplog()
{
    ////////////////////////
    //// INITIALIZATION ////
    ////////////////////////
    // blacklists / whitelists
    for ( int i=11; i<14; i++ ) {
        this->warnlists[ i ].emplace( 1, std::unordered_map<int, BWlist>() ); // access
        this->warnlists[ i ].emplace( 2, std::unordered_map<int, BWlist>() ); // error
        // access
        this->warnlists[ i ][ 1 ].emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists[ i ][ 1 ].emplace( 11, BWlist{ .used=false, .list={"DELETE","HEAD","OPTIONS","PUT","PATCH"} } );
        this->warnlists[ i ][ 1 ].emplace( 12, BWlist{ .used=false, .list={} } );
        this->blacklists[ i ][ 1 ].emplace( 20, BWlist{ .used=true,  .list={"::1"} } );
        // error
        this->warnlists[ i ][ 2 ].emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists[ i ][ 2 ].emplace( 31, BWlist{ .used=false, .list={} } );
        this->blacklists[ i ][ 2 ].emplace( 20, BWlist{ .used=true,  .list={"::1"} } );
        this->blacklists[ i ][ 2 ].emplace( 31, BWlist{ .used=false, .list={} } );
    }

    // default format strings
    this->logs_format_stings[11] = unordered_map<int, string>();
    this->logs_format_stings[11][1] = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    this->logs_format_stings[11][2] = "[%t] [%l] [pid %P] %F: %E: [client %a] %M";
    this->logs_format_stings[12] = unordered_map<int, string>();
    this->logs_format_stings[12][1] = "$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"";
    this->logs_format_stings[12][2] = "$time_iso8601 [$error_level] $pid: *$cid $error_message";
    this->logs_format_stings[13] = unordered_map<int, string>();
    this->logs_format_stings[13][1] = "date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs-version cs(User-Agent) cs(Cookie) cs(Referer) cs-host sc-status sc-substatus sc-win32-status sc-bytes cs-bytes time-taken";
    this->logs_format_stings[13][2] = "";

    // initialize format strings
    this->logs_formats[11][1] = this->formatOps.processApacheFormatString( this->logs_format_stings.at(11).at(1), 1 );
    this->logs_formats[11][2] = this->formatOps.processApacheFormatString( this->logs_format_stings.at(11).at(2), 2 );
    this->logs_formats[12][1] = this->formatOps.processFormatString( this->logs_format_stings.at(12).at(1), 1, 12 );
    this->logs_formats[12][2] = this->formatOps.processFormatString( this->logs_format_stings.at(12).at(2), 2, 12 );
    //this->logs_formats[13][1] = this->formatOps.processFormatString( this->logs_format_stings.at(13).at(1), 1, 13 );
    //this->logs_formats[13][2] = this->formatOps.processFormatString( this->logs_format_stings.at(13).at(2), 2, 13 );

    this->current_ALF = this->logs_formats[11][1];
    this->current_ELF = this->logs_formats[11][2];

    // apache2 access/error logs location
    this->logs_paths.emplace( 11, unordered_map<int, string>() );
    this->logs_paths[11].emplace( 1, "/var/log/apache2" );
    this->logs_paths[11].emplace( 2, "/var/log/apache2" );
    // nginx access/error logs location
    this->logs_paths.emplace( 12, unordered_map<int, string>() );
    this->logs_paths[12].emplace( 1, "/var/log/nginx" );
    this->logs_paths[12].emplace( 2, "/var/log/nginx" );
    // iis access/error logs location
    this->logs_paths.emplace( 13, unordered_map<int, string>() );
    this->logs_paths[13].emplace( 1, "C:\\inetpub\\logs\\LogFiles\\W3SVC" );
    this->logs_paths[13].emplace( 2, "C:\\Windows\\System32\\LogFiles\\HTTPERR" );

    // apache2 access/error log files' names
    this->logs_base_names.emplace( 11, unordered_map<int, LogName>() );
    this->logs_base_names[11].emplace( 1, LogName { .starts   = "access.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    this->logs_base_names[11].emplace( 2, LogName { .starts   = "error.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    // nginx access/error log files' names
    this->logs_base_names.emplace( 12, unordered_map<int, LogName>() );
    this->logs_base_names[12].emplace( 1, LogName { .starts   = "access.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    this->logs_base_names[12].emplace( 2, LogName { .starts   = "error.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    // iis access/error log files' names
    this->logs_base_names.emplace( 13, unordered_map<int, LogName>() );
    this->logs_base_names[13].emplace( 1, LogName { .starts   = "nc",
                                                    .contains = "",
                                                    .ends     = ".log" });
    this->logs_base_names[13].emplace( 2, LogName { .starts   = "",
                                                    .contains = "",
                                                    .ends     = "" });


    // access logs data
    this->data_collection[1] = std::vector<std::unordered_map<int, std::string>>();
    // error logs data
    this->data_collection[2] = std::vector<std::unordered_map<int, std::string>>();

    // hashes of newly parsed files
    this->used_files_hashes[1] = std::vector<std::string>();
    this->used_files_hashes[2] = std::vector<std::string>();

    ///////////////////////
    //// CONFIGURATION ////
    ///////////////////////
    /*this->readConfigs();

    this->hashOps.readLists( this->configs_path );*/

}


//////////////////
//// SETTINGS ////
int Craplog::getWarningSize()
{
    return this->warning_size;
}

void Craplog::setWarningSize( const int new_size )
{
    this->warning_size = new_size;
}


// get the logs format
FormatOps::LogsFormat Craplog::getAccessLogsFormat( const int web_server_id ) &
{
    return this->logs_formats.at( web_server_id ).at( 1 );
}
FormatOps::LogsFormat Craplog::getErrorLogsFormat( const int web_server_id ) &
{
    return this->logs_formats.at( web_server_id ).at( 2 );
}

// set the logs format
void Craplog::setAccessLogsFormat( const int web_server_id, const std::string& format_string )
{
    this->logs_formats.at( web_server_id ).at( 1 ) = this->formatOps.processFormatString(
        this->logs_format_stings.at( web_server_id ).at( 1 ),
        1, web_server_id );
}
void Craplog::setErrorLogsFormat(const int web_server_id, const std::string& format_string )
{
    this->logs_formats.at( web_server_id ).at( 2 ) = this->formatOps.processFormatString(
        this->logs_format_stings.at( web_server_id ).at( 2),
        2, web_server_id );
}


// set the current Web Server
void Craplog::setCurrentWSID( const int web_server_id )
{
    this->current_WS = web_server_id;
    this->setCurrentALF();
    this->setCurrentELF();
}

int Craplog::getCurrentWSID() const
{
    return this->current_WS;
}

// set the current access logs format
void Craplog::setCurrentALF()
{
    this->current_ALF = this->logs_formats.at( this->current_WS ).at( 1 );
}
// set the current error logs format
void Craplog::setCurrentELF()
{
    this->current_ELF = this->logs_formats.at( this->current_WS ).at( 2 );
}

// get the current access logs format
FormatOps::LogsFormat Craplog::getCurrentALF() const&
{
    return this->current_ALF;
}
// get the current error logs format
FormatOps::LogsFormat Craplog::getCurrentELF() const&
{
    return this->current_ELF;
}



///////////////////
//// LOGS LIST ////
// return the size of the list
int Craplog::getLogsListSize() {
    return this->logs_list.size();
}

// return the list. rescan if fresh is true
vector<Craplog::LogFile> Craplog::getLogsList( const bool fresh )
{
    if ( fresh == true ) {
        this->scanLogsDir();
    }
    return this->logs_list;
}


// return the path of the file matching the given name
Craplog::LogFile Craplog::getLogFileItem( const QString& file_name )
{
    LogFile logfile;
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            logfile = item;
            break;
        }
    }
    return logfile;
}


// return the path of the file matching the given name
string Craplog::getLogFilePath( const QString& file_name )
{
    string path;
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            path = item.path;
            break;
        }
    }
    return path;
}

// set a file as selected
bool Craplog::setLogFileSelected( const QString& file_name )
{
    bool result = false;
    for ( Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            item.selected = true;
            result = true;
            break;
        }
    }
    return result;
}


// scan the logs path to update the log files list
void Craplog::scanLogsDir()
{
    this->logs_list.clear();
    int n=2;
    auto logs_paths = this->logs_paths.at( this->current_WS );
    if ( logs_paths.at(1) == logs_paths.at(2) ) {
        // same dir for both access and error logs, loop only once
        n=1;
    }
    for ( int i=0; i<n; i++ ) {
        string logs_path = logs_paths.at( i+1 );
        if ( !IOutils::isDir( logs_path ) ) {
            // this directory doesn't exists
            if ( IOutils::exists( logs_path ) ) {
                DialogSec::errDirNotExists( nullptr, logs_path );
            }
            continue;
        }
        // iterate over entries in the logs folder
        for ( const auto& dir_entry : std::filesystem::directory_iterator{logs_path}) {
            // get the attributes
            int size = dir_entry.file_size();
            string path = dir_entry.path().string();
            string name = dir_entry.path().filename().string();
            // check the readability
            if ( IOutils::checkFile(path,true) == false ) {
                continue;
            }

            bool proceed = true;
            std::vector<std::string> content;
            try {
                content = IOutils::readLines( path );
            } catch (const std::ios_base::failure& err) {
                // failed reading
                proceed = false;
                // >> err.what() << //
                DialogSec::errFailedReadFile( nullptr, name );
            } catch (...) {
                // failed somehow
                proceed = false;
                QString err_msg = QMessageBox::tr("An error occured while handling the file");
                DialogSec::errGeneric( nullptr, err_msg +":\n"+ QString::fromStdString(name) );
            }

            if ( content.size() == 0 ) {
                if ( this->dialogs_Level > 0 ) {
                    DialogSec::warnEmptyFile( nullptr, name );
                }
                continue;
            }

            if ( proceed == true ) {
                LogOps::LogType log_type = this->logOps.defineFileType( name, content, this->logs_formats.at( this->current_WS ) );
                if ( log_type == LogOps::LogType::Failed ) {
                    // failed to get the log type, do not append
                    DialogSec::errFailedDefineLogType( nullptr, name );
                    continue;
                }

                // match only valid files names
                if ( this->isFileNameValid( name, log_type ) == false ) {
                    continue;
                }

                std::string hash = this->hashOps.digestFile( path );

                auto logfile = LogFile{
                    .selected = false,
                    .used_already = this->hashOps.hasBeenUsed( hash, this->current_WS, log_type ),
                    .size = size,
                    .name = QString::fromStdString( name ),
                    .hash = hash,
                    .path = path,
                    .type = log_type
                };
                // push in the list
                this->logs_list.push_back( logfile );
            }
        }
    }
}



bool Craplog::isFileNameValid( const std::string& name, const LogOps::LogType& log_type )
{
    bool valid = true;
    if ( this->logs_base_names.at( this->current_WS ).at( log_type ).starts != "" ) {
        if ( ! StringOps::startsWith( name, this->logs_base_names.at( this->current_WS ).at( log_type ).starts ) ) {
            return false;
        }
    }
    if ( this->logs_base_names.at( this->current_WS ).at( log_type ).contains != "" ) {
        if ( ! StringOps::contains(
                    name.substr( this->logs_base_names.at( this->current_WS ).at( log_type ).starts.size() ),
                    this->logs_base_names.at( this->current_WS ).at( log_type ).contains ) ) {
            return false;
        }
    }
    if ( this->logs_base_names.at( this->current_WS ).at( log_type ).ends != "" ) {
        if ( ! StringOps::endsWith( name, this->logs_base_names.at( this->current_WS ).at( log_type ).ends ) ) {
            return false;
        }
    }

    switch ( this->current_WS ) {
        int start, stop;
        case 11 | 12:
            // further checks for apache / nginx
            start = name.find_last_of( ".log." )+1;
            stop = name.find( ".gz", start);
            if ( stop < 0 || stop >= name.size() ) {
                stop = name.size()-1;
            }
            // serach for incremental numbers
            for ( int i=start; i<=stop; i++ ) {
                if ( StringOps::isNumeric( name.at( i ) ) == false ) {
                    valid = false;
                    break;
                }
            }
            break;

        case 13:
            // further checks for iis
            start = this->logs_base_names.at( 13 ).at( log_type ).starts.size();
            stop = name.size() - this->logs_base_names.at( 13 ).at( log_type ).ends.size();
            // search for incremental number / date
            for ( int i=start; i<=stop; i++ ) {
                if ( StringOps::isNumeric( name.at( i ) ) == false ) {
                    valid = false;
                    break;
                }
            }
            break;
    }
    return valid;
}


///////////////
//// WORKK ////
void Craplog::startWorking()
{
    this->working = true;
    this->parsing = true;
    this->proceed = true;
    this->total_size = 0;
    this->parsed_size = 0;
    this->parsed_lines = 0;
    this->access_size = 0;
    this->error_size = 0;
    this->data_collection[1].clear();
    this->data_collection[2].clear();
    this->access_logs_lines.clear();
    this->error_logs_lines.clear();
    this->used_files_hashes[1].clear();
    this->used_files_hashes[2].clear();
}
void Craplog::stopWorking()
{
    this->working = false;
    this->parsing = false;
    this->data_collection[1].clear();
    this->data_collection[2].clear();
    this->used_files_hashes[1].clear();
    this->used_files_hashes[2].clear();
}
bool Craplog::isWorking()
{
    return this->working;
}
bool Craplog::isParsing()
{
    return this->parsing;
}

// performances
int Craplog::getPerfSize()
{
    return this->perf_size;
}
int Craplog::getTotalSize()
{
    return this->total_size;
}
int Craplog::getParsedSize()
{
    return this->parsed_size;
}
int Craplog::getParsedLines()
{
    return this->parsed_lines;
}
int Craplog::getAccessSize()
{
    return this->access_size;
}
int Craplog::getErrorSize()
{
    return this->error_size;
}

void Craplog::sumIgnoredSize( const int size, const int log_type )
{
    if ( log_type == 1 ) {
        this->ignored_access_size += size;
    } else if ( log_type == 2 ) {
        this->ignored_error_size += size;
    } else {
        // wrong log_type
    }
}

void Craplog::collectPerfData()
{
    this->parsed_size  = this->logOps.getSize();
    this->parsed_lines = this->logOps.getLines();
    this->perf_size    = this->parsed_size;
}


void Craplog::run()
{
    this->startWorking();

    // collect log lines
    this->joinLogLines();
    // parse the log lines to fill the collection
    this->parseLogLines();
    // clear log lines data
    this->access_logs_lines.clear();
    this->error_logs_lines.clear();
    // finished parsing logs
    this->parsing = false;
    this->perf_size = this->parsed_size;


    // insert the hashes of the used files
    this->hashOps.insertUsedHashes( this->used_files_hashes, this->current_WS );

    this->stopWorking();
}



void Craplog::joinLogLines()
{
    std::vector<std::string> content;
    for ( const LogFile& file : this->logs_list ) {

        if ( this->proceed == false ) { break; }

        if ( file.selected == false ) {
            // not selected, skip
            continue;
        }

        // check again the type
        if ( file.type == LogOps::LogType::Failed ) {
            bool choice = DialogSec::choiceUndefinedLogType( nullptr, file.name );
            if ( choice == false ) {
                // choosed to abort all
                this->proceed = false;
                break;
            } else {
                // choosed to discard and continue
                continue;
            }
        }

        // check if the file has been used already
        if ( file.used_already == true ) {
            // already used
            QString msg = file.name;
            if ( this->dialogs_Level == 2 ) {
                msg += "\n" + QString::fromStdString( file.hash );
            }
            int choice = DialogSec::choiceFileAlreadyUsed( nullptr, msg );
            if ( choice < 0 ) {
                // choosed to abort all
                this->proceed = false;
                break;
            } else if ( choice > 0 ) {
                // choosed to discard and continue
                continue;
            }/* else {
                // choosed to ignore and use the file anyway
            }*/
        }

        // collect lines
        content.clear();
        try {
            // try reading
            content = StringOps::splitrip( IOutils::readFile( file.path ) );
        } catch (const std::ios_base::failure& err) {
            // failed reading
            // >> err.what() << //
            DialogSec::errFailedReadFile( nullptr, file.name, true );
            continue;
        } catch (...) {
            // failed somehow
            DialogSec::errFailedReadFile( nullptr, file.name, true );
            continue;
        }

        // append to the relative list
        if ( file.type == LogOps::LogType::Access ) {
            this->access_logs_lines.insert( this->access_logs_lines.end(), content.begin(), content.end() );
            this->used_files_hashes.at( 1 ).push_back( file.hash );
        } else {
            this->error_logs_lines.insert( this->error_logs_lines.end(), content.begin(), content.end() );
            this->used_files_hashes.at( 2 ).push_back( file.hash );
        }

        this->total_size += file.size;
        this->total_lines += content.size();
    }
}


void Craplog::parseLogLines()
{
    if ( this-> proceed == true && this->access_logs_lines.size() > 0 ) {
        this->data_collection[1] = this->logOps.parseLines(
            this->access_logs_lines,
            this->logs_formats.at( this->current_WS ).at( 1 ) );
    }

    if ( this-> proceed == true && this->error_logs_lines.size() > 0 ) {
        this->data_collection[2] = this->logOps.parseLines(
            this->error_logs_lines,
            this->logs_formats.at( this->current_WS ).at( 2 ) );
    }
}


bool Craplog::isBlacklistUsed( const int web_server_id, const int log_type, const int log_field_id )
{
    return this->blacklists.at( this->current_WS ).at( log_type ).at( log_field_id ).used;
}
bool Craplog::isWarnlistUsed( const int web_server_id, const int log_type, const int log_field_id )
{
    return this->warnlists.at( this->current_WS ).at( log_type ).at( log_field_id ).used;
}
const std::vector<std::string>& Craplog::getBlacklist( const int web_server_id, const int log_type, const int log_field_id )
{
    return this->blacklists.at( this->current_WS ).at( log_type ).at( log_field_id ).list;
}
const std::vector<std::string>& Craplog::getWarnlist( const int web_server_id, const int log_type, const int log_field_id )
{
    return this->warnlists.at( this->current_WS ).at( log_type ).at( log_field_id ).list;
}


void Craplog::storeLogLines()
{
    bool successful;
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc = sqlite3_open("test.db", &db);

    if ( rc ) {
        // error opening database
        this->proceed = false;
        std::string err_msg = "";
        if ( this->dialogs_Level == 2 ) {
            err_msg.append( "\n\nSQLite error message:\n" );
            err_msg.append( sqlite3_errmsg(db) );
        }
        DialogSec::errDatabaseFailedOpening( nullptr, "statistics", err_msg );
    }

    try {
        // ACID transaction
        rc = sqlite3_exec(db, "BEGIN;", nullptr, nullptr, &zErrMsg);
        if( rc != SQLITE_OK ) {
            // error opening database
            this->proceed = false;
            std::string err_msg = "";
            if ( this->dialogs_Level > 0 ) {
                err_msg.append( "\n\nStatement:\n" );
                err_msg.append( "\"BEGIN;\"" );
                if ( this->dialogs_Level == 2 ) {
                    err_msg.append( "\n\nSQLite error message:\n" );
                    err_msg.append( zErrMsg );
                }
            }
            DialogSec::errDatabaseFailedExecuting( nullptr, "statistics", err_msg );
            sqlite3_free( zErrMsg );
        }

        if ( this->proceed == true && this->data_collection.at( 1 ).size() > 0 ) {
            successful = StoreOps::storeData( db, *this, this->data_collection.at( 1 ), 1 );
            this->proceed = successful;
        }

        if ( this->proceed == true && this->data_collection.at( 2 ).size() > 0 ) {
            successful = StoreOps::storeData( db, *this, this->data_collection.at( 1 ), 2 );
            this->proceed = successful;
        }

        if ( this->proceed == true ) {
            // commit the transaction
            rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &zErrMsg);
            if( rc != SQLITE_OK ) {
                // error opening database
                this->proceed = false;
                std::string err_msg = "";
                if ( this->dialogs_Level > 0 ) {
                    err_msg.append( "\n\nStatement:\n" );
                    err_msg.append( "\"COMMIT;\"" );
                    if ( this->dialogs_Level == 2 ) {
                        err_msg.append( "\n\nSQLite error message:\n" );
                        err_msg.append( zErrMsg );
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, "statistics", err_msg );
                sqlite3_free( zErrMsg );
            }
        }

    } catch (...) {
        // wrongthing went some...
        QString msg = QMessageBox::tr("An error occured while working on the database\n\nAborting");
        DialogSec::errGeneric( nullptr, msg );
        this->proceed = false;
    }

    sqlite3_close( db );
}


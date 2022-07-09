
#include "craplog.h"

#include "utilities.h"
#include "modules/checks.h"
#include "tools/craplog/modules/store.h"

#include <filesystem>
#include <thread>

#include <iostream>


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
        this->warnlists[ i ][ 1 ].emplace( 11, BWlist{ .used=false, .list={"DELETE","HEAD","OPTIONS","PUT","PATCH"} } );
        this->warnlists[ i ][ 1 ].emplace( 12, BWlist{ .used=false, .list={} } );
        this->warnlists[ i ][ 1 ].emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists[ i ][ 1 ].emplace( 21, BWlist{ .used=false, .list={} } );
        this->blacklists[ i ][ 1 ].emplace( 20, BWlist{ .used=true,  .list={"::1"} } );
        // error
        this->warnlists[ i ][ 2 ].emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists[ i ][ 2 ].emplace( 31, BWlist{ .used=false, .list={} } );
        this->blacklists[ i ][ 2 ].emplace( 20, BWlist{ .used=true,  .list={"::1"} } );
        this->blacklists[ i ][ 2 ].emplace( 31, BWlist{ .used=false, .list={} } );
    }

    // default format strings
    this->logs_format_stings[11] = std::unordered_map<int, std::string>();
    this->logs_format_stings[11][1] = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    this->logs_format_stings[11][2] = "[%t] [%l] [pid %P] %F: %E: [client %a] %M";
    this->logs_format_stings[12] = std::unordered_map<int, std::string>();
    this->logs_format_stings[12][1] = "$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"";
    this->logs_format_stings[12][2] = "$time_iso8601 [$error_level] $pid: *$cid $error_message";
    this->logs_format_stings[13] = std::unordered_map<int, std::string>();
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
    this->logs_paths.emplace( 11, std::unordered_map<int, std::string>() );
    this->logs_paths[11].emplace( 1, "/var/log/apache2" );
    this->logs_paths[11].emplace( 2, "/var/log/apache2" );
    // nginx access/error logs location
    this->logs_paths.emplace( 12, std::unordered_map<int, std::string>() );
    this->logs_paths[12].emplace( 1, "/var/log/nginx" );
    this->logs_paths[12].emplace( 2, "/var/log/nginx" );
    // iis access/error logs location
    this->logs_paths.emplace( 13, std::unordered_map<int, std::string>() );
    this->logs_paths[13].emplace( 1, "C:\\inetpub\\logs\\LogFiles\\W3SVC" );
    this->logs_paths[13].emplace( 2, "C:\\Windows\\System32\\LogFiles\\HTTPERR" );

    // apache2 access/error log files' names
    this->logs_base_names.emplace( 11, std::unordered_map<int, LogName>() );
    this->logs_base_names[11].emplace( 1, LogName { .starts   = "access.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    this->logs_base_names[11].emplace( 2, LogName { .starts   = "error.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    // nginx access/error log files' names
    this->logs_base_names.emplace( 12, std::unordered_map<int, LogName>() );
    this->logs_base_names[12].emplace( 1, LogName { .starts   = "access.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    this->logs_base_names[12].emplace( 2, LogName { .starts   = "error.log.",
                                                    .contains = "",
                                                    .ends     = "" });
    // iis access/error log files' names
    this->logs_base_names.emplace( 13, std::unordered_map<int, LogName>() );
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
const int Craplog::getDialogLevel()
{
    return this->dialog_level;
}
void Craplog::setDialogLevel( const int new_level )
{
    this->dialog_level = new_level;
    this->hashOps.setDialogLevel( new_level );
}

const std::string& Craplog::getStatsDatabasePath()
{
    return this->db_stats_path;
}
const std::string& Craplog::getHashesDatabasePath()
{
    return this->db_hashes_path;
}

void Craplog::setStatsDatabasePath( const std::string& path )
{
    this->db_stats_path = path;
}
void Craplog::setHashesDatabasePath( const std::string& path )
{
    this->db_hashes_path = path;
}

const int Craplog::getWarningSize()
{
    return this->warning_size;
}

void Craplog::setWarningSize( const int new_size )
{
    this->warning_size = new_size;
}


// get the logs format
const FormatOps::LogsFormat& Craplog::getAccessLogsFormat( const int web_server_id )
{
    return this->logs_formats.at( web_server_id ).at( 1 );
}
const FormatOps::LogsFormat& Craplog::getErrorLogsFormat( const int web_server_id )
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

const int Craplog::getCurrentWSID()
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
const FormatOps::LogsFormat& Craplog::getCurrentALF()
{
    return this->current_ALF;
}
// get the current error logs format
const FormatOps::LogsFormat& Craplog::getCurrentELF()
{
    return this->current_ELF;
}



///////////////////
//// LOGS LIST ////
// return the size of the list
const int Craplog::getLogsListSize() {
    return this->logs_list.size();
}

// return the list. rescan if fresh is true
const std::vector<Craplog::LogFile>& Craplog::getLogsList( const bool fresh )
{
    if ( fresh == true ) {
        this->scanLogsDir();
    }
    return this->logs_list;
}


// return the path of the file matching the given name
const Craplog::LogFile& Craplog::getLogFileItem( const QString& file_name )
{
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            return item;
        }
    }
    // should be unreachable
    throw("File item not found");
}


// return the path of the file matching the given name
const std::string& Craplog::getLogFilePath( const QString& file_name )
{
    std::string path;
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            return item.path;
        }
    }
    // should be unreachable
    throw("File item not found");
}

// set a file as selected
const bool Craplog::setLogFileSelected( const QString& file_name )
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
        std::string logs_path = logs_paths.at( i+1 );
        if ( !IOutils::isDir( logs_path ) ) {
            // this directory doesn't exists
            if ( IOutils::exists( logs_path ) ) {
                DialogSec::errDirNotExists( nullptr, QString::fromStdString( logs_path ) );
            }
            continue;
        }
        // iterate over entries in the logs folder
        for ( const auto& dir_entry : std::filesystem::directory_iterator{logs_path}) {
            // get the attributes
            int size = dir_entry.file_size();
            std::string path = dir_entry.path().string();
            QString name = QString::fromStdString( dir_entry.path().filename().string() );
            // check the readability
            if ( IOutils::checkFile( path, true ) == false ) {
                if ( this->dialog_level == 2 ) {
                    DialogSec::warnEmptyFile( nullptr, name );
                }
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
                DialogSec::errGeneric( nullptr, err_msg +":\n"+ name );
            }

            if ( content.size() == 0 ) {
                if ( this->dialog_level == 2 ) {
                    DialogSec::warnEmptyFile( nullptr, name );
                }
                continue;
            }

            if ( proceed == true ) {
                LogOps::LogType log_type = this->logOps.defineFileType( name.toStdString(), content, this->logs_formats.at( this->current_WS ) );
                if ( log_type == LogOps::LogType::Failed ) {
                    // failed to get the log type, do not append
                    DialogSec::errFailedDefiningLogType( nullptr, name );
                    continue;
                }

                // match only valid files names
                if ( this->isFileNameValid( name.toStdString(), log_type ) == false ) {
                    continue;
                }

                std::string hash = this->hashOps.digestFile( path );

                auto logfile = LogFile{
                    .selected = false,
                    .used_already = this->hashOps.hasBeenUsed( hash, this->current_WS, log_type ),
                    .size = size,
                    .name = name,
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



const bool Craplog::isFileNameValid( const std::string& name, const LogOps::LogType& log_type )
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
const bool Craplog::isWorking()
{
    return this->working;
}
const bool Craplog::isParsing()
{
    return this->parsing;
}

// performances
const int Craplog::getPerfSize()
{
    return this->perf_size;
}
void Craplog::sumPerfSize( const int size )
{
    this->perf_size += size;
}
const int Craplog::getTotalSize()
{
    return this->total_size;
}
const int Craplog::getParsedSize()
{
    return this->parsed_size;
}
const int Craplog::getParsedLines()
{
    return this->parsed_lines;
}
const int Craplog::getAccessSize()
{
    return this->access_size;
}
const int Craplog::getErrorSize()
{
    return this->error_size;
}

const int Craplog::getIgnoredSize( const int log_type )
{
    if ( log_type == 1 ) {
        return this->ignored_access_size;
    } else if ( log_type == 2 ) {
        return this->ignored_error_size;
    } else {
        // wrong log_type
        throw( "Unexpected LogType: " + std::to_string(log_type) );
    }
}
void Craplog::sumIgnoredSize( const int size, const int log_type )
{
    if ( log_type == 1 ) {
        this->ignored_access_size += size;
    } else if ( log_type == 2 ) {
        this->ignored_error_size += size;
    } else {
        // wrong log_type
        throw( "Unexpected LogType: " + std::to_string(log_type) );
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
    this->parsed_size  = this->logOps.getSize();
    this->parsed_lines = this->logOps.getLines();
    this->perf_size    = this->parsed_size;

    // store the new data
    this->storeLogLines();


    // insert the hashes of the used files
    this->hashOps.insertUsedHashes( this->db_hashes_path, this->used_files_hashes, this->current_WS );

    this->stopWorking();
}



const bool Craplog::checkStuff()
{
    bool proceed = true;
    this->log_files_to_use.clear();
    for ( const LogFile& file : this->logs_list ) {

        if ( proceed == false ) { break; }

        if ( file.selected == false ) {
            // not selected, skip
            continue;
        }

        // check again the type
        if ( file.type == LogOps::LogType::Failed ) {
            bool choice = DialogSec::choiceUndefinedLogType( nullptr, file.name );
            if ( choice == false ) {
                // choosed to abort all
                proceed = false;
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
            if ( this->dialog_level == 2 ) {
                msg += "\n" + QString::fromStdString( file.hash );
            }
            int choice = DialogSec::choiceFileAlreadyUsed( nullptr, msg );
            if ( choice < 0 ) {
                // choosed to abort all
                proceed = false;
                break;
            } else if ( choice > 0 ) {
                // choosed to discard and continue
                continue;
            }/* else {
                // choosed to ignore and use the file anyway
            }*/
        }

        // check if the file respects the warning size
        if ( this->warning_size >= 0 ) {
            if ( file.size > this->warning_size ) {
                // exceeds the warning size
                QString size_str, msg = file.name;
                if ( this->dialog_level >= 1 ) {
                    std::string size_sfx=" B";
                    float size = (float)file.size;
                    if (size > 1024) {
                        size /= 1024; size_sfx = " KiB";
                        if (size > 1024) {
                            size /= 1024; size_sfx = " MiB";
                        }
                    }
                    size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                    msg += QString("\n\nSize of the file:\n%1%2").arg( size_str, size_sfx.c_str() );
                    if ( this->dialog_level == 2 ) {
                        size = (float)this->warning_size;
                        if (size > 1024) {
                            size /= 1024; size_sfx = " KiB";
                            if (size > 1024) {
                                size /= 1024; size_sfx = " MiB";
                            }
                        }
                        size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                        msg += QString("\n\nWarning size parameter:\n%1%2").arg( size_str, size_sfx.c_str() );
                    }
                }
                int choice = DialogSec::choiceFileSizeWarning( nullptr, msg );
                if ( choice < 0 ) {
                    // choosed to abort all
                    proceed = false;
                    break;
                } else if ( choice > 0 ) {
                    // choosed to discard and continue
                    continue;
                }/* else {
                    // choosed to ignore and use the file anyway
                }*/
            }
        }

        // check if the statistics' database is fune
        if ( CheckSec::checkStatsDatabase( this->db_stats_path ) == false ) {
            // checks failed, abort
            proceed = false;
            break;
        }
        if ( CheckSec::checkHashesDatabase( this->db_hashes_path ) == false ) {
            // checks failed, abort
            proceed = false;
            break;
        }

        this->log_files_to_use.push_back( file );
    }

    this->proceed = proceed;
    return proceed;
}


void Craplog::joinLogLines()
{
    std::vector<std::string> content;
    for ( const LogFile& file : this->log_files_to_use ) {

        if ( this->proceed == false ) { break; }

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
    this->log_files_to_use.clear();
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


const bool Craplog::isBlacklistUsed( const int web_server_id, const int log_type, const int log_field_id )
{
    return this->blacklists.at( this->current_WS ).at( log_type ).at( log_field_id ).used;
}
const bool Craplog::isWarnlistUsed( const int web_server_id, const int log_type, const int log_field_id )
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
    QString db_path = QString::fromStdString( this->db_stats_path );
    QString db_name = QString::fromStdString( this->db_stats_path.substr( this->db_stats_path.find_last_of( '/' ) + 1 ) );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( db_path );

    if ( db.open() == false ) {
        // error opening database
        this->proceed = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, err_msg );

    } else {

        try {
            // ACID transaction
            if ( db.transaction() == false ) {
                // error opening database
                this->proceed = false;
                QString stmt_msg="", err_msg = "";
                if ( this->dialog_level > 0 ) {
                    stmt_msg = "db.transaction()";
                    if ( this->dialog_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
            }

            if ( this->proceed == true && this->data_collection.at( 1 ).size() > 0 ) {
                successful = StoreOps::storeData( db, *this, this->data_collection.at( 1 ), 1 );
                this->proceed = successful;
            }

            if ( this->proceed == true && this->data_collection.at( 2 ).size() > 0 ) {
                successful = StoreOps::storeData( db, *this, this->data_collection.at( 2 ), 2 );
                this->proceed = successful;
            }

            if ( this->proceed == true ) {
                // commit the transaction
                if ( db.commit() == false ) {
                    // error opening database
                    this->proceed = false;
                    QString stmt_msg="", err_msg = "";
                    if ( this->dialog_level > 0 ) {
                        stmt_msg = "db.commit()";
                        if ( this->dialog_level == 2 ) {
                            err_msg= db.lastError().text();
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                }
            }
            if ( proceed == false ) {
                // rollback
                throw (std::exception());
            }

        } catch (...) {
            // wrongthing w3nt some.,.
            this->proceed = false;
            bool err_shown = false;
            // rollback the transaction
            if ( db.rollback() == false ) {
                // error rolling back commits
                QString stmt_msg="", err_msg = "";
                if ( this->dialog_level > 0 ) {
                    stmt_msg = "db.rollback()";
                    if ( this->dialog_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                err_shown = true;
            }
            if ( err_shown == false ) {
                // show a message
                QString msg = QMessageBox::tr("An error occured while working on the database\n\nAborting");
                DialogSec::errGeneric( nullptr, msg );
            }
        }

        db.close();
    }

}


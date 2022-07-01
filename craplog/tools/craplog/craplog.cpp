
#include "craplog.h"

#include "utilities.h"

#include <filesystem>

#include <iostream>
#include <thread>

using std::string, std::vector, std::unordered_map;


Craplog::Craplog()
{
    // initialize default values
    this->logs_format_stings[11] = unordered_map<int, string>();
    this->logs_format_stings[11][1] = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    this->logs_format_stings[11][2] = "[%t] [%l] [pid %P] %F: %E: [client %a] %M";
    this->logs_format_stings[12] = unordered_map<int, string>();
    this->logs_format_stings[12][1] = "$remote_addr - $remote_user [$time_local] \"$request\" $status $body_bytes_sent \"$http_referer\" \"$http_user_agent\"";
    this->logs_format_stings[12][2] = "$time_iso8601 [$error_level] $pid: *$cid $error_message";
    this->logs_format_stings[13] = unordered_map<int, string>();
    this->logs_format_stings[13][1] = "date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs-version cs(User-Agent) cs(Cookie) cs(Referer) cs-host sc-status sc-substatus sc-win32-status sc-bytes cs-bytes time-taken";
    this->logs_format_stings[13][2] = "";

    // TEMPORARY !!!
    this->logs_formats[11][1] = this->formatOps.processFormatString( this->logs_format_stings[11][1], 1, 11 );
    this->logs_formats[11][2] = this->formatOps.processFormatString( this->logs_format_stings[11][2], 2, 11 );
    this->logs_formats[12][1] = this->formatOps.processFormatString( this->logs_format_stings[12][1], 1, 12 );
    /* FINAL WILL BE:
    for ( int i=11; i<14; i++ ) {
        for ( int j=1; j<3; j++ ) {
            this->logs_formats[i][j] = this->formatOps.processFormatString( this->logs_format_stings[i][j], j, i );
        }
    }
    */

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

    /*this->readConfigs();

    this->hashOps.readLists( this->configs_path );*/

}


//////////////////
//// SETTINGS ////
int Craplog::getWarningSize()
{
    return this->warning_size;
}

void Craplog::setWarningSize( int new_size )
{
    this->warning_size = new_size;
}


// get the logs format
FormatOps::LogsFormat Craplog::getAccessLogsFormat( int web_server_id ) &
{
    return this->logs_formats[ web_server_id ][1];
}
FormatOps::LogsFormat Craplog::getErrorLogsFormat( int web_server_id ) &
{
    return this->logs_formats[ web_server_id ][2];
}

// set the logs format
void Craplog::setAccessLogsFormat( const int web_server_id, const std::string& format_string )
{
    this->logs_formats[web_server_id][1] = this->formatOps.processFormatString(
        this->logs_format_stings[web_server_id][1],
        1, web_server_id );
}
void Craplog::setErrorLogsFormat(const int web_server_id, const std::string& format_string )
{
    this->logs_formats[web_server_id][2] = this->formatOps.processFormatString(
        this->logs_format_stings[web_server_id][2],
        2, web_server_id );
}


// set the current Web Server
void Craplog::setCurrentWSID( int web_server_id )
{
    this->current_WS = web_server_id;
}

int Craplog::getCurrentWSID() const
{
    return this->current_WS;
}

// set the current access logs format
void Craplog::setCurrentALF()
{
    this->current_ALF = this->logs_formats[ this->current_WS ][1];
}
// set the current error logs format
void Craplog::setCurrentELF()
{
    this->current_ELF = this->logs_formats[ this->current_WS ][2];
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
vector<Craplog::LogFile> Craplog::getLogsList( bool fresh )
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
    auto logs_paths = this->logs_paths[ this->current_WS ];
    if ( logs_paths[1] == logs_paths[2] ) {
        // same dir for both access and error logs, loop only once
        n=1;
    }
    for ( int i=0; i<n; i++ ) {
        string logs_path = logs_paths[i+1];
        if ( !IOutils::isDir( logs_path ) ) {
            // this directory doesn't exists
            if ( IOutils::exists( logs_path ) ) {
                DialogSec::msgDirNotExists( nullptr, logs_path );
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
                DialogSec::msgFailedReadFile( nullptr, name );
            } catch (...) {
                // failed somehow
                proceed = false;
                QString err_msg = QMessageBox::tr("An error occured while reading");
                DialogSec::msgGenericError( nullptr, err_msg +":\n"+ QString::fromStdString(name) );
            }

            if ( proceed == true ) {
                LogOps::LogType log_type = this->logOps.defineFileType( name, content, this->logs_formats[ this->current_WS ] );
                if ( log_type == LogOps::LogType::Failed ) {
                    // failed to get the log type, do not append
                    DialogSec::msgFailedDefineLogType( nullptr, name );
                    continue;
                }

                // match only valid files names
                if ( this->isFileNameValid( name, log_type ) == false ) {
                    continue;
                }

                std::string hash = this->hashOps.digestFile( path );

                auto logfile = LogFile{
                    .selected = false,
                    .used_already = this->hashOps.hasBeenUsed( hash, this->current_WS ),
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
    if ( this->logs_base_names[ this->current_WS ][ log_type ].starts != "" ) {
        if ( ! StringOps::startsWith( name, this->logs_base_names[ this->current_WS ][ log_type ].starts ) ) {
            return false;
        }
    }
    if ( this->logs_base_names[ this->current_WS ][ log_type ].contains != "" ) {
        if ( ! StringOps::contains(
                    name.substr( this->logs_base_names[ this->current_WS ][ log_type ].starts.size() ),
                    this->logs_base_names[ this->current_WS ][ log_type ].contains ) ) {
            return false;
        }
    }
    if ( this->logs_base_names[ this->current_WS ][ log_type ].ends != "" ) {
        if ( ! StringOps::endsWith( name, this->logs_base_names[ this->current_WS ][ log_type ].ends ) ) {
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
                if ( StringOps::isNumeric( name[i] ) == false ) {
                    valid = false;
                    break;
                }
            }
            break;

        case 13:
            // further checks for iis
            start = this->logs_base_names[ 13 ][ log_type ].starts.size();
            stop = name.size() - this->logs_base_names[ 13 ][ log_type ].ends.size();
            // search for incremental number / date
            for ( int i=start; i<=stop; i++ ) {
                if ( StringOps::isNumeric( name[i] ) == false ) {
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
}
void Craplog::stopWorking()
{
    this->working = false;
    this->data_collection[1].clear();
    this->data_collection[2].clear();
}
bool Craplog::isWorking()
{
    return this->working;
}

// performances
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


void Craplog::run()
{
    this->startWorking();

    // collect log lines
    this->joinLogLines();

    if ( this->proceed == true ) {
        this->parseLogLines();
    }

    this->stopWorking();
}



void Craplog::joinLogLines()
{
    std::vector<std::string> content;
    for ( const LogFile& file : this->logs_list ) {

        if ( this->proceed == false ) { break; }

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
            DialogSec::msgFailedReadFile( nullptr, file.name, true );
            continue;
        } catch (...) {
            // failed somehow
            DialogSec::msgFailedReadFile( nullptr, file.name, true );
            continue;
        }

        // append to the relative list
        if ( file.type == LogOps::LogType::Access ) {
            this->access_logs_lines.insert( this->access_logs_lines.end(), content.begin(), content.end() );
        } else {
            this->error_logs_lines.insert( this->error_logs_lines.end(), content.begin(), content.end() );
        }

        this->total_size += file.size;
    }
}


void Craplog::parseLogLines()
{

}


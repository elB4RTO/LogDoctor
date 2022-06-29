
#include "craplog.h"

#include "utilities.h"

#include <filesystem>

#include <iostream>

using std::string, std::vector, std::unordered_map;


Craplog::Craplog()
{
    this->logs_format_stings[11] = unordered_map<int, string>();
    this->logs_format_stings[11][1] = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    this->logs_format_stings[11][2] = "[%t] [%l] [pid %P] %F: %E: [client %a] %M";/*
    this->logs_format_stings[12] = unordered_map<int, string>();
    this->logs_format_stings[12][1] = "$remote_addr - $remote_user [$time_local] \"$request\" $status $body_bytes_sent \"$http_referer\" \"$http_user_agent\"";
    this->logs_format_stings[12][2] = "";
    this->logs_format_stings[13] = unordered_map<int, string>();
    this->logs_format_stings[13][1] = "";
    this->logs_format_stings[13][2] = "";*/

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
    unordered_map<int, string> new_map;
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


    /*this->readConfigs();

    this->hashOps.readLists( this->configs_path );*/

}


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
Craplog::LogFile Craplog::getLogFileItem( QString file_name )
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
string Craplog::getLogFilePath( QString file_name )
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
bool Craplog::setLogFileSelected( QString file_name )
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
                Dialogs::msgDirNotExists( this, logs_path );
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
            // match only files having ".log." in their name
            if ( this->isFileNameValid( name ) == false ) {
                continue;
            }

            auto logfile = LogFile{
                .selected = false,
                .size = size,
                .name = QString::fromStdString( name ),
                .hash = this->hashOps.digestFile( path ),
                .path = path,
                .type = this->logOps.defineFileType( name, IOutils::readLines( path ), this->logs_formats[ this->current_WS ] )
            };
            if ( logfile.type == LogOps::LogType::Failed ) {
                // failed to get the log type, do not append
                // error message already displayed while defining as failed in logOps
                continue;
            }
            // push in the list
            this->logs_list.push_back( logfile );
        }
    }
}



bool Craplog::isFileNameValid( string name )
{
    bool valid = false;
    if ( StringOps::startsWith( name, "access.log." )
      || StringOps::startsWith( name, "error.log." ) ) {
        int start = name.find_last_of( ".log." )+1;
        int stop = name.find( ".gz", start);
        if ( stop >= name.size() ) {
            stop = name.size()-1;
        }
        valid = true;
        for ( int i=start; i<=stop; i++ ) {
            if ( StringOps::isNumeric( name[i] ) == false ) {
                valid = false;
                break;
            }
        }
    }
    return valid;
}


int Craplog::getWarningSize()
{
    return this->warning_size;
}

void Craplog::setWarningSize( int new_size )
{
    this->warning_size = new_size;
}


// get the logs format
FormatOps::LogsFormat Craplog::getAccessLogsFormat( int web_server_id )
{
    return this->logs_formats[ web_server_id ][1];
}
FormatOps::LogsFormat Craplog::getErrorLogsFormat( int web_server_id )
{
    return this->logs_formats[ web_server_id ][2];
}

// set the logs format
void Craplog::setAccessLogsFormat( int web_server_id, string format_string )
{

}
void Craplog::setErrorLogsFormat( int web_server_id, string format_string )
{

}


// set the current Web Server
void Craplog::setCurrentWSID( int web_server_id )
{
    this->current_WS = web_server_id;
}

int Craplog::getCurrentWSID()
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
FormatOps::LogsFormat Craplog::getCurrentALF()
{
    return this->current_ALF;
}
// get the current error logs format
FormatOps::LogsFormat Craplog::getCurrentELF()
{
    return this->current_ELF;
}



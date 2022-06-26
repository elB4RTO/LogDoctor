
#include "craplog.h"

#include "utilities.h"

#include <filesystem>

#include <iostream>


Craplog::Craplog()
{
    this->current_ALF = FormatOps::LogsFormat{
        .string = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"",
        .initial = "",
        .final   = "\"",
        .separators = {" ", " ", " ", " \"", "\" ", " ", " \"", "\" \""},
        .fields = {"client","logname","user","date_time","request","response_code","http_size","referer","user_agent"}
    };
    this->current_ELF = FormatOps::LogsFormat{
        .string = "[%t] [%l] [pid %P] %F: %E: [client %a] %M",
        .initial = "[",
        .final   = "",
        .separators = {"] [", "] [pid", "] ", ": ", ": [client ", "] "},
        .fields = {"date_time","error_level","pid","source_file","os_err","client:port","error_message"}
    };

    // apache2 access/error logs location
    std::unordered_map<int, std::string> new_map;
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


    /*this->readConfigs();

    this->hashOps.readLists( this->configs_path );*/

    this->scanLogsDir();
}


// return the size of the list
int Craplog::getLogsListSize() {
    return this->logs_list.size();
}

// return the list. rescan if fresh is true
std::vector<Craplog::LogFile> Craplog::getLogsList( bool fresh )
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
std::string Craplog::getLogFilePath( QString file_name )
{
    std::string path;
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            path = item.path;
            break;
        }
    }
    return path;
}

// set a file as selected
int Craplog::setLogFileSelected( QString file_name )
{
    int result = 1;
    for ( Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            item.selected = true;
            result = 0;
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
        std::string logs_path = logs_paths[i+1];
        // iterate over entries in the logs folder
        for ( const auto& dir_entry : std::filesystem::directory_iterator{logs_path}) {
            // get the attributes
            int size = dir_entry.file_size();
            std::string path = dir_entry.path().string();
            std::string name = dir_entry.path().filename().string();
            std::cout << logs_path << std::endl;
            std::cout << name << " : " << path << std::endl;
            std::cout << this->hashOps.digestFile( path ) << std::endl;
            // match only files having ".log." in their name
            if ( dir_entry.is_regular_file() == false
              || this->isFileNameValid( name ) == false ) {
                continue;
            }
            auto logfile =LogFile{
                .selected = false,
                .size = size,
                .name = QString::fromStdString( name ),
                .hash = this->hashOps.digestFile( path ),
                .path = path,
                .type = this->defineFileType( name, IOutils::readLines( path ) )
            };
            if ( logfile.type == Craplog::LogType::Failed ) {
                // failed to get the log type
                // error message displayed while defining as failed in logOps
                continue;
            }
            // push in the list
            this->logs_list.push_back( logfile );
        }
    }
}



bool Craplog::isFileNameValid( std::string name )
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


Craplog::LogType Craplog::defineFileType( std::string name, std::vector<std::string> lines )
{
    int n_acc=0, n_err=0;
    Craplog::LogType supposed_type, real_type;
    if ( StringOps::startsWith( name, "access" ) ) {
        supposed_type = Craplog::LogType::Access;
    } else if ( StringOps::startsWith( name, "error" ) ) {
        supposed_type = Craplog::LogType::Error;
    } else {
        // this shouldn't be
            // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    }
    real_type = Craplog::LogType::Failed;
    for ( const std::string& line : lines ) {
        if ( supposed_type == Craplog::LogType::Access ) {
            if ( this->current_ALF.initial.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::startsWith( line, this->current_ALF.initial ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            if ( this->current_ALF.final.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::endsWith( line, this->current_ALF.final ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            n_acc++;
        } else {
            if ( this->current_ELF.initial.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::startsWith( line, this->current_ELF.initial ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            if ( this->current_ELF.final.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::endsWith( line, this->current_ELF.final ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            n_err++;
        }
    }
    if ( n_acc > 0 && n_err == 0 ) {
        // access logs
        real_type = Craplog::LogType::Access;
    } else if ( n_err > 0 && n_acc == 0 ) {
        // error logs
        real_type = Craplog::LogType::Error;
    } else {
        // something is wrong with these logs, put a warnin
        if ( n_acc > 0 && n_err > 0 ) {
            // both access and error types found
                // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
        } else {
            // every line was invalid
                // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
        }
    }
    return real_type;
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
void Craplog::setAccessLogsFormat( int web_server_id, std::string format_string )
{

}
void Craplog::setErrorLogsFormat( int web_server_id, std::string format_string )
{

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



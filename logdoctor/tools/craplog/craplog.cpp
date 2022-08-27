
#include "craplog.h"

#include "modules/checks.h"
#include "modules/charts/donuts.h"
#include "modules/exceptions.h"
#include "modules/dialogs.h"

#include "utilities/gzip.h"
#include "utilities/io.h"

#include "tools/shared.h"
#include "tools/craplog/modules/store.h"

#include <QPainter>

#include <filesystem>
#include <thread>
#include <exception>
#include <ctime>


Craplog::Craplog()
{
    ////////////////////////
    //// INITIALIZATION ////
    ////////////////////////
    // blacklists / whitelists
    for ( int i=this->APACHE_ID; i<=this->IIS_ID; i++ ) {
        this->warnlists.emplace(  i, std::unordered_map<int, BWlist>() );
        this->blacklists.emplace( i, std::unordered_map<int, BWlist>() );
        // default data
        this->warnlists.at( i ).emplace( 11, BWlist{ .used=false, .list={"DELETE","HEAD","OPTIONS","PUT","PATCH"} } );
        this->warnlists.at( i ).emplace( 12, BWlist{ .used=true,  .list={"/robots.txt","/../","/./","/.env","/.htaccess","/phpmyadmin","/wp-admin","/wp-content","/wp-config.php","/config.py","/views.py","/routes.py","/stepu.cgi","/cgi-bin"} } );
        this->warnlists.at( i ).emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists.at( i ).emplace( 21, BWlist{ .used=false, .list={} } );
        this->blacklists.at( i ).emplace( 20, BWlist{ .used=true,  .list={} } );
    }

    // default format strings
    this->logs_format_strings.emplace(
        this->APACHE_ID, "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"" );
    this->logs_format_strings.emplace(
        this->NGINX_ID,  "$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"" );
    this->logs_format_strings.emplace(
        this->IIS_ID, "date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken" );

    // initialize formats
    this->logs_formats.emplace(
        this->APACHE_ID, this->formatOps.processApacheFormatString( this->logs_format_strings.at(this->APACHE_ID) ) );
    this->logs_formats.emplace(
        this->NGINX_ID,  this->formatOps.processNginxFormatString( this->logs_format_strings.at(this->NGINX_ID) ) );
    this->logs_formats.emplace(
        this->IIS_ID,    this->formatOps.processIisFormatString( this->logs_format_strings.at(this->IIS_ID), 0 ) );

    this->current_LF = this->logs_formats.at( this->APACHE_ID );

    // apache2 access/error logs location
    this->logs_paths.emplace( this->APACHE_ID, "/var/log/apache2" );
    // nginx access/error logs location
    this->logs_paths.emplace( this->NGINX_ID, "/var/log/nginx" );
    // iis access/error logs location
    this->logs_paths.emplace( this->IIS_ID, "C:/inetpub/logs/LogFiles" );

    // apache2 access/error log files' names
    this->logs_base_names.emplace( this->APACHE_ID, LogName{ .starts   = "access.log.",
                                                             .contains = "",
                                                             .ends     = "" } );
    // nginx access/error log files' names
    this->logs_base_names.emplace( this->NGINX_ID, LogName{ .starts   = "access.log.",
                                                            .contains = "",
                                                            .ends     = "" });
    // iis access/error log files' names
    this->logs_base_names.emplace( this->IIS_ID, LogName{ .starts   = "",
                                                          .contains = "_ex",
                                                          .ends     = ".log" });

}


//////////////////
//// SETTINGS ////
const int& Craplog::getDialogsLevel()
{
    return this->dialogs_level;
}
void Craplog::setDialogsLevel( const int& new_level )
{
    this->dialogs_level = new_level;
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
    this->db_stats_path = path + "/collection.db";
}
void Craplog::setHashesDatabasePath( const std::string& path )
{
    this->db_hashes_path = path + "/hashes.db";
}

const long& Craplog::getWarningSize()
{
    return this->warning_size;
}

void Craplog::setWarningSize(const long& new_size )
{
    this->warning_size = new_size;
}


////////////////////
//// WARN/BLACK ////
const bool& Craplog::isBlacklistUsed( const int& web_server_id, const int& log_field_id )
{
    return this->blacklists.at( web_server_id ).at( log_field_id ).used;
}
const bool& Craplog::isWarnlistUsed( const int& web_server_id, const int& log_field_id )
{
    return this->warnlists.at( web_server_id ).at( log_field_id ).used;
}

void Craplog::setBlacklistUsed( const int& web_server_id, const int& log_field_id, const bool& used )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).used = used;
}
void Craplog::setWarnlistUsed( const int& web_server_id, const int& log_field_id, const bool& used )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).used = used;
}

const std::vector<std::string>& Craplog::getBlacklist( const int& web_server_id, const int& log_field_id )
{
    return this->blacklists.at( web_server_id ).at( log_field_id ).list;
}
const std::vector<std::string>& Craplog::getWarnlist( const int& web_server_id, const int& log_field_id )
{
    return this->warnlists.at( web_server_id ).at( log_field_id ).list;
}

void Craplog::setBlacklist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->blacklists.at( web_server_id ).at( log_field_id ).list.push_back( item );
    }
}
void Craplog::setWarnlist( const int& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->warnlists.at( web_server_id ).at( log_field_id ).list.push_back( item );
    }
}

void Craplog::blacklistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).list.push_back( new_item );
}
void Craplog::warnlistAdd( const int& web_server_id, const int& log_field_id, const std::string& new_item )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).list.push_back( new_item );
}

void Craplog::blacklistRemove( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    // move the item to the end, then pop it
    for ( int i=0; i<list.size()-1; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1 );
            list.at( i+1 ) = item;
        }
    }
    list.pop_back();
}
void Craplog::warnlistRemove( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    // move the item to the end, then pop it
    for ( int i=0; i<list.size()-1; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1 );
            list.at( i+1 ) = item;
        }
    }
    list.pop_back();
}

int Craplog::blacklistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    int i;
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    for ( i=1; i<list.size(); i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i-1 );
            list.at( i-1 ) = item;
            i--;
            break;
        }
    }
    return i;
}
int Craplog::warnlistMoveUp( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    int i;
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    for ( i=1; i<list.size(); i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i-1 );
            list.at( i-1 ) = item;
            i--;
            break;
        }
    }
    return i;
}

int Craplog::blacklistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    int i;
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    for ( i=0; i<list.size()-1; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1 );
            list.at( i+1 ) = item;
            i++;
            break;
        }
    }
    return i;
}
int Craplog::warnlistMoveDown( const int& web_server_id, const int& log_field_id, const std::string& item )
{
    int i;
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    for ( i=0; i<list.size()-1; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1 );
            list.at( i+1 ) = item;
            i++;
            break;
        }
    }
    return i;
}


/////////////////
//// FORMATS ////
// get the logs format string
const std::string& Craplog::getLogsFormatString( const int& web_server_id )
{
    return this->logs_format_strings.at( web_server_id );
}

// get the logs format
const FormatOps::LogsFormat& Craplog::getLogsFormat(const int& web_server_id )
{
    return this->logs_formats.at( web_server_id );
}

// set the logs format
void Craplog::setApacheLogFormat( const std::string& format_string )
{
    // apache
    try {
        this->logs_formats.at( this->APACHE_ID ) =
            this->formatOps.processApacheFormatString( format_string );
        this->logs_format_strings.at( this->APACHE_ID ) = format_string;
    } catch ( LogFormatException& e ) {
        DialogSec::errInvalidLogFormatString( nullptr, e.what() );
    } catch (...) {
        DialogSec::errGeneric( nullptr, DialogSec::tr("An error occured while parsing the format string"), true );
    }
}
void Craplog::setNginxLogFormat( const std::string& format_string )
{
    // nginx
    try {
        this->logs_formats.at( this->NGINX_ID ) =
            this->formatOps.processNginxFormatString( format_string );
        this->logs_format_strings.at( this->NGINX_ID ) = format_string;
    } catch ( LogFormatException& e ) {
        DialogSec::errInvalidLogFormatString( nullptr, e.what() );
    } catch (...) {
        DialogSec::errGeneric( nullptr, DialogSec::tr("An error occured while parsing the format string"), true );
    }
}
void Craplog::setIisLogFormat( const std::string& format_string, const int& log_module )
{
    // iis
    try {
        this->logs_formats.at( this->IIS_ID ) =
            this->formatOps.processIisFormatString( format_string, log_module );
        this->logs_format_strings.at( this->IIS_ID ) = format_string;
        this->changeIisLogsBaseNames( log_module );
    } catch ( LogFormatException& e ) {
        DialogSec::errInvalidLogFormatString( nullptr, e.what() );
    } catch (...) {
        DialogSec::errGeneric( nullptr, DialogSec::tr("An error occured while parsing the format string"), true );
    }
}

const QString Craplog::getLogsFormatSample( const int& web_server_id )
{
    QString sample;
    if ( web_server_id == this->APACHE_ID ) {
        sample = this->formatOps.getApacheLogSample( this->logs_formats.at( web_server_id ) );
    } else if ( web_server_id == this->NGINX_ID ) {
        sample = this->formatOps.getNginxLogSample( this->logs_formats.at( web_server_id ) );
    } else if ( web_server_id == this->IIS_ID ) {
        sample = this->formatOps.getIisLogSample( this->logs_formats.at( web_server_id ) );
    } else {
        // unexpected WebServer
        throw WebServerException( "Unexpected WebServerID: " + std::to_string( web_server_id ) );
    }
    return sample;
}


// set the current Web Server
void Craplog::setCurrentWSID( const int& web_server_id )
{
    this->current_WS = web_server_id;
    this->setCurrentLogFormat();
}

const int& Craplog::getCurrentWSID()
{
    return this->current_WS;
}

// set the current access logs format
void Craplog::setCurrentLogFormat()
{
    this->current_LF = this->logs_formats.at( this->current_WS );
}

// get the current access logs format
const FormatOps::LogsFormat& Craplog::getCurrentLogFormat()
{
    return this->current_LF;
}


///////////////////
//// LOGS PATH ////
const std::string& Craplog::getLogsPath( const int& web_server )
{
    return this->logs_paths.at( web_server );
}
void Craplog::setLogsPath( const int& web_server, const std::string& new_path )
{
    this->logs_paths.at( web_server ) = new_path;
}


///////////////////
//// LOGS LIST ////
// return the size of the list
const int Craplog::getLogsListSize() {
    return this->logs_list.size();
}

// return the list. rescan if fresh is true
const std::vector<Craplog::LogFile>& Craplog::getLogsList( const bool& fresh )
{
    if ( fresh ) {
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
    throw GenericException("File item not found");
}


// return the path of the file matching the given name
/*const std::string& Craplog::getLogFilePath( const QString& file_name )
{
    for ( const Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            return item.path;
        }
    }
    // should be unreachable
    throw GenericException("File item not found");
}*/

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
    bool successful = true;
    this->logs_list.clear();
    std::string &logs_path = this->logs_paths.at( this->current_WS );
    if ( ! IOutils::isDir( logs_path ) ) {
        // this directory doesn't exists
        if ( IOutils::exists( logs_path ) ) {
            DialogSec::errDirNotExists( nullptr, QString::fromStdString( logs_path ) );
        }
        successful = false;
    }
    if ( successful ) {
        int size;
        QString name;
        std::string path;
        // iterate over entries in the logs folder
        for ( const auto& dir_entry : std::filesystem::directory_iterator{logs_path}) {
            // get the attributes
            path = dir_entry.path().string();
            name = QString::fromStdString( dir_entry.path().filename().string() );
            // check if it is actually a file
            if ( IOutils::checkFile( path ) ) {
                // it's a file, check the readability
                if ( ! IOutils::checkFile( path, true ) ) {
                    // not readable, skip
                    if ( this->dialogs_level == 2 ) {
                        DialogSec::warnFileNotReadable( nullptr, name );
                    }
                    continue;
                }
                // it's readable, get the size
                size = dir_entry.file_size();
            } else {
                continue;
            }

            std::vector<std::string> content;
            try {
                // read 32 random lines
                IOutils::randomLines( path, content, 32 );

            } catch (GenericException& e) {
                // failed closing gzip file pointer
                DialogSec::errGeneric( nullptr, e.what() );
                continue;
            }

            if ( content.size() == 0 ) {
                if ( this->dialogs_level == 2 ) {
                    DialogSec::warnEmptyFile( nullptr, name );
                }
                continue;
            }

            LogOps::LogType log_type = this->logOps.defineFileType(
                name.toStdString(), content, this->logs_formats.at( this->current_WS ) );
            content.clear();
            if ( log_type == LogOps::LogType::Failed ) {
                // failed to get the log type, do not append
                DialogSec::errFailedDefiningLogType( nullptr, name );
                continue;
            } else if ( log_type == LogOps::LogType::Discarded ) {
                // skip
                continue;
            }

            // match only valid files names
            if ( ! this->isFileNameValid( name.toStdString() ) ) {
                continue;
            }

            std::string hash;
            try {
                hash = this->hashOps.digestFile( path );
            } catch (GenericException& e) {
                // failed to digest
                DialogSec::errGeneric( nullptr, e.what() );
                continue;
            }

            LogFile logfile = {
                .selected = false,
                .used_already = this->hashOps.hasBeenUsed( hash, this->current_WS ),
                .size = size,
                .name = name,
                .hash = hash,
                .path = path
            };
            // push in the list
            this->logs_list.push_back( logfile );
        }
    }
}


void Craplog::changeIisLogsBaseNames( const int& module_id )
{
    switch ( module_id ) {
        case 0: // W3C
            this->logs_base_names.at( 13 ).contains = "_ex"; break;
        case 1: // NCSA
            this->logs_base_names.at( 13 ).contains = "_nc"; break;
        case 2: // IIS
            this->logs_base_names.at( 13 ).contains = "_in"; break;

        default: // shouldn't be reachable
            throw GenericException( "Unexpected LogFormatModule ID: "+std::to_string( module_id ), true ); // leave un-catched
    }
}
const bool Craplog::isFileNameValid( const std::string& name )
{
    bool valid = true;
    if ( this->logs_base_names.at( this->current_WS ).starts != "" ) {
        if ( ! StringOps::startsWith( name, this->logs_base_names.at( this->current_WS ).starts ) ) {
            return false;
        }
    }
    if ( this->logs_base_names.at( this->current_WS ).contains != "" ) {
        if ( ! StringOps::contains(
                                    name.substr( this->logs_base_names.at( this->current_WS ).starts.size() ),
                                    this->logs_base_names.at( this->current_WS ).contains ) ) {
            return false;
        }
    }
    if ( this->logs_base_names.at( this->current_WS ).ends != "" ) {
        if ( ! StringOps::endsWith( name, this->logs_base_names.at( this->current_WS ).ends )
          && ! StringOps::endsWith( name, ".gz" ) ) {
            return false;
        }
    }

    switch ( this->current_WS ) {
        size_t start, stop;
        case 11 | 12:
            // further checks for apache / nginx
            start = StringOps::findLast( name, ".log." )+5;
            if ( start == std::string::npos ) {
                valid = false;
                break;
            }
            stop = name.size()-1;
            if ( StringOps::endsWith( name, ".gz" ) ) {
                stop -= 3;
            }
            // serach for incremental numbers
            for ( int i=start; i<=stop; i++ ) {
                if ( ! StringOps::isNumeric( name.at( i ) ) ) {
                    valid = false;
                    break;
                }
            }
            break;

        case 13:
            // further checks for iis
            start =  name.find( this->logs_base_names.at( 13 ).contains ) + 3;
            if ( start == std::string::npos ) {
                valid = false;
                break;
            }
            stop = name.size()-5; // removing the finel '.log' extension
            if ( StringOps::endsWith( name, ".gz" ) ) {
                stop -= 3;
            }
            // search for date
            std::string date;
            for ( int i=start; i<=stop; i++ ) {
                if ( ! StringOps::isNumeric( name.at( i ) ) ) {
                    valid = false;
                    break;
                }
                date.push_back( name.at( i ) );
            }
            if ( valid ) {
                // check if the file has today's date
                time_t t;
                struct tm *tmp;
                char aux_date[7];
                time( &t );
                tmp = localtime( &t );
                // using strftime to display time
                strftime( aux_date, 7, "%y%m%d", tmp );
                valid = false;
                for ( int i=0; i<6; i++ ) {
                    if ( date.at(i) != aux_date[i] ) {
                        // different date, valid
                        valid = true;
                        break;
                    }
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

    this->perf_size = 0;
    this->total_size = 0;
    this->parsed_size = 0;
    this->warnlisted_size = 0;
    this->blacklisted_size = 0;
    this->total_lines = 0;
    this->parsed_lines = 0;

    this->data_collection.clear();
    this->logs_lines.clear();
    this->used_files_hashes.clear();
}
void Craplog::stopWorking()
{
    this->working = false;
    this->parsing = false;
}
const bool& Craplog::isWorking()
{
    return this->working;
}
const bool& Craplog::isParsing()
{
    return this->parsing;
}
const bool& Craplog::editedDatabase()
{
    return this->db_edited;
}

// performances
const unsigned& Craplog::getPerfSize()
{
    return this->perf_size;
}
void Craplog::sumPerfSize( const unsigned& size )
{
    this->perf_size   += size;
    this->parsed_size += size;
}
const unsigned& Craplog::getTotalSize()
{
    return this->total_size;
}
const unsigned& Craplog::getParsedSize()
{
    return this->parsed_size;
}
const unsigned& Craplog::getParsedLines()
{
    return this->parsed_lines;
}

void Craplog::sumWarningsSize( const unsigned& size )
{
    this->warnlisted_size += size;
}
void Craplog::sumBlacklistededSize( const unsigned& size )
{
    this->blacklisted_size += size;
}

void Craplog::collectPerfData()
{
    this->parsed_size  = this->logOps.getSize();
    this->parsed_lines = this->logOps.getLines();
    this->perf_size    = this->parsed_size;
}

void Craplog::clearDataCollection()
{
    this->data_collection.clear();
}


void Craplog::run()
{
    this->startWorking();
    try {
        if ( this->proceed ) {
            // collect log lines
            this->joinLogLines();
        }
        if ( this->proceed ) {
            // parse the log lines to fill the collection
            this->parseLogLines();
            // finished parsing logs
            this->parsing = false;
            this->parsed_size  = this->logOps.getSize();
            this->parsed_lines = this->logOps.getLines();
            this->perf_size    = this->parsed_size;
        }
        // clear log lines data
        this->logs_lines.clear();

        if ( this->proceed ) {
            // store the new data
            this->storeLogLines();
        }

        if ( this->proceed ) {
            // succesfully updated the database
            if ( this->parsed_size > 0 ) {
                this->db_edited = true;
            }
            // insert the hashes of the used files
            this->hashOps.insertUsedHashes( this->db_hashes_path, this->used_files_hashes, this->current_WS );
        }
        this->used_files_hashes.clear();

    // only catch generic, leave others un-catched
    } catch (GenericException& e) {
        DialogSec::errGeneric( nullptr, e.what() );
        this->proceed = false;;
    }

    this->stopWorking();
}



const bool Craplog::checkStuff()
{
    this->proceed = true;
    this->log_files_to_use.clear();
    for ( const LogFile& file : this->logs_list ) {

        if ( ! this->proceed ) { break; }

        if ( ! file.selected ) {
            // not selected, skip
            continue;
        }

        // check if the file has been used already
        if ( file.used_already ) {
            // already used
            QString msg = file.name;
            if ( this->dialogs_level == 2 ) {
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

        // check if the file respects the warning size
        if ( this->warning_size >= 0 ) {
            if ( file.size > this->warning_size ) {
                // exceeds the warning size
                QString size_str, msg = file.name;
                if ( this->dialogs_level >= 1 ) {
                    std::string size_sfx=" B";
                    float size = (float)file.size;
                    if (size > 1024) {
                        size /= 1024; size_sfx = " KiB";
                        if (size > 1024) {
                            size /= 1024; size_sfx = " MiB";
                        }
                    }
                    size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                    msg += QString("\n\n%1:\n%2%3").arg( DialogSec::tr("Size of the file"), size_str, size_sfx.c_str() );
                    if ( this->dialogs_level == 2 ) {
                        size = (float)this->warning_size;
                        if (size > 1024) {
                            size /= 1024; size_sfx = " KiB";
                            if (size > 1024) {
                                size /= 1024; size_sfx = " MiB";
                            }
                        }
                        size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                        msg += QString("\n\n%1:\n%2%3").arg( DialogSec::tr("Warning size parameter"), size_str, size_sfx.c_str() );
                    }
                }
                int choice = DialogSec::choiceFileSizeWarning( nullptr, msg );
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
        }

        // check if the statistics' database is fune
        if ( ! CheckSec::checkStatsDatabase( this->db_stats_path ) ) {
            // checks failed, abort
            this->proceed = false;
            break;
        }
        if ( ! CheckSec::checkHashesDatabase( this->db_hashes_path ) ) {
            // checks failed, abort
            this->proceed = false;
            break;
        }

        this->log_files_to_use.push_back( file );
    }

    return this->proceed;
}


void Craplog::joinLogLines()
{
    std::string aux;
    std::vector<std::string> content;
    for ( const LogFile& file : this->log_files_to_use ) {

        if ( ! this->proceed ) { break; }

        // collect lines
        try {
            // try reading
            content.clear();
            aux = "";
            try {
                // try as gzip compressed archive first
                GZutils::readFile( file.path, aux );

            } catch (const GenericException& e) {
                // failed closing file pointer
                throw e;

            } catch (...) {
                // fallback on reading as normal file
                if ( aux.size() > 0 ) {
                    aux = "";
                }
                IOutils::readFile( file.path, aux );
            }
            StringOps::splitrip( content, aux );
            if ( this->current_WS == this->IIS_ID ) {
                this->logOps.cleanLines( content );
            }

        // re-catched in run()
        } catch (const GenericException) {
            // failed closing gzip file pointer
            throw GenericException( QString("%1:\n%2").arg(
                DialogSec::tr("An error accured while reading the gzipped file"),
                QString::fromStdString( file.path )
                ).toStdString() );

        } catch (const std::ios_base::failure& err) {
            // failed reading as text
            throw GenericException( QString("%1:\n%2").arg(
                DialogSec::tr("An error accured while reading the file"),
                QString::fromStdString( file.path )
                ).toStdString() );

        } catch (...) {
            // failed somehow
            throw GenericException( QString("%1:\n%2").arg(
                DialogSec::tr("Something failed while handling the file"),
                QString::fromStdString( file.path )
                ).toStdString() );
        }

        // append to the relative list
        this->logs_lines.insert( this->logs_lines.end(), content.begin(), content.end() );
        this->used_files_hashes.push_back( file.hash );
        this->total_size  += file.size;
        this->total_lines += content.size();
    }
    aux.clear();
    content.clear();
    this->log_files_to_use.clear();
}


void Craplog::parseLogLines()
{
    if ( this-> proceed && this->logs_lines.size() > 0 ) {
        this->logOps.parseLines(
            this->data_collection,
            this->logs_lines,
            this->logs_formats.at( this->current_WS ) );
    }
}



void Craplog::storeLogLines()
{
    QString db_path = QString::fromStdString( this->db_stats_path );
    QString db_name = QString::fromStdString( this->db_stats_path.substr( this->db_stats_path.find_last_of( '/' ) + 1 ) );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( db_path );

    if ( ! db.open() ) {
        // error opening database
        this->proceed = false;
        QString err_msg = "";
        if ( this->dialogs_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, err_msg );

    } else {

        bool successful;
        try {
            // ACID transaction
            if ( ! db.transaction() ) {
                // error opening database
                this->proceed = false;
                QString stmt_msg="", err_msg = "";
                if ( this->dialogs_level > 0 ) {
                    stmt_msg = "db.transaction()";
                    if ( this->dialogs_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
            }

            if ( this->proceed && this->data_collection.size() > 0 ) {
                successful = StoreOps::storeData( db, *this, this->data_collection );
                this->proceed = successful;
            }

            if ( this->proceed ) {
                // commit the transaction
                if ( ! db.commit() ) {
                    // error opening database
                    this->proceed = false;
                    QString stmt_msg="", err_msg = "";
                    if ( this->dialogs_level > 0 ) {
                        stmt_msg = "db.commit()";
                        if ( this->dialogs_level == 2 ) {
                            err_msg= db.lastError().text();
                        }
                    }
                    DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                }
            }
            if ( ! proceed ) {
                // rollback
                throw (std::exception());
            }

        } catch (...) {
            // wrongthing w3nt some.,.
            this->proceed = false;
            bool err_shown = false;
            // rollback the transaction
            if ( ! db.rollback() ) {
                // error rolling back commits
                QString stmt_msg="", err_msg = "";
                if ( this->dialogs_level > 0 ) {
                    stmt_msg = "db.rollback()";
                    if ( this->dialogs_level == 2 ) {
                        err_msg = db.lastError().text();
                    }
                }
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, stmt_msg, err_msg );
                err_shown = true;
            }
            if ( ! err_shown ) {
                // show a message
                DialogSec::errGeneric( nullptr, QString("%1\n\n%2").arg(
                    DialogSec::tr("An error occured while working on the database"),
                    DialogSec::tr( f_ABORTING.c_str() ) ) );
            }
        }

        db.close();
    }

}


const QString Craplog::printableSize( const unsigned& bytes )
{
    std::string size_str, size_sfx=" B";
    float size = (float)bytes;
    if (size > 1024) {
        size /= 1024;
        size_sfx = " KiB";
        if (size > 1024) {
            size /= 1024;
            size_sfx = " MiB";
        }
    }
    // cut decimals depending on how big the floor is
    size_str = std::to_string( size );
    size_t cut_index = size_str.find('.');
    if ( cut_index == std::string::npos ) {
        cut_index = size_str.find(',');
        if ( cut_index == std::string::npos ) {
            cut_index = 0;
        }
    }
    if ( cut_index != 0 ) {
        cut_index ++;
    }
    short n_decimals = 3;
    if ( size >= 100 ) {
        n_decimals = 2;
        if ( size >= 1000 ) {
            n_decimals = 1;
            if ( size >= 10000 ) {
                n_decimals = 0;
                cut_index --;
            }
        }
    }
    if ( cut_index > 0 ) {
        cut_index += n_decimals;
        if ( cut_index > size_str.size()-1 ) {
            cut_index = size_str.size()-1;
        }
    }
    return QString::fromStdString( size_str.substr(0, cut_index ) + size_sfx );
}


/*const std::vector<int> Craplog::calcDayTraffic()
{
    std::vector<int> traffic = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if ( this->data_collection.size() > 0 ) {
        for ( const auto& data : this->data_collection ) {
            if ( data.find( 4 ) != data.end() ) {
                try {
                    traffic.at( std::stoi(data.at(4)) ) ++;
                } catch (...) {
                    continue;
                }
            }
        }
    }
    return traffic;
}*/

void Craplog::makeCharts( const QChart::ChartTheme& theme, const std::unordered_map<std::string, QFont>& fonts, QChartView* size_chart )
{
    const QString
        size_chart_name        = TR::tr("Logs Size Breakdown"),
        ignored_slice_name     = TR::tr("Ignored"),
        parsed_slice_name      = TR::tr("Parsed"),
        warning_slice_name     = TR::tr("Warnings"),
        blacklisted_slice_name = TR::tr("Blacklisted");/*,
        traffic_chart_name     = TR::tr("Time of Day Logs Traffic Ensemble"),
        access_bar_name        = TR::tr("Access Logs");*/

    // logs size donut chart
    QPieSeries *parsedSize_donut = new QPieSeries();
    parsedSize_donut->setName( this->printableSize( this->parsed_size ) );
    parsedSize_donut->append(
        parsed_slice_name + "@" + parsed_slice_name + "@" + this->printableSize( this->parsed_size-this->warnlisted_size ),
        this->parsed_size-this->warnlisted_size );
    parsedSize_donut->append(
        warning_slice_name + "@" + warning_slice_name + "@" + this->printableSize( this->warnlisted_size ),
        this->warnlisted_size );
    parsedSize_donut->append(
        blacklisted_slice_name + "@" + blacklisted_slice_name + "@" + this->printableSize( this->blacklisted_size ),
        this->blacklisted_size );

    // logs size donut chart
    QPieSeries *ignoredSize_donut = new QPieSeries();
    ignoredSize_donut->setName( this->printableSize( this->total_size-this->parsed_size-this->blacklisted_size ) );
    ignoredSize_donut->append(
        ignored_slice_name + "@#" + ignored_slice_name + "@#" + this->printableSize( this->total_size-this->parsed_size-this->blacklisted_size ),
        this->total_size-this->parsed_size-this->blacklisted_size );
    ignoredSize_donut->setLabelsVisible( false );

    DonutBreakdown *sizeBreakdown = new DonutBreakdown();
    sizeBreakdown->setTheme( theme );
    sizeBreakdown->setAnimationOptions( QChart::AllAnimations );
    sizeBreakdown->setTitle( size_chart_name );
    sizeBreakdown->setTitleFont( fonts.at("main") );
    if ( this->proceed && this->total_size > 0 ) {
        sizeBreakdown->legend()->setAlignment( Qt::AlignRight );
        sizeBreakdown->addBreakdownSeries( parsedSize_donut, Qt::GlobalColor::darkCyan, fonts.at("main_small") );
        sizeBreakdown->addBreakdownSeries( ignoredSize_donut, Qt::GlobalColor::gray, fonts.at("main_small") );
    } else {
        /*sizeBreakdown->addBreakdownSeries( parsedSize_donut, Qt::GlobalColor::white, fonts.at("main_small") );
        parsedSize_donut->setVisible( false );
        sizeBreakdown->addBreakdownSeries( ignoredSize_donut, Qt::GlobalColor::white, fonts.at("main_small") );
        ignoredSize_donut->setVisible( false );*/
        sizeBreakdown->legend()->setVisible( false );
        sizeBreakdown->setTitle("");
    }
    sizeBreakdown->legend()->setFont( fonts.at("main") );

    size_chart->setChart( sizeBreakdown );
    size_chart->setRenderHint( QPainter::Antialiasing );


    // logs traffic bars chart
    /*QColor col;
    QBarSet *access_bars = new QBarSet( access_bar_name );
    col = Qt::GlobalColor::darkCyan;
    access_bars->setColor( col.lighter( 130 ) );

    int max_traf = 0;
    for ( const int& tfc : this->calcDayTraffic() ) {
        *access_bars << tfc;
        if ( tfc > max_traf ) {
            max_traf = tfc;
        }
    }

    QBarSeries *bars = new QBarSeries();
    bars->append( access_bars );

    QChart *t_chart = new QChart();
    t_chart->setTheme( theme );
    t_chart->addSeries( bars );
    t_chart->setTitle( traffic_chart_name );
    t_chart->setTitleFont( fonts.at("main") );
    t_chart->legend()->setFont( fonts.at("main_small") );
    t_chart->setAnimationOptions( QChart::SeriesAnimations );
    //t_chart->setBackgroundBrush( Qt::darkGray );

    QStringList categories;
    categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
               << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append( categories );
    axisX->setLabelsFont( fonts.at( "main_small" ) );
    t_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat( "%d" );
    axisY->setRange( 0, max_traf );
    axisY->setLabelsFont( fonts.at( "main_small" ) );
    t_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY) ;

    t_chart->legend()->setVisible( true );
    t_chart->legend()->setAlignment( Qt::AlignBottom );

    traf_chart->setChart( t_chart );
    traf_chart->setRenderHint( QPainter::Antialiasing );*/

}



#include "craplog.h"

#include "utilities/checks.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"
#include "modules/shared.h"

#include "modules/craplog/modules/donuts.h"
#include "modules/craplog/modules/logs.h"
#include "modules/craplog/modules/worker.h"

#include <QUrl>
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
    for ( unsigned i{this->APACHE_ID}; i<=this->IIS_ID; i++ ) {
        this->warnlists.emplace(  i, std::unordered_map<int, BWlist>{} );
        this->blacklists.emplace( i, std::unordered_map<int, BWlist>{} );
        // default data
        this->warnlists.at( i ).emplace( 11, BWlist{ .used=false, .list={"DELETE","HEAD","OPTIONS","PUT","PATCH"} } );
        this->warnlists.at( i ).emplace( 12, BWlist{ .used=true,  .list={"/robots.txt","/../","/./","/.env","/.htaccess","/phpmyadmin","/wp-admin","/wp-content","/wp-config.php","/config.py","/views.py","/routes.py","/setup.cgi","/cgi-bin"} } );
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
const int& Craplog::getDialogsLevel() const
{
    return this->dialogs_level;
}
void Craplog::setDialogsLevel( const int& new_level )
{
    this->dialogs_level = new_level;
    this->hashOps.setDialogLevel( new_level );
}

const std::string& Craplog::getStatsDatabasePath() const
{
    return this->db_stats_path;
}
const std::string& Craplog::getHashesDatabasePath() const
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

const size_t Craplog::getWarningSize() const
{
    return this->warning_size;
}

void Craplog::setWarningSize(const size_t new_size )
{
    this->warning_size = new_size;
}


////////////////////
//// WARN/BLACK ////
const bool& Craplog::isBlacklistUsed( const unsigned& web_server_id, const int& log_field_id ) const
{
    return this->blacklists.at( web_server_id ).at( log_field_id ).used;
}
const bool& Craplog::isWarnlistUsed( const unsigned& web_server_id, const int& log_field_id ) const
{
    return this->warnlists.at( web_server_id ).at( log_field_id ).used;
}

void Craplog::setBlacklistUsed( const unsigned& web_server_id, const int& log_field_id, const bool& used )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).used = used;
}
void Craplog::setWarnlistUsed( const unsigned& web_server_id, const int& log_field_id, const bool& used )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).used = used;
}

const std::vector<std::string>& Craplog::getBlacklist( const unsigned& web_server_id, const int& log_field_id ) const
{
    return this->blacklists.at( web_server_id ).at( log_field_id ).list;
}
const std::vector<std::string>& Craplog::getWarnlist( const unsigned& web_server_id, const int& log_field_id ) const
{
    return this->warnlists.at( web_server_id ).at( log_field_id ).list;
}

void Craplog::setBlacklist( const unsigned& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->blacklistAdd( web_server_id, log_field_id, item );
    }
}
void Craplog::setWarnlist( const unsigned& web_server_id, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->warnlistAdd( web_server_id, log_field_id, item );
    }
}

void Craplog::blacklistAdd( const unsigned& web_server_id, const int& log_field_id, const std::string& new_item )
{
    this->blacklists.at( web_server_id ).at( log_field_id ).list.push_back(
        this->sanitizeBWitem( log_field_id, new_item ) );
}
void Craplog::warnlistAdd( const unsigned& web_server_id, const int& log_field_id, const std::string& new_item )
{
    this->warnlists.at( web_server_id ).at( log_field_id ).list.push_back(
        this->sanitizeBWitem( log_field_id, new_item ) );
}

void Craplog::blacklistRemove( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    // move the item to the end, then pop it
    const size_t max{ list.size()-1ul };
    for ( size_t i{0}; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1ul );
            list.at( i+1ul ) = item;
        }
    }
    list.pop_back();
}
void Craplog::warnlistRemove( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    // move the item to the end, then pop it
    const size_t max{ list.size()-1ul };
    for ( size_t i{0}; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1ul );
            list.at( i+1ul ) = item;
        }
    }
    list.pop_back();
}

const int Craplog::blacklistMoveUp( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    size_t i{ 1 };
    const size_t max{ list.size() };
    for ( ; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i-1ul );
            list.at( i-1ul ) = item;
            i--;
            break;
        }
    }
    return static_cast<int>( i );
}
const int Craplog::warnlistMoveUp( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    size_t i{ 1 };
    const size_t max{ list.size() };
    for ( ; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i-1ul );
            list.at( i-1ul ) = item;
            i--;
            break;
        }
    }
    return static_cast<int>( i );
}

const int Craplog::blacklistMoveDown( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->blacklists.at( web_server_id ).at( log_field_id ).list;
    size_t i{ 0 };
    const size_t max{ list.size()-1ul };
    for ( ; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1ul );
            list.at( i+1ul ) = item;
            i++;
            break;
        }
    }
    return static_cast<int>( i );
}
const int Craplog::warnlistMoveDown( const unsigned& web_server_id, const int& log_field_id, const std::string& item )
{
    auto& list = this->warnlists.at( web_server_id ).at( log_field_id ).list;
    size_t i{ 0 };
    const size_t max{ list.size()-1ul };
    for ( ; i<max; i++ ) {
        if ( list.at( i ) == item ) {
            list.at( i ) = list.at( i+1ul );
            list.at( i+1ul ) = item;
            i++;
            break;
        }
    }
    return static_cast<int>( i );
}

const std::string Craplog::sanitizeBWitem( const int& log_field_id, const std::string& new_item ) const
{
    std::string sanitized_item;
    switch ( log_field_id ) {
        case 11:
            sanitized_item = StringOps::strip( new_item );
            if ( ! StringOps::isAlphabetic( sanitized_item ) ) {
                // only letters allowed
                throw BWlistException("Invalid Method");
            }
            sanitized_item = StringOps::toUpper( new_item );
            break;
        case 12:
            sanitized_item = StringOps::lstrip( new_item );
            if ( sanitized_item.empty() ) {
                throw BWlistException("Invalid URI");
            }
            sanitized_item = QUrl::toPercentEncoding(
                QString::fromStdString( sanitized_item ),
                "/#&?=+").toStdString();
            break;
        case 20:
            sanitized_item = StringOps::strip( new_item );
            if ( ! StringOps::isIP( sanitized_item ) ) {
                // only IPv4/IPv6 allowed
                throw BWlistException("Invalid Client");
            }
            break;
        case 21:
            sanitized_item = StringOps::replace( new_item, "\"", "\\\"" );
            break;
        default:
            // shouldn't be here
            throw GenericException("Unexpected LogField ID: "+std::to_string(log_field_id));
            break;
    }
    return sanitized_item;
}


/////////////////
//// FORMATS ////
// get the logs format string
const std::string& Craplog::getLogsFormatString( const unsigned& web_server_id ) const
{
    return this->logs_format_strings.at( web_server_id );
}

// get the logs format
const LogsFormat& Craplog::getLogsFormat(const unsigned& web_server_id ) const
{
    return this->logs_formats.at( web_server_id );
}

// set the logs format
const bool Craplog::setApacheLogFormat( const std::string& format_string )
{
    // apache
    bool success{ true };
    try {
        this->logs_formats.at( this->APACHE_ID ) =
            this->formatOps.processApacheFormatString( format_string );
        this->logs_format_strings.at( this->APACHE_ID ) = format_string;
    } catch ( LogFormatException& e ) {
        success &= false;
        DialogSec::errInvalidLogFormatString( e.what() );
    } catch (...) {
        success &= false;
        DialogSec::errGeneric( DialogSec::tr("An error occured while parsing the format string"), true );
    }
    return success;
}
const bool Craplog::setNginxLogFormat( const std::string& format_string )
{
    // nginx
    bool success{ true };
    try {
        this->logs_formats.at( this->NGINX_ID ) =
            this->formatOps.processNginxFormatString( format_string );
        this->logs_format_strings.at( this->NGINX_ID ) = format_string;
    } catch ( LogFormatException& e ) {
        success &= false;
        DialogSec::errInvalidLogFormatString( e.what() );
    } catch (...) {
        success &= false;
        DialogSec::errGeneric( DialogSec::tr("An error occured while parsing the format string"), true );
    }
    return success;
}
const bool Craplog::setIisLogFormat( const std::string& format_string, const int& log_module )
{
    // iis
    bool success{ true };
    try {
        this->logs_formats.at( this->IIS_ID ) =
            this->formatOps.processIisFormatString( format_string, log_module );
        this->logs_format_strings.at( this->IIS_ID ) = format_string;
        this->changeIisLogsBaseNames( log_module );
    } catch ( LogFormatException& e ) {
        success &= false;
        DialogSec::errInvalidLogFormatString( e.what() );
    } catch (...) {
        success &= false;
        DialogSec::errGeneric( DialogSec::tr("An error occured while parsing the format string"), true );
    }
    return success;
}

const QString Craplog::getLogsFormatSample( const unsigned& web_server_id ) const
{
    if ( web_server_id == this->APACHE_ID ) {
        return this->formatOps.getApacheLogSample( this->logs_formats.at( web_server_id ) );
    } else if ( web_server_id == this->NGINX_ID ) {
        return this->formatOps.getNginxLogSample( this->logs_formats.at( web_server_id ) );
    } else if ( web_server_id == this->IIS_ID ) {
        return this->formatOps.getIisLogSample( this->logs_formats.at( web_server_id ) );
    } else {
        // unexpected WebServer
        throw WebServerException( "Unexpected WebServerID: " + std::to_string( web_server_id ) );
    }
}


// set the current Web Server
void Craplog::setCurrentWSID( const unsigned& web_server_id )
{
    this->current_WS = web_server_id;
    this->setCurrentLogFormat();
}

const unsigned& Craplog::getCurrentWSID() const
{
    return this->current_WS;
}

// set the current access logs format
void Craplog::setCurrentLogFormat()
{
    this->current_LF = this->logs_formats.at( this->current_WS );
}

// get the current access logs format
const LogsFormat& Craplog::getCurrentLogFormat() const
{
    return this->current_LF;
}


///////////////////
//// LOGS PATH ////
const std::string& Craplog::getLogsPath( const unsigned& web_server ) const
{
    return this->logs_paths.at( web_server );
}
void Craplog::setLogsPath( const unsigned& web_server, const std::string& new_path )
{
    this->logs_paths.at( web_server ) = new_path;
}


///////////////////
//// LOGS LIST ////
// return the size of the list
const int Craplog::getLogsListSize() const {
    return this->logs_list.size();
}

// return the list. rescan if fresh is true
const std::vector<LogFile>& Craplog::getLogsList( const bool fresh )
{
    if ( fresh ) {
        this->scanLogsDir();
    }
    return this->logs_list;
}


// return the LogFile instance of the file matching the given name
const LogFile& Craplog::getLogFileItem( const QString& file_name ) const
{
    const auto& item{ std::find_if
        ( this->logs_list.begin(), this->logs_list.end(),
          [&file_name](const LogFile& it){ return it.name()==file_name; } ) };
    if ( item != this->logs_list.end() ) return *item;
    // should be unreachable
    throw GenericException("File item not found");
}


// set a file as selected
const bool Craplog::setLogFileSelected( const QString& file_name )
{
    const auto item{ std::find_if
        ( this->logs_list.begin(), this->logs_list.end(),
          [&file_name]( const LogFile& it )
                      { return it.name() == file_name; } ) };
    if ( item != this->logs_list.end() ) {
        item->setSelected();
        return true;
    }
    return false;
}

void Craplog::clearLogFilesSelection()
{
    std::ignore = std::for_each(
        this->logs_list.begin(), this->logs_list.end(),
        []( LogFile& it )
          { if (it.isSelected()) it.setUnselected(); } );
}


// scan the logs path to update the log files list
void Craplog::scanLogsDir()
{
    this->logs_list.clear();
    const std::string& logs_path{ this->logs_paths.at( this->current_WS ) };
    if ( ! IOutils::isDir( logs_path ) ) {
        // this directory doesn't exists
        if ( IOutils::exists( logs_path ) ) {
            DialogSec::errDirNotExists( QString::fromStdString( logs_path ) );
        }
        return;
    }
    size_t size;
    QString name;
    std::string path;
    // iterate over entries in the logs folder
    for ( const auto& dir_entry : std::filesystem::directory_iterator{logs_path}) {
        // get the attributes
        path = dir_entry.path().string();
        name = QString::fromStdString( dir_entry.path().filename().string() );

        // match only valid files names
        if ( ! this->isFileNameValid( name.toStdString() ) ) {
            continue;
        }

        // check if it is actually a file
        if ( IOutils::checkFile( path ) ) {
            // it's a file, check the readability
            if ( ! IOutils::checkFile( path, true ) ) {
                // not readable, skip
                if ( this->dialogs_level == 2 ) {
                    DialogSec::warnFileNotReadable( name );
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
            IOutils::randomLines( path, content, 32ul );

        } catch ( GenericException& e ) {
            // failed closing gzip file pointer
            DialogSec::errGeneric( e.what() );
            continue;
        }

        if ( content.empty() ) {
            if ( this->dialogs_level == 2 ) {
                DialogSec::warnEmptyFile( name );
            }
            continue;
        }

        const LogType log_type = LogOps::defineFileType(
            content, this->logs_formats.at( this->current_WS ) );
        content.clear();
        switch ( log_type ) {
            case LogType::Failed:
                // failed to get the log type, do not append
                DialogSec::errFailedDefiningLogType( name );
            case LogType::Discarded:
                // skip
                continue;
            default:
                break;
        }

        std::string hash;
        try {
            this->hashOps.digestFile( path, hash );
        } catch ( GenericException& e ) {
            // failed to digest
            DialogSec::errGeneric( e.what() );
            continue;
        }

        // push in the list
        this->logs_list.push_back( LogFile{
            false, this->hashOps.hasBeenUsed( hash, this->current_WS ),
            size, name, hash, path });
    }
}


void Craplog::changeIisLogsBaseNames( const int& module_id )
{
    switch ( module_id ) {
        case 0: // W3C
            this->logs_base_names.at( 13u ).contains = "_ex"; break;
        case 1: // NCSA
            this->logs_base_names.at( 13u ).contains = "_nc"; break;
        case 2: // IIS
            this->logs_base_names.at( 13u ).contains = "_in"; break;

        default: // shouldn't be reachable
            throw GenericException( "Unexpected LogFormatModule ID: "+std::to_string( module_id ), true ); // leave un-catched
    }
}
const bool Craplog::isFileNameValid( const std::string& name ) const
{
    bool valid{ true };
    if ( ! this->logs_base_names.at( this->current_WS ).starts.empty() ) {
        if ( ! StringOps::startsWith( name, this->logs_base_names.at( this->current_WS ).starts ) ) {
            return false;
        }
    }
    if ( ! this->logs_base_names.at( this->current_WS ).contains.empty() ) {
        if ( ! StringOps::contains( name.substr( this->logs_base_names.at( this->current_WS ).starts.size() ),
                                    this->logs_base_names.at( this->current_WS ).contains ) ) {
            return false;
        }
    }
    if ( ! this->logs_base_names.at( this->current_WS ).ends.empty() ) {
        if ( ! StringOps::endsWith( name, this->logs_base_names.at( this->current_WS ).ends )
          && ! StringOps::endsWith( name, ".gz" ) ) {
            return false;
        }
    }

    switch ( this->current_WS ) {
        size_t start, stop;
        case 11 | 12:
            // further checks for apache / nginx
            start = name.rfind(".log." );
            if ( start == std::string::npos ) {
                valid &= false;
                break;
            }
            start += 5ul;
            stop = name.size()-1ul;
            if ( StringOps::endsWith( name, ".gz" ) ) {
                stop -= 3ul;
            }
            // serach for incremental numbers
            for ( size_t i{start}; i<=stop; i++ ) {
                if ( ! StringOps::isNumeric( name.at( i ) ) ) {
                    valid &= false;
                    break;
                }
            }
            break;

        case 13:
            // further checks for iis
            start = name.find( this->logs_base_names.at( 13u ).contains ) + 3ul;
            if ( start == std::string::npos ) {
                valid &= false;
                break;
            }
            stop = name.size()-5ul; // removing the finel '.log' extension
            if ( StringOps::endsWith( name, ".gz" ) ) {
                stop -= 3ul;
            }
            // search for date
            std::string date;
            for ( size_t i{start}; i<=stop; i++ ) {
                if ( ! StringOps::isNumeric( name.at( i ) ) ) {
                    valid &= false;
                    break;
                }
                date.push_back( name.at( i ) );
            }
            if ( valid ) {
                // check if the file has today's date
                time_t t;
                time( &t );
                struct tm* tmp = localtime( &t );
                char aux_date[7];
                // using strftime to display time
                strftime( aux_date, 7, "%y%m%d", tmp );
                valid &= false;
                for ( size_t i{0}; i<6ul; i++ ) {
                    if ( date.at(i) != aux_date[i] ) {
                        // different date, valid
                        valid |= true;
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
const bool Craplog::checkStuff()
{
    this->proceed |= true;
    {
        const size_t l_size{ this->logs_list.size() };
        this->log_files_to_use.clear();
        if ( this->log_files_to_use.capacity() < l_size ) {
            this->log_files_to_use.reserve( l_size );
        }
        this->used_files_hashes.clear();
        if ( this->used_files_hashes.capacity() < l_size ) {
            this->used_files_hashes.reserve( l_size );
        }
    }

    for ( const LogFile& file : this->logs_list ) {

        if ( ! this->proceed ) { break; }

        if ( ! file.isSelected() ) {
            // not selected, skip
            continue;
        }

        // check if the file has been used already
        if ( file.hasBeenUsed() ) {
            // already used
            QString msg{ file.name() };
            if ( this->dialogs_level == 2 ) {
                msg += "\n" + QString::fromStdString( file.hash() );
            }
            const int choice = DialogSec::choiceFileAlreadyUsed( msg );
            if ( choice == 0 ) {
                // choosed to abort all
                this->proceed &= false;
                break;
            } else if ( choice == 1 ) {
                // choosed to discard the file and continue
                continue;
            } else if ( choice == 2 ) {
                // choosed to ignore and use the file anyway
                ;
            } else {
                // shouldn't be here
                throw GenericException( "Unexpeced value returned: "+std::to_string(choice) );
            }
        } else {
            // not used already, check for duplicates in the same list
            if ( VecOps::contains( this->used_files_hashes, file.hash() ) ) {
                // appears twice in the list
                QString msg{ file.name() };
                if ( this->dialogs_level == 2 ) {
                        msg += "\n" + QString::fromStdString( file.hash() );
                }
                const int choice = DialogSec::choiceDuplicateFile( msg );
                if ( choice == 0 ) {
                        // choosed to abort all
                        this->proceed &= false;
                        break;
                } else if ( choice == 1 ) {
                        // choosed to discard the file and continue
                        continue;
                } else if ( choice == 2 ) {
                        // choosed to ignore and use the file anyway
                        ;
                } else {
                        // shouldn't be here
                        throw GenericException( "Unexpeced value returned: "+std::to_string(choice) );
                }
            }
        }

        // check if the file respects the warning size
        if ( this->warning_size > 0ul ) {
            if ( file.size() > this->warning_size ) {
                // exceeds the warning size
                QString msg{ file.name() };
                if ( this->dialogs_level >= 1 ) {
                    msg += QString("\n\n%1:\n%2").arg(
                        DialogSec::tr("Size of the file"),
                        PrintSec::printableSize( file.size() ) );
                    if ( this->dialogs_level == 2 ) {
                        msg += QString("\n\n%1:\n%2").arg(
                            DialogSec::tr("Warning size parameter"),
                            PrintSec::printableSize( this->warning_size ) );
                    }
                }
                const int choice = DialogSec::choiceFileSizeWarning( msg );
                if ( choice == 0 ) {
                    // choosed to abort all
                    this->proceed &= false;
                    break;
                } else if ( choice == 1 ) {
                    // choosed to discard the file and continue
                    continue;
                } else if ( choice == 2 ) {
                    // choosed to ignore and use the file anyway
                    ;
                } else {
                    // shouldn't be here
                    throw GenericException( "Unexpeced value returned: "+std::to_string(choice) );
                }
            }
        }

        // check if the databases are fune
        if ( ! CheckSec::checkCollectionDatabase( this->db_stats_path ) ) {
            // checks failed, abort
            this->proceed &= false;
            break;
        }
        if ( ! CheckSec::checkHashesDatabase( this->db_hashes_path ) ) {
            // checks failed, abort
            this->proceed &= false;
            break;
        }

        this->log_files_to_use.push_back(
            std::make_tuple( file.path(), file.hash() )
        );
        this->used_files_hashes.push_back( file.hash() );
    }

    if ( this->proceed && this->log_files_to_use.size() == 0ul ) {
        // no files left, abort
        DialogSec::msgNoFileToParse();
        this->proceed &= false;
    }

    return this->proceed;
}


void Craplog::startWorking()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->proceed |= true;
    this->total_lines  = 0ul;
    this->parsed_lines = 0ul;
    this->total_size   = 0ul;
    this->parsed_size  = 0ul;
    this->warnlisted_size  = 0ul;
    this->blacklisted_size = 0ul;
    // hire a worker
    CraplogWorker* worker{ new CraplogWorker(
        this->current_WS,
        this->dialogs_level,
        this->db_stats_path,
        this->db_hashes_path,
        this->logs_formats.at( this->current_WS ),
        this->blacklists.at( this->current_WS ),
        this->warnlists.at( this->current_WS ),
        this->log_files_to_use
    ) };
    QThread* worker_thread{ new QThread() };
    worker->moveToThread( worker_thread );
    // start the worker
    connect( worker_thread, &QThread::started,
             worker, &CraplogWorker::work );
    // worker started parsing
    connect( worker, &CraplogWorker::startedParsing,
             this, &Craplog::workerStartedParsing );
    // worker finished parsing
    connect( worker, &CraplogWorker::finishedParsing,
             this, &Craplog::workerFinishedParsing );
    // receive performance data
    connect( worker, &CraplogWorker::perfData,
             this, &Craplog::updatePerfData );
    // receive chart data, only received when worker has done
    connect( worker, &CraplogWorker::chartData,
             this, &Craplog::updateChartData );
    // worker finished its career
    connect( worker, &CraplogWorker::done,
             this, &Craplog::stopWorking );
    // plan deleting the thread
    connect( worker, &CraplogWorker::retire,
             worker_thread, &QThread::quit );
    // plan deleting the worker
    connect( worker, &CraplogWorker::retire,
             worker, &CraplogWorker::deleteLater );
    // make the worker work
    worker_thread->start();
}
void Craplog::stopWorking( const bool successful )
{
    this->db_edited = successful;
    if ( successful ) {
        // insert the hashes of the used files
        this->hashOps.insertUsedHashes( this->db_hashes_path, this->used_files_hashes, this->current_WS );
    }
    emit this->finishedWorking();
}

const bool Craplog::editedDatabase() const
{
    return this->db_edited;
}


const size_t Craplog::getParsedSize()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    return this->parsed_size;
}
const size_t Craplog::getParsedLines()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    return this->parsed_lines;
}
const QString Craplog::getParsingSpeed()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    auto stop{ ( is_parsing )
        ? std::chrono::system_clock::now()
        : this->parsing_time_stop };
    return PrintSec::printableSpeed(
        static_cast<double>(
            this->parsed_size ),
        static_cast<double>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                stop - this->parsing_time_start
            ).count()
            ) * 0.001
        );
}

void Craplog::workerStartedParsing()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->is_parsing |= true;
    this->parsing_time_start = std::chrono::system_clock::now();
}
void Craplog::workerFinishedParsing()
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->parsing_time_stop = std::chrono::system_clock::now();
    this->is_parsing &= false;
}
const bool Craplog::isParsing() const
{
    return this->is_parsing;
}

void Craplog::updatePerfData( const size_t parsed_size, const size_t parsed_lines )
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->parsed_size  = parsed_size;
    this->parsed_lines = parsed_lines;
}
void Craplog::updateChartData( const size_t total_size, const size_t total_lines, const size_t warnlisted_size, const size_t blacklisted_size )
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->total_size  = total_size;
    this->total_lines = total_lines;
    this->warnlisted_size  = warnlisted_size;
    this->blacklisted_size = blacklisted_size;
}


void Craplog::makeChart( const QChart::ChartTheme& theme, const std::unordered_map<std::string, QFont>& fonts, QChartView* size_chart ) const
{
    const QString
        size_chart_name        { TR::tr("Logs Size Breakdown") },
        ignored_slice_name     { TR::tr("Ignored")             },
        parsed_slice_name      { TR::tr("Parsed")              },
        warning_slice_name     { TR::tr("Warnings")            },
        blacklisted_slice_name { TR::tr("Blacklisted")         };

    // logs size donut chart
    QPieSeries* parsedSize_donut{ new QPieSeries() };
    parsedSize_donut->setName( PrintSec::printableSize( this->parsed_size + this->blacklisted_size ) );
    const size_t parsed_size{ this->parsed_size - this->warnlisted_size };
    parsedSize_donut->append(
        "P@" + parsed_slice_name + "@" + PrintSec::printableSize( parsed_size ),
        parsed_size );
    parsedSize_donut->append(
        "W@" + warning_slice_name + "@" + PrintSec::printableSize( this->warnlisted_size ),
        this->warnlisted_size );
    parsedSize_donut->append(
        "B@" + blacklisted_slice_name + "@" + PrintSec::printableSize( this->blacklisted_size ),
        this->blacklisted_size );

    // logs size donut chart
    QPieSeries* ignoredSize_donut{ new QPieSeries() };
    const size_t ignored_size{ this->total_size - this->parsed_size - this->blacklisted_size };
    QString printable_ignored_size{ PrintSec::printableSize( ignored_size ) };
    ignoredSize_donut->setName( printable_ignored_size );
    ignoredSize_donut->append(
        "I@#" + ignored_slice_name + "@#" + printable_ignored_size,
        ignored_size );
    ignoredSize_donut->setLabelsVisible( false );

    DonutBreakdown* sizeBreakdown{ new DonutBreakdown() };
    sizeBreakdown->setTheme( theme );
    sizeBreakdown->setAnimationOptions( QChart::AllAnimations );
    sizeBreakdown->setTitle( size_chart_name );
    if ( this->proceed && this->total_size > 0ul ) {
        sizeBreakdown->legend()->setAlignment( Qt::AlignRight );
        sizeBreakdown->addBreakdownSeries( parsedSize_donut, Qt::GlobalColor::darkCyan, fonts.at("main_small") );
        sizeBreakdown->addBreakdownSeries( ignoredSize_donut, Qt::GlobalColor::gray, fonts.at("main_small") );
    } else {
        sizeBreakdown->legend()->setVisible( false );
        sizeBreakdown->setTitle("");
    }
    sizeBreakdown->legend()->markers( ignoredSize_donut ).first()->setVisible( false );

    size_chart->setChart( sizeBreakdown );
    size_chart->setRenderHint( QPainter::Antialiasing );
}

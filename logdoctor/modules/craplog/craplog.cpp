
#include "craplog.h"

#include "utilities/checks.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/memory.h"
#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"
#include "modules/shared.h"

#include "modules/craplog/modules/donuts.h"
#include "modules/craplog/modules/logs.h"
#include "modules/craplog/modules/workers/lister.h"
#include "modules/craplog/modules/workers/parser.h"

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
    for ( const WebServer& w : {WS_APACHE,WS_NGINX,WS_IIS} ) {
        this->warnlists.emplace(  w, std::unordered_map<int, BWlist>(4) );
        this->blacklists.emplace( w, std::unordered_map<int, BWlist>(1) );
        // default data
        this->warnlists.at( w ).emplace( 11, BWlist{ .used=false, .list={} } );
        this->warnlists.at( w ).emplace( 12, BWlist{ .used=false, .list={} } );
        this->warnlists.at( w ).emplace( 20, BWlist{ .used=false, .list={} } );
        this->warnlists.at( w ).emplace( 21, BWlist{ .used=false, .list={} } );
        this->blacklists.at( w ).emplace( 20, BWlist{ .used=false, .list={} } );
    }

    // default format strings
    this->logs_format_strings.emplace(
        WS_APACHE, "" );
    this->logs_format_strings.emplace(
        WS_NGINX,  "" );
    this->logs_format_strings.emplace(
        WS_IIS,    "" );

    // initialize formats
    this->logs_formats.emplace(
        WS_APACHE, LogsFormat() );
    this->logs_formats.emplace(
        WS_NGINX,  LogsFormat() );
    this->logs_formats.emplace(
        WS_IIS,    LogsFormat() );

    this->current_log_format = this->logs_formats.at( WS_APACHE );

    // apache2 access/error logs location
    this->logs_paths.emplace( WS_APACHE, "/var/log/apache2" );
    // nginx access/error logs location
    this->logs_paths.emplace( WS_NGINX, "/var/log/nginx" );
    // iis access/error logs location
    this->logs_paths.emplace( WS_IIS, "C:/inetpub/logs/LogFiles" );

    // apache2 access/error log files' names
    this->logs_base_names.emplace( WS_APACHE, LogName{ .starts   = "access.log.",
                                                       .contains = "",
                                                       .ends     = "" } );
    // nginx access/error log files' names
    this->logs_base_names.emplace( WS_NGINX, LogName{ .starts   = "access.log.",
                                                      .contains = "",
                                                      .ends     = "" });
    // iis access/error log files' names
    this->logs_base_names.emplace( WS_IIS, LogName{ .starts   = "",
                                                    .contains = "_ex",
                                                    .ends     = ".log" });
}


//////////////////
//// SETTINGS ////
DialogsLevel Craplog::getDialogsLevel() const noexcept
{
    return this->dialogs_level;
}
void Craplog::setDialogsLevel( const DialogsLevel new_level ) noexcept
{
    this->dialogs_level = new_level;
    this->hashOps.setDialogLevel( new_level );
}

const std::string& Craplog::getStatsDatabasePath() const noexcept
{
    return this->db_stats_path;
}
const std::string& Craplog::getHashesDatabasePath() const noexcept
{
    return this->db_hashes_path;
}

void Craplog::setStatsDatabasePath( const std::string& path ) noexcept
{
    this->db_stats_path = path + "/collection.db";
}
void Craplog::setHashesDatabasePath( const std::string& path ) noexcept
{
    this->db_hashes_path = path + "/hashes.db";
}

size_t Craplog::getWarningSize() const noexcept
{
    return this->warning_size;
}

void Craplog::setWarningSize(const size_t new_size ) noexcept
{
    this->warning_size = new_size;
}


////////////////////
//// WARN/BLACK ////
bool Craplog::isBlacklistUsed( const WebServer& web_server, const int& log_field_id ) const noexcept
{
    return this->blacklists.at( web_server ).at( log_field_id ).used;
}
bool Craplog::isWarnlistUsed( const WebServer& web_server, const int& log_field_id ) const noexcept
{
    return this->warnlists.at( web_server ).at( log_field_id ).used;
}

void Craplog::setBlacklistUsed( const WebServer& web_server, const int& log_field_id, const bool used ) noexcept
{
    this->blacklists.at( web_server ).at( log_field_id ).used = used;
}
void Craplog::setWarnlistUsed( const WebServer& web_server, const int& log_field_id, const bool used ) noexcept
{
    this->warnlists.at( web_server ).at( log_field_id ).used = used;
}

const std::vector<std::string>& Craplog::getBlacklist( const WebServer& web_server, const int& log_field_id ) const noexcept
{
    return this->blacklists.at( web_server ).at( log_field_id ).list;
}
const std::vector<std::string>& Craplog::getWarnlist( const WebServer& web_server, const int& log_field_id ) const noexcept
{
    return this->warnlists.at( web_server ).at( log_field_id ).list;
}

void Craplog::setBlacklist( const WebServer& web_server, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->blacklists.at( web_server ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->blacklistAdd( web_server, log_field_id, item );
    }
}
void Craplog::setWarnlist( const WebServer& web_server, const int& log_field_id, const std::vector<std::string>& new_list )
{
    this->warnlists.at( web_server ).at( log_field_id ).list.clear();
    for ( const std::string& item : new_list ) {
        this->warnlistAdd( web_server, log_field_id, item );
    }
}

void Craplog::blacklistAdd( const WebServer& web_server, const int& log_field_id, const std::string& new_item )
{
    this->blacklists.at( web_server ).at( log_field_id ).list.push_back(
        this->sanitizeBWitem( log_field_id, new_item ) );
}
void Craplog::warnlistAdd( const WebServer& web_server, const int& log_field_id, const std::string& new_item )
{
    this->warnlists.at( web_server ).at( log_field_id ).list.push_back(
        this->sanitizeBWitem( log_field_id, new_item ) );
}

void Craplog::blacklistRemove( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->blacklists.at( web_server ).at( log_field_id ).list;
    if ( const auto it{ std::find( list.cbegin(), list.cend(), item ) }; it != list.cend() ) {
        list.erase( it );
    }
}
void Craplog::warnlistRemove( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->warnlists.at( web_server ).at( log_field_id ).list;
    if ( const auto it{ std::find( list.cbegin(), list.cend(), item ) }; it != list.cend() ) {
        list.erase( it );
    }
}

int Craplog::blacklistMoveUp( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->blacklists.at( web_server ).at( log_field_id ).list;
    if ( auto it{ std::find( std::next(list.begin()), list.end(), item ) }; it != list.cend() ) {
        const int pos{ static_cast<int>( std::distance(list.begin(), it) ) - 1 };
        std::swap( *it, *std::prev(it) );
        return pos;
    }
    return -1;
}
int Craplog::warnlistMoveUp( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->warnlists.at( web_server ).at( log_field_id ).list;
    if ( auto it{ std::find( std::next(list.begin()), list.end(), item ) }; it != list.cend() ) {
        const int pos{ static_cast<int>( std::distance(list.begin(), it) ) - 1 };
        std::swap( *it, *std::prev(it) );
        return pos;
    }
    return -1;
}

int Craplog::blacklistMoveDown( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->blacklists.at( web_server ).at( log_field_id ).list;
    if ( auto it{ std::find( list.begin(), std::prev(list.end()), item ) }; it != list.cend() ) {
        const int pos{ static_cast<int>( std::distance(list.begin(), it) ) + 1 };
        std::swap( *it, *std::next(it) );
        return pos;
    }
    return -1;
}
int Craplog::warnlistMoveDown( const WebServer& web_server, const int& log_field_id, const std::string& item ) noexcept
{
    auto& list = this->warnlists.at( web_server ).at( log_field_id ).list;
    if ( auto it{ std::find( list.begin(), std::prev(list.end()), item ) }; it != list.cend() ) {
        const int pos{ static_cast<int>( std::distance(list.begin(), it) ) + 1 };
        std::swap( *it, *std::next(it) );
        return pos;
    }
    return -1;
}

std::string Craplog::sanitizeBWitem( const int& log_field_id, const std::string& new_item ) const
{
    std::string sanitized_item;
    switch ( log_field_id ) {
        case 11:
            sanitized_item = StringOps::strip( new_item );
            if ( ! StringOps::isAlphabetic( sanitized_item ) ) {
                // only letters allowed
                throw BWlistException("Invalid Method");
            }
            sanitized_item = StringOps::toUpper( sanitized_item );
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
const std::string& Craplog::getLogsFormatString( const WebServer& web_server ) const noexcept
{
    return this->logs_format_strings.at( web_server );
}

// get the logs format
const LogsFormat& Craplog::getLogsFormat(const WebServer& web_server ) const noexcept
{
    return this->logs_formats.at( web_server );
}

// set the logs format
bool Craplog::setApacheLogFormat( const std::string& format_string ) noexcept
{
    // apache
    bool success{ true };
    try {
        this->logs_formats.at( WS_APACHE ) =
            this->formatOps.processApacheFormatString( format_string );
        this->logs_format_strings.at( WS_APACHE ) = format_string;
    } catch ( LogFormatException& e ) {
        success &= false;
        DialogSec::errInvalidLogFormatString( e.what() );
    } catch (...) {
        success &= false;
        DialogSec::errGeneric( DialogSec::tr("An error occured while parsing the format string"), true );
    }
    return success;
}
bool Craplog::setNginxLogFormat( const std::string& format_string ) noexcept
{
    // nginx
    bool success{ true };
    try {
        this->logs_formats.at( WS_NGINX ) =
            this->formatOps.processNginxFormatString( format_string );
        this->logs_format_strings.at( WS_NGINX ) = format_string;
    } catch ( LogFormatException& e ) {
        success &= false;
        DialogSec::errInvalidLogFormatString( e.what() );
    } catch (...) {
        success &= false;
        DialogSec::errGeneric( DialogSec::tr("An error occured while parsing the format string"), true );
    }
    return success;
}
bool Craplog::setIisLogFormat( const std::string& format_string, const int log_module ) noexcept
{
    // iis
    bool success{ true };
    try {
        this->logs_formats.at( WS_IIS ) =
            this->formatOps.processIisFormatString( format_string, log_module );
        this->logs_format_strings.at( WS_IIS ) = format_string;
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

QString Craplog::getLogsFormatSample( const WebServer& web_server ) const
{
    switch ( web_server ) {
        case WS_APACHE:
            return this->formatOps.getApacheLogSample( this->logs_formats.at( web_server ) );
        case WS_NGINX:
            return this->formatOps.getNginxLogSample( this->logs_formats.at( web_server ) );
        case WS_IIS:
            return this->formatOps.getIisLogSample( this->logs_formats.at( web_server ) );
        default:
            throw WebServerException( "Unexpected WebServer: " + toString(web_server) );
    }
}

bool Craplog::checkCurrentLogsFormat() const noexcept
{
    if ( this->current_log_format.string.empty() ) {
            // format string not set
            DialogSec::errLogFormatNotSet( nullptr );
            return false;
    } else if ( this->current_log_format.fields.empty() ) {
            // no field, useless to parse
            DialogSec::errLogFormatNoFields( nullptr );
            return false;
    } else if ( this->current_log_format.separators.size() < this->current_log_format.fields.size()-1 ) {
            // at least one separator is missing between two (or more) fields
            DialogSec::errLogFormatNoSeparators( nullptr );
            return false;
    }
    return true;
}


// set the current Web Server
void Craplog::setCurrentWebServer( const WebServer web_server ) noexcept
{
    this->current_web_server = web_server;
    this->setCurrentLogFormat();
}

WebServer Craplog::getCurrentWSID() const noexcept
{
    return this->current_web_server;
}

// set the current access logs format
void Craplog::setCurrentLogFormat() noexcept
{
    this->current_log_format = this->logs_formats.at( this->current_web_server );
}

// get the current access logs format
const LogsFormat& Craplog::getCurrentLogFormat() const noexcept
{
    return this->current_log_format;
}


///////////////////
//// LOGS PATH ////
const std::string& Craplog::getLogsPath( const WebServer& web_server ) const noexcept
{
    return this->logs_paths.at( web_server );
}
void Craplog::setLogsPath( const WebServer& web_server, const std::string& new_path ) noexcept
{
    this->logs_paths.at( web_server ) = new_path;
}


///////////////////
//// LOGS LIST ////
// return the size of the list
size_t Craplog::getLogsListSize() const noexcept
{
    return this->logs_list.size();
}

// return the current list
const std::vector<LogFile>& Craplog::getLogsList() const noexcept
{
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
bool Craplog::setLogFileSelected( const QString& file_name ) noexcept
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

void Craplog::clearLogFilesSelection() noexcept
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
    // hire the worker
    CraplogLister* worker{ new CraplogLister(
        this->current_web_server,
        this->dialogs_level,
        this->logs_paths.at( this->current_web_server ),
        this->logs_formats.at( this->current_web_server ),
        this->hashOps,
        [this]( const std::string& file_name)
              { return this->isFileNameValid( file_name ); }
    ) };
    QThread* worker_thread{ new QThread() };
    worker->moveToThread( worker_thread );
    // start the worker
    connect( worker_thread, &QThread::started,
             worker, &CraplogLister::work );
    // receive a new log file
    connect( worker, &CraplogLister::pushLogFile,
             this, &Craplog::appendLogFile );
    // show a dialog
    connect( worker, &CraplogLister::showDialog,
             this, &Craplog::showWorkerDialog );
    // worker finished its career
    connect( worker, &CraplogLister::done,
             this, &Craplog::logsDirScanned );
    // plan deleting the worker
    connect( worker, &CraplogLister::retire,
             worker, &CraplogLister::deleteLater );
    // quit the thread
    connect( worker, &CraplogLister::retire,
             worker_thread, &QThread::quit );
    // plan deleting the thread
    connect( worker_thread, &QThread::finished,
             worker_thread, &QThread::deleteLater );
    // make the worker work
    worker_thread->start();
}

void Craplog::appendLogFile( const LogFile log_file ) noexcept
{
    this->logs_list.push_back( std::move( log_file ) );
    emit this->pushLogFile( this->logs_list.back() );
}

void Craplog::logsDirScanned() noexcept
{
    emit this->finishedRefreshing();
}


void Craplog::changeIisLogsBaseNames( const int module_id )
{
    switch ( module_id ) {
        case 0: // W3C
            this->logs_base_names.at( WS_IIS ).contains = "_ex"; break;
        case 1: // NCSA
            this->logs_base_names.at( WS_IIS ).contains = "_nc"; break;
        case 2: // IIS
            this->logs_base_names.at( WS_IIS ).contains = "_in"; break;

        default: // shouldn't be reachable
            throw GenericException( "Unexpected LogFormatModule ID: "+std::to_string( module_id ), true ); // leave un-catched
    }
}

bool Craplog::isFileNameValid( const std::string& name ) const
{
    bool valid{ true };
    if ( ! this->logs_base_names.at( this->current_web_server ).starts.empty() ) {
        if ( ! StringOps::startsWith( name, this->logs_base_names.at( this->current_web_server ).starts ) ) {
            return false;
        }
    }
    if ( ! this->logs_base_names.at( this->current_web_server ).contains.empty() ) {
        if ( ! StringOps::contains( name.substr( this->logs_base_names.at( this->current_web_server ).starts.size() ),
                                    this->logs_base_names.at( this->current_web_server ).contains ) ) {
            return false;
        }
    }
    if ( ! this->logs_base_names.at( this->current_web_server ).ends.empty() ) {
        if ( ! StringOps::endsWith( name, this->logs_base_names.at( this->current_web_server ).ends )
          && ! StringOps::endsWith( name, ".gz" ) ) {
            return false;
        }
    }

    switch ( this->current_web_server ) {
        case WS_APACHE:
            [[fallthrough]];
        case WS_NGINX: {
            // further checks for apache / nginx
            size_t start, stop;
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
            for ( size_t i{start}; i<=stop; ++i ) {
                if ( ! CharOps::isNumeric( name.at( i ) ) ) {
                    valid &= false;
                    break;
                }
            }
            }break;

        case WS_IIS: {
            // further checks for iis
            size_t start, stop;
            start = name.find( this->logs_base_names.at( WS_IIS ).contains ) + 3ul;
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
            for ( size_t i{start}; i<=stop; ++i ) {
                if ( ! CharOps::isNumeric( name.at( i ) ) ) {
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
                for ( size_t i{0}; i<6ul; ++i ) {
                    if ( date.at(i) != aux_date[i] ) {
                        // different date, valid
                        valid |= true;
                        break;
                    }
                }
            }
            }break;

        default:
            throw WebServerException( "Unexpected WebServer: " + toString(this->current_web_server) );
    }
    return valid;
}


///////////////
//// WORKK ////
bool Craplog::checkStuff()
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

    size_t logs_size{ 0ul };
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
            if ( this->dialogs_level == DL_EXPLANATORY ) {
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
                if ( this->dialogs_level == DL_EXPLANATORY ) {
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
                if ( this->dialogs_level >= DL_NORMAL ) {
                    msg += QString("\n\n%1:\n%2").arg(
                        DialogSec::tr("Size of the file"),
                        PrintSec::printableSize( file.size() ) );
                    if ( this->dialogs_level == DL_EXPLANATORY ) {
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
        logs_size += file.size();
    }

    // check if there are enough files to use
    if ( this->proceed && this->log_files_to_use.size() == 0ul ) {
        // no files left, abort
        DialogSec::msgNoFileToParse();
        this->proceed &= false;
    }

    // check if the total size of the files do not exceed the available RAM
    if ( this->proceed && logs_size >= MemOps::availableMemory() ) {
        // no files left, abort
        QString msg;
        if ( this->dialogs_level >= DL_NORMAL ) {
            msg += QString("\n\n%1: %2").arg(
                DialogSec::tr("Available memory"),
                PrintSec::printableSize( MemOps::availableMemory() ) );
            if ( this->dialogs_level == DL_EXPLANATORY ) {
                msg += QString("\n%1: %2").arg(
                    DialogSec::tr("Size of the logs"),
                    PrintSec::printableSize( logs_size ) );
            }
        }
        DialogSec::msgNotEnoughMemory( msg );
        this->proceed &= false;
    }

    return this->proceed;
}

void Craplog::showWorkerDialog( const WorkerDialog dialog_type, const QStringList args ) const noexcept
{
    switch ( dialog_type ) {
        case WorkerDialog::errGeneric:
            DialogSec::errGeneric( args.at(0) );
            break;
        case WorkerDialog::errDirNotExists:
            DialogSec::errDirNotExists( args.at(0) );
            break;
        case WorkerDialog::errFailedDefiningLogType:
            DialogSec::errFailedDefiningLogType( args.at(0) );
            break;
        case WorkerDialog::errFailedParsingLogs:
            DialogSec::errFailedParsingLogs( args.at(0) );
            break;
        case WorkerDialog::errDatabaseFailedOpening:
            if ( args.size() < 2 ) {
                // do not throw, just print to stderr
                GenericException{ "call to showWorkerDialog() with invalid number of list items", true };
            }
            DialogSec::errDatabaseFailedOpening( args.at(0), args.at(1) );
            break;
        case WorkerDialog::errDatabaseFailedExecuting:
            if ( args.size() < 3 ) {
                // do not throw, just print to stderr
                GenericException{ "call to showWorkerDialog() with invalid number of list items", true };
            }
            DialogSec::errDatabaseFailedExecuting( args.at(0), args.at(1), args.at(2) );
            break;
        case WorkerDialog::warnFileNotReadable:
            DialogSec::warnFileNotReadable( args.at(0) );
            break;
        case WorkerDialog::warnEmptyFile:
            DialogSec::warnEmptyFile( args.at(0) );
            break;
    }
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
    this->hireWorker();
}
void Craplog::hireWorker() const
{
    CraplogParser* worker{ new CraplogParser(
        this->current_web_server,
        this->dialogs_level,
        this->db_stats_path,
        this->db_hashes_path,
        this->logs_formats.at( this->current_web_server ),
        this->blacklists.at( this->current_web_server ),
        this->warnlists.at( this->current_web_server ),
        this->log_files_to_use
    ) };
    QThread* worker_thread{ new QThread() };
    worker->moveToThread( worker_thread );
    // start the worker
    connect( worker_thread, &QThread::started,
             worker, &CraplogParser::work );
    // worker started parsing
    connect( worker, &CraplogParser::startedParsing,
             this, &Craplog::workerStartedParsing );
    // worker finished parsing
    connect( worker, &CraplogParser::finishedParsing,
             this, &Craplog::workerFinishedParsing );
    // receive performance data
    connect( worker, &CraplogParser::perfData,
             this, &Craplog::updatePerfData );
    // receive chart data, only received when worker has done
    connect( worker, &CraplogParser::chartData,
             this, &Craplog::updateChartData );
    // show a dialog
    connect( worker, &CraplogParser::showDialog,
             this, &Craplog::showWorkerDialog );
    // worker finished its career
    connect( worker, &CraplogParser::done,
             this, &Craplog::stopWorking );
    // plan deleting the worker
    connect( worker, &CraplogParser::retire,
             worker, &CraplogParser::deleteLater );
    // quit the thread
    connect( worker, &CraplogParser::retire,
             worker_thread, &QThread::quit );
    // plan deleting the thread
    connect( worker_thread, &QThread::finished,
             worker_thread, &QThread::deleteLater );
    // make the worker work
    worker_thread->start();
}
void Craplog::stopWorking( const bool successful )
{
    this->db_edited = successful;
    if ( successful ) {
        // insert the hashes of the used files
        this->hashOps.insertUsedHashes( this->db_hashes_path, this->used_files_hashes, this->current_web_server );
    }
    emit this->finishedWorking();
}

bool Craplog::editedDatabase() const noexcept
{
    return this->db_edited;
}


size_t Craplog::getParsedSize() noexcept
{
    std::unique_lock<std::mutex> lock( this->mutex );
    return this->parsed_size;
}
size_t Craplog::getParsedLines() noexcept
{
    std::unique_lock<std::mutex> lock( this->mutex );
    return this->parsed_lines;
}
QString Craplog::getParsingSpeed() noexcept
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

void Craplog::workerStartedParsing() noexcept
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->is_parsing |= true;
    this->parsing_time_start = std::chrono::system_clock::now();
}
void Craplog::workerFinishedParsing() noexcept
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->parsing_time_stop = std::chrono::system_clock::now();
    this->is_parsing &= false;
}
bool Craplog::isParsing() const noexcept
{
    return this->is_parsing;
}

void Craplog::updatePerfData( const size_t parsed_size, const size_t parsed_lines ) noexcept
{
    std::unique_lock<std::mutex> lock( this->mutex );
    this->parsed_size  = parsed_size;
    this->parsed_lines = parsed_lines;
}
void Craplog::updateChartData( const size_t total_size, const size_t total_lines, const size_t warnlisted_size, const size_t blacklisted_size ) noexcept
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
    parsedSize_donut->setName( PrintSec::printableSize( this->parsed_size ) );
    const size_t parsed_size{ this->parsed_size - this->warnlisted_size - this->blacklisted_size };
    parsedSize_donut->append(
        "P@" + parsed_slice_name + "@" + PrintSec::printableSize( parsed_size ),
        static_cast<qreal>( parsed_size ) );
    parsedSize_donut->append(
        "W@" + warning_slice_name + "@" + PrintSec::printableSize( this->warnlisted_size ),
        static_cast<qreal>( this->warnlisted_size ) );
    parsedSize_donut->append(
        "B@" + blacklisted_slice_name + "@" + PrintSec::printableSize( this->blacklisted_size ),
        static_cast<qreal>( this->blacklisted_size ) );

    // logs size donut chart
    QPieSeries* ignoredSize_donut{ new QPieSeries() };
    const size_t ignored_size{ this->total_size - this->parsed_size };
    QString printable_ignored_size{ PrintSec::printableSize( ignored_size ) };
    ignoredSize_donut->setName( printable_ignored_size );
    ignoredSize_donut->append(
        "I@#" + ignored_slice_name + "@#" + printable_ignored_size,
        static_cast<qreal>( ignored_size ) );
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
    if ( auto markers{ sizeBreakdown->legend()->markers( ignoredSize_donut ) }; !markers.isEmpty() ) {
        markers.first()->setVisible( false );
    }

    size_chart->setChart( sizeBreakdown );
    size_chart->setRenderHint( QPainter::Antialiasing );
}

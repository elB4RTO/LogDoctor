
#include "lister.h"

#include "utilities/io.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/craplog/modules/hash.h"
#include "modules/craplog/modules/logs.h"


CraplogLister::CraplogLister( const unsigned web_server_id, const unsigned dialogs_level, const std::string& logs_path, const LogsFormat& logs_format, const HashOps& hashOps, std::function<bool(const std::string&)> check_filename, QObject* parent )
    : QObject        { parent         }
    , wsID           { web_server_id  }
    , dialogs_level  { dialogs_level  }
    , logs_path      { logs_path      }
    , logs_format    { logs_format    }
    , hashOps        { hashOps        }
    , check_filename { check_filename }
{

}


void CraplogLister::work()
{
    const std::string& logs_path{ this->logs_path };
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
        if ( ! this->check_filename( name.toStdString() ) ) {
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
            content, this->logs_format );
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
        emit this->pushLogFile( LogFile{
            false, this->hashOps.hasBeenUsed( hash, this->wsID ),
            size, name, hash, path } );
    }
    emit this->done();
    emit this->retire();
}

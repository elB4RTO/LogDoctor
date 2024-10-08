
#include "lister.h"

#include "utilities/io.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/craplog/modules/lib.h"
#include "modules/craplog/modules/hasher.h"
#include "modules/craplog/utilities/logs.h"

#include "modules/craplog/workers/lib.h"


CraplogLister::CraplogLister( const WebServer web_server, const DialogsLevel dialogs_level, const PathHandler& logs_path, const LogsFormat& logs_format, const Hasher& hasher, const std::function<bool(const std::string&)> check_filename, QObject* parent )
    : QObject        { parent         }
    , web_server     { web_server     }
    , dialogs_level  { dialogs_level  }
    , logs_path      { logs_path      }
    , logs_format    { logs_format    }
    , hasher         { hasher         }
    , check_filename { check_filename }
{
}


void CraplogLister::quit()
{
    emit this->done();
    emit this->retire();
}

void CraplogLister::work()
{
    const auto path_exp{ this->logs_path.getPath() };
    if ( ! path_exp.has_value() ) {
        if ( path_exp.error().isReasonSymlink() ) {
            emit this->showDialog( WorkerDialog::errPathHasSymlink,
                                  {toQString( path_exp.error().invalid_component ),
                                   toQString( path_exp.error().full_path )} );
            this->quit();
            return;
        } else {
            emit this->showDialog( WorkerDialog::errDirNotExists,
                                  {toQString( this->logs_path.getPathUnchecked() )} );
            this->quit();
            return;
        }
    } else if ( ! IOutils::isDir( path_exp.value() ) ) {
        emit this->showDialog( WorkerDialog::errDirNotExists,
                               {toQString( this->logs_path.getPathUnchecked() )} );
        this->quit();
        return;
    }
    std::size_t size;
    QString name;
    std::string path;
    // iterate over entries in the logs folder
    for ( const auto& dir_entry : std::filesystem::directory_iterator{path_exp.value()}) {
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
                if ( this->dialogs_level == DL_EXPLANATORY ) {
                    emit this->showDialog( WorkerDialog::warnFileNotReadable,
                                           {name} );
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
            emit this->showDialog( WorkerDialog::errGeneric,
                                   {e.what()} );
            continue;
        }

        if ( content.empty() ) {
            if ( this->dialogs_level == DL_EXPLANATORY ) {
                emit this->showDialog( WorkerDialog::warnEmptyFile,
                                       {name} );
            }
            continue;
        }

        const LogType log_type = LogOps::defineFileType(
            content, this->logs_format );
        content.clear();
        switch ( log_type ) {
        case LogType::Failed:
            // failed to get the log type, do not append
            emit this->showDialog( WorkerDialog::errFailedDefiningLogType,
                                   {name} );
        case LogType::Discarded:
            // skip
            continue;
        default:
            break;
        }

        std::string hash;
        try {
            this->hasher.digestFile( path, hash );
        } catch ( GenericException& e ) {
            // failed to digest
            emit this->showDialog( WorkerDialog::errGeneric,
                                   {e.what()} );
            continue;
        }

        // push in the list
        emit this->pushLogFile( LogFile{
            false, this->hasher.hasBeenUsed( hash, this->web_server ),
            size, name, hash, path } );
    }
    this->quit();
}

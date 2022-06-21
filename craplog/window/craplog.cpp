#include "craplog.h"

Craplog::Craplog()
{
    this-> logs_path = "/var/log/apache2";
    //this-> readConfigs();
    this-> scanLogsDir();
}

// return the list. rescan if fresh is true
std::vector<Craplog::LogFile> Craplog::getLogsList( bool fresh )
{
    if ( fresh == true ) {
        this->scanLogsDir();
    }
    return this-> logs_list;
}

// return the path of the file matching the given name
std::string Craplog::getLogFilePath( QString file_name )
{
    std::string path;
    for ( Craplog::LogFile& item : this->logs_list ) {
        if ( item.name == file_name ) {
            path = item.path;
            break;
        }
    }
    return path;
}

// scan the logs path to update the log files list
void Craplog::scanLogsDir()
{
    this-> logs_list.clear();
    // iterate over entries in the logs folder
    for (auto const& dir_entry : std::filesystem::directory_iterator{this->logs_path}) {
        // get the attributes
        int size = dir_entry.file_size();
        std::string path = dir_entry.path().string();
        std::string name = dir_entry.path().filename().string();
        // match only files having ".log." in their name
        if ( ! dir_entry.is_regular_file()
          || name.find(".log.") == -1 ) {
            continue;
        }
        // push in the list
        this-> logs_list.push_back(
            LogFile{
                .selected =  false,
                .size = size,
                .name = QString::fromStdString( name ),
                .path = dir_entry.path().string(),
        });
    }
}

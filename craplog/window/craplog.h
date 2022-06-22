#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <filesystem>

#include <QMainWindow>


class Craplog
{
public:
    Craplog();
    
    struct LogFile {
        bool selected;
        int size;
        QString name;
        std::string path;
    };
    std::vector<LogFile> getLogsList( bool fresh=false );
    int getLogsListSize();
    std::string getLogFilePath( QString file_name );
    int setLogFileSelected( QString file_name );

private:
    std::string logs_path;
    std::vector<LogFile> logs_list;
    
    void readConfigs();
    
    void scanLogsDir();
    void loadFileContent();
};

#endif // CRAPLOG_H

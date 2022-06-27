
#include "logs.h"

using std::string, std::vector, std::unordered_map;


LogOps::LogOps()
{

}


LogOps::LogType LogOps::defineFileType( string name, vector<string> lines, unordered_map<int, FormatOps::LogsFormat> formats )
{
    int n_acc=0, n_err=0;
    LogOps::LogType supposed_type, real_type;
    FormatOps::LogsFormat current_ALF = formats[1],
                          current_ELF = formats[2];
    if ( StringOps::startsWith( name, "access" ) ) {
        supposed_type = LogOps::LogType::Access;
    } else if ( StringOps::startsWith( name, "error" ) ) {
        supposed_type = LogOps::LogType::Error;
    } else {
        // this shouldn't be
            // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    }
    real_type = this->LogType::Failed;
    for ( const string& line : lines ) {
        if ( supposed_type == LogOps::LogType::Access ) {
            if ( current_ALF.initial.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::startsWith( line, current_ALF.initial ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            if ( current_ALF.final.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::endsWith( line, current_ALF.final ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            n_acc++;
        } else {
            if ( current_ELF.initial.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::startsWith( line, current_ELF.initial ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            if ( current_ELF.final.size() > 0 ) {
                // a fixed starter char is set
                if ( StringOps::endsWith( line, current_ELF.final ) == false ) {
                    // but wasn't found
                    continue;
                }
            }
            n_err++;
        }
    }
    if ( n_acc > 0 && n_err == 0 ) {
        // access logs
        real_type = LogOps::LogType::Access;
    } else if ( n_err > 0 && n_acc == 0 ) {
        // error logs
        real_type = LogOps::LogType::Error;
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


vector<string> LogOps::splitLine( string line, int type )
{

}

vector<string> LogOps::splitLines( string line, int type )
{

}


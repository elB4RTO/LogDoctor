#include "logs.h"

#include "utilities/strings.h"

LogOp::LogOp()
{

}


bool LogOp::isNameValid(std::string name)
{
    bool valid = false;
    if ( StringOp::startsWith( name, "access.log." )
      || StringOp::startsWith( name, "error.log." ) ) {

    }
    return valid;
}


Craplog::LogType LogOp::defineLogType( std::vector<std::string> lines )
{
    int n_acc=0, n_err=0;
    auto type = Craplog::LogType::Failed;
    for ( const std::string& line : lines ) {
        if ( line[0] == '[' ) {
            n_err ++;
        } else {
            if ( line.find('"') < line.size() ) {
                n_acc ++;
            }
        }
    }
    if ( n_acc > 0 && n_err == 0 ) {
        // access logs
        type = Craplog::LogType::Access;
    } else if ( n_err > 0 && n_acc == 0 ) {
        // error logs
        type = Craplog::LogType::Error;
    } else {
        // something is wrong with these logs, put a warnin
        if ( n_acc > 0 && n_err > 0 ) {
            // both access and error types found
            std::cout << "WRONG!" << std::endl; // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
        } else {
            // every line was invalid
            std::cout << "WRONG!" << std::endl; // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
        }
    }
    return type;
}

std::vector<std::string> LogOp::splitLine( std::string line, Craplog::LogType type )
{

}

std::vector<std::string> LogOp::splitLines( std::string line, Craplog::LogType type )
{

}

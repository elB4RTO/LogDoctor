#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <vector>

#include "qstring.h"

class StringOp
{
public:
    StringOp();
    
    static std::string strip( std::string str, std::string chars=" \n\t\b\r\v" );    
    static std::string lstrip( std::string str, std::string chars=" \n\t\b\r\v" );    
    static std::string rstrip( std::string str, std::string chars=" \n\t\b\r\v" );
    
    static std::vector<std::string> split( std::string str, std::string sep="\n" );
    
    static std::vector<std::string> splitrip( std::string str, std::string sep="\n", std::string chars=" \n\t\b\r\v" );
};

#endif // STRINGOP_H

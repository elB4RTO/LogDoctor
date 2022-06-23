#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <vector>
#include "qstring.h"

class StringOp
{
public:
    StringOp();
    
    std::string strip( std::string str, std::string chars=" \n\t\b\r\v" );    
    std::string lstrip( std::string str, std::string chars=" \n\t\b\r\v" );    
    std::string rstrip( std::string str, std::string chars=" \n\t\b\r\v" );
    
    std::vector<std::string> split( std::string str, std::string sep );
};

#endif // STRINGOP_H

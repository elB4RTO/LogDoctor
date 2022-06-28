#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <vector>

#include "qstring.h"

class StringOps
{
public:
    StringOps();


    static int count( std::string str, std::string flag, bool consecutives=true );

    static bool isNumeric( std::string str );
    static bool isNumeric( char chr );

    static bool startsWith( std::string str, std::string flag );
    static bool endsWith( std::string str, std::string flag );

    static std::string strip( std::string str, std::string chars=" \n\t\b\r\v" );
    static std::string lstrip( std::string str, std::string chars=" \n\t\b\r\v" );
    static std::string rstrip( std::string str, std::string chars=" \n\t\b\r\v" );
    static std::string lstripUntil( std::string str, std::string chr, bool inclusive=true, bool consecutives=true );

    static std::vector<std::string> split( std::string str, std::string sep="\n" );

    static std::vector<std::string> splitrip( std::string str, std::string sep="\n", std::string chars=" \n\t\b\r\v" );
};

#endif // StringOps_H

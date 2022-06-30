#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <vector>

#include "qstring.h"

class StringOps
{
public:
    StringOps();


    static int count( const std::string& str, const std::string& flag, const bool consecutives=true );

    static bool isNumeric( const std::string& str );
    static bool isNumeric( const char chr );

    static bool startsWith( const std::string& str, const std::string& flag );
    static bool endsWith( const std::string& str, const std::string& flag );

    static bool contains( const std::string& str, const std::string& flag );

    static std::string strip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string lstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string rstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string lstripUntil( const std::string& str, const std::string& chr, const bool inclusive=true, const bool consecutives=true );

    static std::vector<std::string> split( const std::string& str, const std::string& sep="\n" );

    static std::vector<std::string> splitrip( const std::string& str, const std::string& sep="\n", const std::string& chars=" \n\t\b\r\v" );
};

#endif // STRINGS_H

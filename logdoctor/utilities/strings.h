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

    static bool isAlphabetic( const std::string& str );
    static bool isAlphabetic( const char chr );

    static bool isAlnum( const std::string& str );
    static bool isAlnum( const char chr );

    static bool startsWith( const std::string& str, const std::string& flag );
    static bool endsWith(   const std::string& str, const std::string& flag );

    static bool contains( const std::string& str, const std::string& flag );

    static std::string strip(  const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string lstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string rstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );
    static std::string lstripUntil( const std::string& str, const std::string& chr, const bool inclusive=true, const bool consecutives=true );

    static void split( std::vector<std::string>& list, const std::string& target_str, const std::string& separator="\n" );

    static void splitrip( std::vector<std::string>& list, const std::string& target_str, const std::string& separator="\n", const std::string& strip=" \n\t\b\r\v" );
};

#endif // STRINGS_H

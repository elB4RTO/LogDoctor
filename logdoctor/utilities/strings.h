#ifndef LOGDOCTOR__UTILITIES__STRINGS_H
#define LOGDOCTOR__UTILITIES__STRINGS_H

#include "chars.h"

#include <string>
#include <vector>
#include <algorithm>


//! StringOps
/*!
    Utilities for the strings
*/
namespace StringOps
{

//! Count the occurrences of the given sequence in the given string
/*!
    \param str The target string
    \param flag The character to find
    \return The number of occurrences
*/
inline size_t count( std::string_view str, const char flag ) noexcept
{
    return static_cast<size_t>( std::count( str.cbegin(), str.cend(), flag ) );
}

//! Count the occurrences of the given sequence in the given string
/*!
    \param str The target string
    \param flag The string to find
    \return The number of occurrences
*/
size_t count( std::string_view str, std::string_view flag ) noexcept;

//! Checks whether a string only contains numeric characters
/*!
    \param str The target string
    \return The result of the check
*/
inline bool isNumeric( std::string_view str ) noexcept
{
    return str.empty()
         ? false
         : std::all_of( str.cbegin(), str.cend(), CharOps::isNumeric );
}

//! Checks whether a string only contains alphabetic characters
/*!
    \param str The target string
    \return The result of the check
*/
inline bool isAlphabetic( std::string_view str ) noexcept
{
    return str.empty()
         ? false
         : std::all_of( str.cbegin(), str.cend(), CharOps::isAlphabetic );
}

//! Checks whether a string only contains alpha-numeric characters
/*!
    \param str The target string
    \return The result of the check
*/
inline bool isAlnum( std::string_view str ) noexcept
{
    return str.empty()
         ? false
         : std::all_of( str.cbegin(), str.cend(), CharOps::isAlnum );
}

//! Checks whether a string contains only IPv4/IPv6 chars
/*!
    This method doesn't check if the IP could be actually valid,
    since this is beyond its purpose
    \param str The target string
    \return The result of the check
*/
inline bool isIP( std::string_view str ) noexcept
{
    return str.empty()
         ? false
         : std::all_of( str.cbegin(), str.cend(), CharOps::isIP );
}

//! Checks if a string starts with the given sequence
/*!
    \param str The target string
    \param flag The character to search for
    \return The result of the check
*/
inline bool startsWith( std::string_view str, const char flag ) noexcept
{
    return str.front() == flag;
}

//! Checks if a string starts with the given sequence
/*!
    Passing an empty flag ends in a false positive
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
inline bool startsWith( std::string_view str, std::string_view flag ) noexcept
{
    return str.rfind( flag, 0ul ) == 0ul;
}

//! Checks if a string ends with the given sequence
/*!
    \param str The target string
    \param flag The character to search for
    \return The result of the check
*/
inline bool endsWith( std::string_view str, const char flag ) noexcept
{
    return str.back() == flag;
}

//! Checks if a string ends with the given sequence
/*!
    Passing an empty flag ends in a false positive
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
inline bool endsWith( std::string_view str, std::string_view flag ) noexcept
{
    return str.rfind( flag ) == str.size()-flag.size();
}

//! Checks if a string contains the given sequence
/*!
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
inline bool contains( std::string_view str, std::string_view flag ) noexcept
{
    return str.find( flag ) != std::string::npos;
}

//!  Strips the given character from both the left and the right side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
std::string strip( const std::string& str, const char chr ) noexcept;

//!  Strips the given characters from both the left and the right side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
std::string strip( const std::string& str, std::string_view chars=" \n\t\b\r\v" ) noexcept;

//!  Strips the given character from the left side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
std::string lstrip( const std::string& str, const char chr ) noexcept;

//!  Strips the given characters from the left side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
std::string lstrip( const std::string& str, std::string_view chars=" \n\t\b\r\v" ) noexcept;

//! Strips the given character from the right side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
std::string rstrip( const std::string &str, const char chr ) noexcept;

//! Strips the given characters from the right side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
std::string rstrip( const std::string& str, std::string_view chars=" \n\t\b\r\v" ) noexcept;

//! Strips everything from a string starting from the left side untill the delimiter is found (a.k.a. cut)
/*!
    \param str The target string
    \param delim The delimiter
    \param inclusive Whether to also strip the delimiter or not
    \param consecutives Whether to strip all the occurrences of the delimiter if they're consecutive, only applies if inclusive
    \return The result string
*/
std::string lstripUntil( const std::string& str, const char delim, const bool inclusive=true, const bool consecutives=true ) noexcept;

//! Splits a string using a separator
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The character to use as separator
*/
void split( std::vector<std::string>& list, const std::string& target_str, const char separator='\n' ) noexcept;

//! Splits a string using a separator
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
*/
void split( std::vector<std::string>& list, const std::string& target_str, std::string_view separator ) noexcept;

//! Splits a string and strips all the splitted items
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
    \param strip The characters to strip away
*/
void splitrip( std::vector<std::string>& list, const std::string& target_str, const char separator='\n', std::string_view strips=" \n\t\b\r\v" ) noexcept;

//! Splits a string and strips all the splitted items
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
    \param strip The characters to strip away
*/
void splitrip( std::vector<std::string>& list, const std::string& target_str, std::string_view separator, std::string_view strips=" \n\t\b\r\v" ) noexcept;

//! Replaces all the occurrences of a sequence with another
/*!
    \param str The target string
    \param target The sequence which will be replaced
    \param replace The sequence to be used to replace the target
    \return The result string
*/
std::string replace( std::string_view str, std::string_view target, std::string_view replace ) noexcept;

//! Converts a string to upper case
/*!
    \param str The target string
    \return The result string
*/
std::string toUpper( std::string_view str ) noexcept;

//! Converts a string to lower case
/*!
    \param str The target string
    \return The result string
*/
std::string toLower( std::string_view str ) noexcept;

} // namespace StringOps


#endif // LOGDOCTOR__UTILITIES__STRINGS_H

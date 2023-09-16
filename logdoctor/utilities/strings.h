#ifndef LOGDOCTOR__UTILITIES__STRINGS_H
#define LOGDOCTOR__UTILITIES__STRINGS_H


#include <string>
#include <vector>


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
const size_t count( std::string_view str, const char flag );

//! Count the occurrences of the given sequence in the given string
/*!
    \param str The target string
    \param flag The string to find
    \return The number of occurrences
*/
const size_t count( std::string_view str, std::string_view flag );

//! Checks whether character is numeric
/*!
    \param str The target character
    \return The result of the check
    \see isNumeric()
*/
const bool isNumeric( const char& chr );

//! Checks whether a string only contains numeric characters
/*!
    \param str The target string
    \return The result of the check
*/
const bool isNumeric( std::string_view str );

//! Checks whether a character is alphabetic
/*!
    \param str The target character
    \return The result of the check
    \see isAlphabetic()
*/
const bool isAlphabetic( const char& chr );

//! Checks whether a string only contains alphabetic characters
/*!
    \param str The target string
    \return The result of the check
*/
const bool isAlphabetic( std::string_view str );

//! Checks whether a character is alpha-numeric
/*!
    \param str The target character
    \return The result of the check
    \see isAlnum
*/
const bool isAlnum( const char& chr );

//! Checks whether a string only contains alpha-numeric characters
/*!
    \param str The target string
    \return The result of the check
*/
const bool isAlnum( std::string_view str );

//! Checks whether a string only contains hexadecimal characters
/*!
    \param str The target character
    \return The result of the check
*/
const bool isHex( const char& chr );

//! Checks whether a string contains only IPv4/IPv6 chars
/*!
    This method doesn't check if the IP could be actually valid,
    since this is beyond its purpose
    \param str The target string
    \return The result of the check
*/
const bool isIP( std::string_view str );

//! Checks if a string starts with the given sequence
/*!
    \param str The target string
    \param flag The character to search for
    \return The result of the check
*/
const bool startsWith( std::string_view str, const char flag );

//! Checks if a string starts with the given sequence
/*!
    Passing an empty flag ends in a false positive
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
const bool startsWith( std::string_view str, std::string_view flag );

//! Checks if a string ends with the given sequence
/*!
    \param str The target string
    \param flag The character to search for
    \return The result of the check
*/
const bool endsWith( std::string_view str, const char flag );

//! Checks if a string ends with the given sequence
/*!
    Passing an empty flag ends in a false positive
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
const bool endsWith( std::string_view str, std::string_view flag );

//! Checks if a string contains the given sequence
/*!
    \param str The target string
    \param flag The sequence to search for
    \return The result of the check
*/
const bool contains( std::string_view str, std::string_view flag );

//!  Strips the given character from both the left and the right side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
const std::string strip( const std::string& str, const char chr );

//!  Strips the given characters from both the left and the right side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
const std::string strip( const std::string& str, std::string_view chars=" \n\t\b\r\v" );

//!  Strips the given character from the left side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
const std::string lstrip( const std::string& str, const char chr );

//!  Strips the given characters from the left side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
const std::string lstrip( const std::string& str, std::string_view chars=" \n\t\b\r\v" );

//! Strips the given character from the right side of a string
/*!
    \param str The target string
    \param chr The character to strip away
    \return The result string
*/
const std::string rstrip( const std::string &str, const char chr );

//! Strips the given characters from the right side of a string
/*!
    \param str The target string
    \param chars The characters to strip away
    \return The result string
*/
const std::string rstrip( const std::string& str, std::string_view chars=" \n\t\b\r\v" );

//! Strips everything from a string starting from the left side untill the delimiter is found (a.k.a. cut)
/*!
    \param str The target string
    \param delim The delimiter
    \param inclusive Whether to also strip the delimiter or not
    \param consecutives Whether to strip all the occurrences of the delimiter if they're consecutive, only applies if inclusive
    \return The result string
*/
const std::string lstripUntil( const std::string& str, const char delim, const bool inclusive=true, const bool consecutives=true );

//! Splits a string using a separator
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The character to use as separator
*/
void split( std::vector<std::string>& list, const std::string& target_str, const char separator='\n' );

//! Splits a string using a separator
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
*/
void split( std::vector<std::string>& list, const std::string& target_str, std::string_view separator );

//! Splits a string and strips all the splitted items
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
    \param strip The characters to strip away
*/
void splitrip( std::vector<std::string>& list, const std::string& target_str, const char separator='\n', std::string_view strips=" \n\t\b\r\v" );

//! Splits a string and strips all the splitted items
/*!
    \param list Will hold the splitted content
    \param target_str The target string
    \param separator The sequence to use as separator
    \param strip The characters to strip away
*/
void splitrip( std::vector<std::string>& list, const std::string& target_str, std::string_view separator, std::string_view strips=" \n\t\b\r\v" );

//! Replaces all the occurrences of a sequence with another
/*!
    \param str The target string
    \param target The sequence which will be replaced
    \param replace The sequence to be used to replace the target
    \return The result string
*/
const std::string replace( std::string_view str, std::string_view target, std::string_view replace );

//! Converts a string to upper case
/*!
    \param str The target string
    \return The result string
*/
const std::string toUpper( std::string_view str );

//! Converts a string to lower case
/*!
    \param str The target string
    \return The result string
*/
const std::string toLower( std::string_view str );

} // namespace StringOps


#endif // LOGDOCTOR__UTILITIES__STRINGS_H

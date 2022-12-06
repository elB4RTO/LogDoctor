#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <vector>


//! StringOps
/*!
    Utilities for the strings
*/
class StringOps
{
public:
    StringOps();


    //! Count the occurrences of the given sequence in the given string
    /*!
        \param str The target string
        \return The number of occurrences
    */
    static const int count( const std::string& str, const std::string& flag, const bool& consecutives=true );

    //! Checks whether a string only contains numeric characters
    /*!
        \param str The target string
        \return The result of the check
    */
    static const bool isNumeric( const std::string& str );

    //! Checks whether character is numeric
    /*!
        \param str The target character
        \return The result of the check
        \see isNumeric()
    */
    static const bool isNumeric( const unsigned char& chr );

    //! Checks whether a string only contains alphabetic characters
    /*!
        \param str The target string
        \return The result of the check
    */
    static const bool isAlphabetic( const std::string& str );

    //! Checks whether a character is alphabetic
    /*!
        \param str The target character
        \return The result of the check
        \see isAlphabetic()
    */
    static const bool isAlphabetic( const unsigned char& chr );

    //! Checks whether a string only contains alpha-numeric characters
    /*!
        \param str The target string
        \return The result of the check
    */
    static const bool isAlnum( const std::string& str );

    //! Checks whether a character is alpha-numeric
    /*!
        \param str The target character
        \return The result of the check
        \see isAlnum
    */
    static const bool isAlnum( const unsigned char& chr );

    //! Checks whether a string only contains hexadecimal characters
    /*!
        \param str The target character
        \return The result of the check
    */
    static const bool isHex( const unsigned char& chr );

    //! Checks whether a string could be a valid IPv4/IPv6
    /*!
        \param str The target string
        \return The result of the check
    */
    static const bool isIP( const std::string& str );

    //! Finds the last occurrence of the given sequence in the given string
    /*!
        \param str The target string
        \param flag The sequence to search for
        \return The position of the last occurrence
    */
    static const size_t findLast( const std::string& str, const std::string& flag );

    //! Checks if a string starts with the given sequence
    /*!
        \param str The target string
        \param flag The sequence to search for
        \return The result of the check
    */
    static const bool startsWith( const std::string& str, const std::string& flag );

    //! Checks if a string ends with the given sequence
    /*!
        \param str The target string
        \param flag The sequence to search for
        \return The result of the check
    */
    static const bool endsWith(   const std::string& str, const std::string& flag );

    //! Checks if a string contains the given sequence
    /*!
        \param str The target string
        \param flag The sequence to search for
        \return The result of the check
    */
    static const bool contains( const std::string& str, const std::string& flag );

    //!  Strips the given characters from both the left and the right side of a string
    /*!
        \param str The target string
        \param chars The characters to strip away
        \return The result string
    */
    static std::string strip(  const std::string& str, const std::string& chars=" \n\t\b\r\v" );

    //!  Strips the given characters from the left side of a string
    /*!
        \param str The target string
        \param chars The characters to strip away
        \return The result string
    */
    static std::string lstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );

    //! Strips the given characters from the right side of a string
    /*!
        \param str The target string
        \param chars The characters to strip away
        \return The result string
    */
    static std::string rstrip( const std::string& str, const std::string& chars=" \n\t\b\r\v" );

    //! Strips everything from a string starting from the left side untill the delimiter is found (a.k.a. cut)
    /*!
        \param str The target string
        \param chr The delimiter
        \param inclusive Whether to also strip the delimiter or not
        \param consecutives Whether to strip all the occurrences of the delimiter if they're consecutive
        \return The result string
    */
    static std::string lstripUntil( const std::string& str, const std::string& chr, const bool& inclusive=true, const bool& consecutives=true );

    //! Splits a string using a separator
    /*!
        \param list Will hold the splitted content
        \param target_str The target string
        \param separator The sequence to use as separator
    */
    static void split( std::vector<std::string>& list, const std::string& target_str, const std::string& separator="\n" );

    //! Splits a string and strips all the splitted items
    /*!
        \param list Will hold the splitted content
        \param target_str The target string
        \param separator The sequence to use as separator
        \param strip The characters to strip away
    */
    static void splitrip( std::vector<std::string>& list, const std::string& target_str, const std::string& separator="\n", const std::string& strip=" \n\t\b\r\v" );

    //! Replaces all the occurrences of a sequence with another
    /*!
        \param str The target string
        \param target The sequence which will be replaced
        \param replace The sequence to be used to replace the target
        \return The result string
    */
    static const std::string replace( const std::string& str, const std::string& target, const std::string& replace );

    //! Converts a string to upper case
    /*!
        \param str The target string
        \return The result string
    */
    static const std::string toUpper( const std::string& str );

    //! Converts a string to lower case
    /*!
        \param str The target string
        \return The result string
    */
    static const std::string toLower( const std::string& str );

};

#endif // STRINGS_H

#ifndef LOGDOCTOR__UTILITIES__CHARS_H
#define LOGDOCTOR__UTILITIES__CHARS_H


//! CharOps
/*!
    Utilities for chars
*/
namespace CharOps
{

//! Checks whether a character is numeric
/*!
    \param chr The target character
    \return The result of the check
*/
inline bool isNumeric( const char& chr ) noexcept
{
    return chr > 47 && chr < 58; // 0-9
}

//! Checks whether a character is alphabetic
/*!
    \param chr The target character
    \return The result of the check
*/
inline bool isAlphabetic( const char& chr ) noexcept
{
    return (chr > 96 && chr < 123)  // a-z
        || (chr > 64 && chr <  91); // A-Z
}

//! Checks whether a character is alphanumeric
/*!
    \param chr The target character
    \return The result of the check
*/
inline bool isAlnum( const char& chr ) noexcept
{
    return isAlphabetic( chr )
        || isNumeric( chr );
}

//! Checks whether a character is hexadecimal
/*!
    \param chr The target character
    \return The result of the check
*/
inline bool isHex( const char& chr ) noexcept
{
    return (chr > 47 && chr < 58)   // 0-9
        || (chr > 64 && chr < 71)   // A-F
        || (chr > 96 && chr < 103); // a-f
}

//! Checks whether a character can be part of an IPv4/IPv6
/*!
    \param chr The target character
    \return The result of the check
*/
inline bool isIP( const char& chr ) noexcept
{
    return chr == 46 // .
        || chr == 58 // :
        || isHex( chr );
}

} // namespace CharOps


#endif // LOGDOCTOR__UTILITIES__CHARS_H

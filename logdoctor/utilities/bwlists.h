#ifndef LOGDOCTOR__UTILITIES__BWLISTS_H
#define LOGDOCTOR__UTILITIES__BWLISTS_H


#include <string>


//! BWutils
/*!
    Utilities for blacklists and warnlists
    \see Blasklists, Warnlists
*/
namespace BWutils
{

//! Returns a sanitized item which can be inserted in a list
/*!
    This fuction doesn't check if the method is actually
    a valid HTTP method, only whether the given string
    is sintattically acceptable.
    \throw BWlistException
*/
std::string sanitizedMethod( const std::string& item );

//! Returns a sanitized item which can be inserted in a list
/*!
    This function percent-encodes some of the characters
    in the provided string: /#&?=+
    \throw BWlistException
*/
std::string sanitizedUri( const std::string& item );

//! Returns a sanitized item which can be inserted in a list
/*!
    This functions doesn't check it the client is actually
    a valid IP address, only whether it is composed by the
    proper set of characters (for an IPv4 or an IPv6)
    \throw BWlistException
*/
std::string sanitizedClient( const std::string& item );

//! Returns a sanitized item which can be inserted in a list
/*!
    This function back-slashes every double-quotes in the
    provided string
    \throw BWlistException
*/
std::string sanitizedUserAgent( std::string_view item );

} // namespace BWutils


#endif // LOGDOCTOR__UTILITIES__BWLISTS_H

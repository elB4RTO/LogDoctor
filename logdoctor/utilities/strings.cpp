
#include "strings.h"

#include <algorithm>


const size_t StringOps::count( const std::string& str, const std::string& flag )
{
    const size_t flg_size{ flag.size() };
    size_t count{ 0 };
    for ( size_t start{0}; (start=str.find(flag, start)) != std::string::npos; count++ ) {
        start += flg_size;
    }
    return count;
}
const size_t StringOps::count( const std::string& str, const char flag )
{
    return std::count( str.cbegin(), str.cend(), flag );
}


const bool StringOps::isNumeric( const std::string& str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !StringOps::isNumeric( chr ); });
}
const bool StringOps::isNumeric( const char& chr )
{
    return chr > 47 && chr < 58;
}


const bool StringOps::isAlphabetic( const std::string& str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !StringOps::isAlphabetic( chr ); });
}
const bool StringOps::isAlphabetic( const char& chr )
{
    return (chr > 64 && chr < 91)
        || (chr > 96 && chr < 123);
}


const bool StringOps::isAlnum( const std::string& str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !StringOps::isAlnum( chr ); });
}
const bool StringOps::isAlnum( const char& chr )
{
    return StringOps::isNumeric( chr )
        || StringOps::isAlphabetic( chr );
}


const bool StringOps::isHex( const char& chr )
{
    return (chr > 47 && chr < 58)
        || (chr > 64 && chr < 71)
        || (chr > 96 && chr < 103);
}


const bool StringOps::isIP( const std::string& str )
{
    if ( str.empty() ) {
        return false;
    }
    for ( const char& chr : str ) {
        if ( chr == '.' || chr == ':' ) {
            continue;
        } else if ( ! StringOps::isHex( chr ) ) {
            return false;
        }
    }
    return true;
}


const bool StringOps::startsWith( const std::string& str, const std::string& flag )
{
    const size_t flg_size{ flag.size() };
    if ( flg_size > str.size() ) {
        return false;
    }
    for ( size_t i{0}; i<flg_size; i++ ) {
        if ( str.at(i) != flag.at(i) ) {
            return false;
        }
    }
    return true;
}


const bool StringOps::endsWith( const std::string& str, const std::string& flag )
{
    const size_t str_size{ str.size()-1  };
    const size_t flg_size{ flag.size()-1 };
    if ( flg_size > str_size ) {
        return false;
    }
    for ( size_t i{0}; i<=flg_size; i++ ) {
        if ( str.at( str_size-i ) != flag.at( flg_size-i ) ) {
            return false;
        }
    }
    return true;
}


const bool StringOps::contains( const std::string& str, const std::string& flag )
{
    return str.find( flag ) != std::string::npos;
}


std::string StringOps::strip( const std::string& str, const std::string& chars )
{
    return StringOps::rstrip( StringOps::lstrip( str, chars ), chars );
}


std::string StringOps::lstrip( const std::string& str, const std::string& chars )
{
    size_t i{ std::string::npos };
    const size_t max{ str.size() };
    char str_index;
    const auto inChars = [&str_index]( const char& chr )
                                     { return str_index == chr; };
    while ( ++i < max ) {
        str_index = str.at( i );
        if ( std::none_of( chars.cbegin(), chars.cend(), inChars) ) {
            return str.substr( i );
        }
    }
    return std::string{};
}


std::string StringOps::rstrip( const std::string& str, const std::string& chars )
{
    const size_t max{ str.size() };
    size_t i{ max };
    char str_index;
    const auto inChars = [&str_index]( const char& chr )
                                     { return str_index == chr; };
    while ( --i < max ) {
        str_index = str.at( i );
        if ( std::none_of( chars.cbegin(), chars.cend(), inChars) ) {
            return str.substr( 0, i+1 );
        }
    }
    return std::string{};
}


std::string StringOps::lstripUntil( const std::string& str, const std::string& delim, const bool& inclusive, const bool& consecutives )
{
    const size_t d_size{ delim.size() };
    size_t start{ str.find( delim ) };
    if ( start == std::string::npos ) {
        return str;
    }
    if ( inclusive ) {
        start += d_size;
        if ( consecutives ) {
            while ( str.find( delim, start ) == start ) {
                start += d_size;
            }
        }
    }
    return str.substr( start );
}



void StringOps::split( std::vector<std::string>& list, const std::string& target_str, const std::string& separator )
{
    if ( target_str.empty() ) {
        return;
    }
    list.reserve( StringOps::count( target_str, separator )+1 );
    const size_t trg_size{ target_str.size() };
    const size_t sep_size{ separator.size()  };
    size_t start{0}, stop;
    while ( (stop=target_str.find( separator, start )) != std::string::npos ) {
        if ( start < stop ) {
            list.push_back( target_str.substr( start, stop-start ) );
        }
        start = stop + sep_size;
    }
    if ( start < trg_size ) {
        list.push_back( target_str.substr( start ) );
    }
}


void StringOps::splitrip( std::vector<std::string>& list, const std::string& target_str, const std::string& separator, const std::string& strips )
{
    StringOps::split( list, StringOps::strip( target_str, strips ), separator );
    std::transform( list.begin(), list.end(), list.begin(),
                    [&strips]( const std::string& str )
                             { return StringOps::strip( str, strips ); } );
}


const std::string StringOps::replace( const std::string& str, const std::string& target, const std::string& replace )
{
    std::string s{ str };
    const size_t t_size{ target.size()  };
    const size_t r_size{ replace.size() };
    size_t start{ s.find( target ) };
    while ( start != std::string::npos ) {
        s.replace( start, t_size, replace );
        start = s.find( target, start+r_size );
    }
    return s;
}


const std::string StringOps::toUpper( const std::string& str )
{
    std::string up{ str };
    std::transform( up.begin(), up.end(), up.begin(),
                    []( const char& c )
                      { return std::toupper( c ); } );
    return up;
}

const std::string StringOps::toLower( const std::string& str )
{
    std::string low{ str };
    std::transform( low.begin(), low.end(), low.begin(),
                    []( const char& c )
                      { return std::tolower( c ); } );
    return low;
}

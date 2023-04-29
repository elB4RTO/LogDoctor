#include "strings.h"

#include <algorithm>


namespace StringOps
{

const size_t count( std::string_view str, const char flag )
{
    return static_cast<size_t>( std::count( str.cbegin(), str.cend(), flag ) );
}

const size_t count( std::string_view str, std::string_view flag )
{
    const size_t flg_size{ flag.size() };
    size_t count{ 0ul };
    for ( size_t start{0ul}; (start=str.find(flag, start)) != std::string::npos; count++ ) {
        start += flg_size;
    }
    return count;
}


const bool isNumeric( const char& chr )
{
    return chr > 47 && chr < 58;
}

const bool isNumeric( std::string_view str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !isNumeric( chr ); });
}


const bool isAlphabetic( const char& chr )
{
    return (chr > 64 && chr < 91)
        || (chr > 96 && chr < 123);
}

const bool isAlphabetic( std::string_view str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !isAlphabetic( chr ); });
}


const bool isAlnum( const char& chr )
{
    return isNumeric( chr )
        || isAlphabetic( chr );
}

const bool isAlnum( std::string_view str )
{
    if ( str.empty() ) {
        return false;
    }
    return !std::any_of( str.cbegin(), str.cend(),
                         []( const char& chr )
                           { return !isAlnum( chr ); });
}


const bool isHex( const char& chr )
{
    return (chr > 47 && chr < 58)
        || (chr > 64 && chr < 71)
        || (chr > 96 && chr < 103);
}


const bool isIP( std::string_view str )
{
    if ( str.empty() ) {
        return false;
    }
    for ( const char& chr : str ) {
        if ( chr == '.' || chr == ':' ) {
            continue;
        } else if ( ! isHex( chr ) ) {
            return false;
        }
    }
    return true;
}


const bool startsWith( std::string_view str, const char flag )
{
    return str.front() == flag;
}

const bool startsWith( std::string_view str, std::string_view flag )
{
    return str.find( flag ) == 0ul;
}


const bool endsWith( std::string_view str, const char flag )
{
    return str.back() == flag;
}

const bool endsWith( std::string_view str, std::string_view flag )
{
    return str.rfind( flag ) == str.size()-flag.size();
}


const bool contains( std::string_view str, std::string_view flag )
{
    return str.find( flag ) != std::string::npos;
}


const std::string strip( const std::string& str, const char chr )
{
    size_t start{ str.find_first_not_of( chr ) };
    size_t stop{ str.find_last_not_of( chr ) };
    if ( start == std::string::npos ) {
        start++;
    }
    if ( stop == std::string::npos ) {
        stop = str.size();
    }
    return str.substr( start, stop-start+1ul );
}

const std::string strip( const std::string& str, std::string_view chars )
{
    size_t start{ str.find_first_not_of( chars ) };
    size_t stop{ str.find_last_not_of( chars ) };
    if ( start == std::string::npos ) {
        start++;
    }
    if ( stop == std::string::npos ) {
        stop = str.size();
    }
    return str.substr( start, stop-start+1ul );
}


const std::string lstrip( const std::string& str, const char chr )
{
    const size_t start{ str.find_first_not_of( chr ) };
    if ( start != std::string::npos ) {
        return str.substr( start );
    }
    return std::string{};
}

const std::string lstrip( const std::string& str, std::string_view chars )
{
    const size_t start{ str.find_first_not_of( chars ) };
    if ( start != std::string::npos ) {
        return str.substr( start );
    }
    return std::string{};
}


const std::string rstrip( const std::string &str, const char chr )
{
    const size_t stop{ str.find_last_not_of( chr ) };
    if ( stop != std::string::npos ) {
        return str.substr( 0ul, stop+1ul );
    }
    return std::string{};
}

const std::string rstrip( const std::string& str, std::string_view chars )
{
    const size_t stop{ str.find_last_not_of( chars ) };
    if ( stop != std::string::npos ) {
        return str.substr( 0ul, stop+1ul );
    }
    return std::string{};
}


const std::string lstripUntil( const std::string& str, const char delim, const bool inclusive, const bool consecutives )
{
    size_t start{ str.find( delim ) };
    if ( start == std::string::npos ) {
        return str;
    }
    if ( inclusive ) {
        start ++;
        if ( consecutives ) {
            while ( str.find( delim, start ) == start ) {
                start ++;
            }
        }
    }
    return str.substr( start );
}


void split( std::vector<std::string>& list, const std::string& target_str, const char separator )
{
    if ( target_str.empty() ) {
        return;
    }
    list.reserve( count( target_str, separator )+1ul );
    size_t start{0ul}, stop;
    while ( (stop=target_str.find( separator, start )) != std::string::npos ) {
        if ( start < stop ) {
            list.push_back( target_str.substr( start, stop-start ) );
        }
        start = stop + 1ul;
    }
    if ( start != std::string::npos ) {
        list.push_back( target_str.substr( start ) );
    }
}

void split( std::vector<std::string>& list, const std::string& target_str, std::string_view separator )
{
    if ( target_str.empty() ) {
        return;
    }
    list.reserve( count( target_str, separator )+1ul );
    const size_t trg_size{ target_str.size() };
    const size_t sep_size{ separator.size()  };
    size_t start{0ul}, stop;
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


void splitrip( std::vector<std::string>& list, const std::string& target_str, const char separator, std::string_view strips )
{
    split( list, strip( target_str, strips ), separator );
    std::transform( list.begin(), list.end(), list.begin(),
                    [&strips]( const std::string& str )
                             { return strip( str, strips ); } );
}

void splitrip( std::vector<std::string>& list, const std::string& target_str, std::string_view separator, std::string_view strips )
{
    split( list, strip( target_str, strips ), separator );
    std::transform( list.begin(), list.end(), list.begin(),
                    [&strips]( const std::string& str )
                             { return strip( str, strips ); } );
}


const std::string replace( std::string_view str, std::string_view target, std::string_view replace )
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


const std::string toUpper( std::string_view str )
{
    std::string up{ str };
    std::transform( up.begin(), up.end(), up.begin(),
                    []( const char& c )
                      { return std::toupper( c ); } );
    return up;
}


const std::string toLower( std::string_view str )
{
    std::string low{ str };
    std::transform( low.begin(), low.end(), low.begin(),
                    []( const char& c )
                      { return std::tolower( c ); } );
    return low;
}

} // namespace StringOps

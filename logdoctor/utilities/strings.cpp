
#include "strings.h"

#include <QStringView>


namespace StringOps
{

std::size_t count( std::string_view str, std::string_view flag ) noexcept
{
    const std::size_t flg_size{ flag.size() };
    std::size_t count{ 0ul };
    for ( std::size_t start{0ul}; (start=str.find(flag, start)) != std::string::npos; ++count ) {
        start += flg_size;
    }
    return count;
}


bool isNumeric( QStringView str ) noexcept
{
    return str.empty()
         ? false
         : std::all_of( str.cbegin(), str.cend(), [](const QChar c){ return c.isDigit(); } );
}


std::string strip( const std::string& str, const char chr ) noexcept
{
    if (const std::size_t start{ str.find_first_not_of( chr ) }; start != std::string::npos ) {
        const std::size_t stop{ str.find_last_not_of( chr ) };
        return str.substr( start, stop-start+1ul );
    }
    return std::string{};
}

std::string strip( const std::string& str, const char* chars ) noexcept
{
    if (const std::size_t start{ str.find_first_not_of( chars ) }; start != std::string::npos ) {
        const std::size_t stop{ str.find_last_not_of( chars ) };
        return str.substr( start, stop-start+1ul );
    }
    return std::string{};
}


std::string lstrip( const std::string& str, const char chr ) noexcept
{
    if (const std::size_t start{ str.find_first_not_of( chr ) }; start != std::string::npos ) {
        return str.substr( start );
    }
    return std::string{};
}

std::string lstrip( const std::string& str, const char* chars ) noexcept
{
    if (const std::size_t start{ str.find_first_not_of( chars ) }; start != std::string::npos ) {
        return str.substr( start );
    }
    return std::string{};
}


std::string rstrip( const std::string &str, const char chr ) noexcept
{
    if (const std::size_t stop{ str.find_last_not_of( chr ) }; stop != std::string::npos ) {
        return str.substr( 0ul, stop+1ul );
    }
    return std::string{};
}

std::string rstrip( const std::string& str, const char* chars ) noexcept
{
    if (const std::size_t stop{ str.find_last_not_of( chars ) }; stop != std::string::npos ) {
        return str.substr( 0ul, stop+1ul );
    }
    return std::string{};
}


std::string lstripUntil( const std::string& str, const char delim, const bool inclusive, const bool consecutives ) noexcept
{
    if (std::size_t start{ str.find( delim ) }; start != std::string::npos ) {
        if ( inclusive ) {
            ++ start;
            if ( consecutives ) {
                if (str[start] == delim) {
                    start = str.find_first_not_of( delim, start );
                    if ( start == std::string::npos ) {
                        return std::string{};
                    }
                }
            }
        }
        return str.substr( start );
    }
    return str;
}


void split( std::vector<std::string>& list, const std::string& target_str, const char separator ) noexcept
{
    if ( target_str.empty() ) {
        return;
    }
    list.reserve( count( target_str, separator )+1ul );
    std::size_t start{0ul}, stop;
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

void split( std::vector<std::string>& list, const std::string& target_str, std::string_view separator ) noexcept
{
    if ( target_str.empty() ) {
        return;
    }
    list.reserve( count( target_str, separator )+1ul );
    const std::size_t trg_size{ target_str.size() };
    const std::size_t sep_size{ separator.size()  };
    std::size_t start{0ul}, stop;
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


void splitrip( std::vector<std::string>& list, const std::string& target_str, const char separator, const char* strips ) noexcept
{
    split( list, strip( target_str, strips ), separator );
    std::transform( list.begin(), list.end(), list.begin(),
                    [&strips]( const std::string& str )
                             { return strip( str, strips ); } );
}

void splitrip(std::vector<std::string>& list, const std::string& target_str, const char* separator, const char* strips ) noexcept
{
    split( list, strip( target_str, strips ), separator );
    std::transform( list.begin(), list.end(), list.begin(),
                    [strips]( const std::string& str )
                             { return strip( str, strips ); } );
}


std::string replace( std::string_view str, std::string_view target, std::string_view replace ) noexcept
{
    std::string s{ str };
    const std::size_t t_size{ target.size()  };
    const std::size_t r_size{ replace.size() };
    std::size_t start{ s.find( target ) };
    while ( start != std::string::npos ) {
        s.replace( start, t_size, replace );
        start = s.find( target, start+r_size );
    }
    return s;
}


std::string toUpper( std::string_view str ) noexcept
{
    std::string up{ str };
    std::transform( up.begin(), up.end(), up.begin(),
                    []( const char c )
                      { return std::toupper( c ); } );
    return up;
}


std::string toLower( std::string_view str ) noexcept
{
    std::string low{ str };
    std::transform( low.begin(), low.end(), low.begin(),
                    []( const char c )
                      { return std::tolower( c ); } );
    return low;
}

} // namespace StringOps

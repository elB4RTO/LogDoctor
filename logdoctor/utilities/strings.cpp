
#include "strings.h"


StringOps::StringOps()
{

}


const int StringOps::count(const std::string& str, const std::string& flag, const bool& consecutives )
{
    size_t start=0, aux_start=0, count=0;
    while (true) {
        start = str.find( flag, start );
        if ( start != std::string::npos ) {
            if ( consecutives == true
              && start == aux_start ) {
                start += flag.size();
                aux_start = start;
                continue;
            }
            count ++;
            start += flag.size();
            aux_start = start;
        } else {
            break;
        }
    }
    return count;
}


const bool StringOps::isNumeric( const std::string& str )
{
    bool result = false;
    if ( str.size() > 0 ) {
        result = true;
        for ( const unsigned char& chr : str ) {
            if ( ! StringOps::isNumeric( chr ) ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isNumeric(const unsigned char& chr )
{
    if ( chr > 47 && chr < 58 ) {
        return true;
    } else {
        return false;
    }
}


const bool StringOps::isAlphabetic( const std::string& str )
{
    bool result = false;
    if ( str.size() > 0 ) {
        result = true;
        for ( const unsigned char& chr : str ) {
            if ( ! StringOps::isAlphabetic( chr ) ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isAlphabetic(const unsigned char& chr )
{
    if ( (chr > 64 && chr < 91)
      || (chr > 96 && chr < 123) ) {
        return true;
    } else {
        return false;
    }
}


const bool StringOps::isAlnum( const std::string& str )
{
    bool result = false;
    if ( str.size() > 0 ) {
        result = true;
        for ( const unsigned char& chr : str ) {
            if ( ! StringOps::isAlnum( chr ) ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isAlnum(const unsigned char& chr )
{
    if ( !StringOps::isNumeric( chr )
      && !StringOps::isAlphabetic( chr ) ) {
        return false;
    } else {
        return true;
    }
}


const bool StringOps::isHex( const unsigned char& chr )
{
    if ( (chr > 47 && chr < 58)
      || (chr > 64 && chr < 71)
      || (chr > 96 && chr < 103) ) {
        return true;
    } else {
        return false;
    }
}


const bool StringOps::isIP( const std::string& str )
{
    bool result = false;
    if ( str.size() > 0 ) {
        result = true;
        for ( const unsigned char& chr : str ) {
            if ( chr == '.' || chr == ':' ) {
                continue;
            } else if ( ! StringOps::isHex( chr ) ) {
                result = false;
                break;
            }
        }
    }
    return result;
}


const size_t StringOps::findLast( const std::string& str, const std::string& flag )
{
    int n=0;
    size_t index, aux=0;
    const size_t f_size=flag.size();
    do {
        index = (n>0) ? aux : -f_size;
        aux = str.find( flag, index+f_size );
        n++;

    } while ( aux != std::string::npos );

    if ( index == -f_size ) {
        index = std::string::npos;
    }

    return index;
}


const bool StringOps::startsWith( const std::string& str, const std::string& flag )
{
    bool result = false;
    if ( flag.size() == 0 ) {
        result = true;
    } else if ( flag.size() <= str.size() ) {
        result = true;
        for ( int i=0; i<flag.size(); i++ ) {
            if ( str.at(i) != flag.at(i) ) {
                result = false;
                break;
            }
        }
    }
    return result;
}


const bool StringOps::endsWith( const std::string& str, const std::string& flag )
{
    bool result = true;
    const int str_size = str.size()-1,
              flg_size = flag.size()-1;
    for ( int i=0; i<flg_size; i++ ) {
        if ( str.at( str_size-i ) != flag.at( flg_size-i ) ) {
            result = false;
            break;
        }
    }
    return result;
}


const bool StringOps::contains( const std::string& str, const std::string& flag )
{
    bool result = true;
    size_t i = str.find( flag );
    if ( i == std::string::npos ) {
        result = false;
    }
    return result;
}


std::string StringOps::strip( const std::string& str, const std::string& chars )
{
    std::string stripped;
    stripped = StringOps::lstrip( str, chars );
    stripped = StringOps::rstrip( stripped, chars );
    return stripped;
}


std::string StringOps::lstrip( const std::string& str, const std::string& chars )
{
    bool found;
    int i = 0;
    const int max = str.size();
    while ( i < max ) {
        found = false;
        char str_index = str.at( i );
        for ( const char& chr : chars ) {
            if ( str_index == chr ) {
                found = true;
                break;
            }
        }
        if ( ! found ) {
            break;
        }
        i++;
    }
    std::string stripped = "";
    if ( i < str.size() ) {
        stripped = str.substr( i );
    }
    return stripped;
}


std::string StringOps::rstrip( const std::string& str, const std::string& chars )
{
    bool found;
    int i = str.size() - 1;
    while ( i >= 0 ) {
        found = false;
        char str_index = str.at( i );
        for ( const char& chr : chars ) {
            if ( str_index == chr ) {
                found = true;
                break;
            }
        }
        if ( ! found ) {
            break;
        }
        i--;
    }
    std::string stripped = "";
    if ( i >= 0 ) {
        stripped = str.substr( 0, str.size() - (str.size() - i) + 1 );
    }
    return stripped;
}


std::string StringOps::lstripUntil( const std::string& str, const std::string& chr, const bool& inclusive, const bool& consecutives )
{
    size_t start, aux_start, aux;
    const size_t max_size = str.size()-1;
    std::string stripped = "";
    if ( max_size >= 0 ) {
        start = str.find( chr );
        if ( inclusive == true ) {
            start += chr.size();
        }
        if ( consecutives == true
          && start != std::string::npos ) {
            aux_start = start;
            while (true) {
                aux = str.find( chr, aux_start );
                if ( aux == std::string::npos ) {
                    break;
                } else if ( aux != aux_start ) {
                    break;
                }
                if ( inclusive == true ) {
                    aux += chr.size();
                }
                aux_start = aux;
            }
            if ( aux_start == max_size+1 ) {
                stripped = "";
            } else {
                stripped = str.substr( aux_start );
            }
        } else {
            if ( start == std::string::npos ) {
                stripped = str;
            } else if ( start == max_size+1 ) {
                stripped = "";
            } else {
                stripped = str.substr( start );
            }
        }
    }
    return stripped;
}



void StringOps::split( std::vector<std::string>& list, const std::string& target_str, const std::string& separator )
{
    std::string slice;
    size_t start=0, stop;
    if ( target_str.size()-1 >= 0 ) {
        while (true) {
            stop = target_str.find( separator, start );
            if ( stop == std::string::npos ) {
                slice = target_str.substr( start );
                if ( slice.size() > 0 ) {
                    list.push_back( slice );
                }
                break;
            } else {
                slice = target_str.substr( start, stop-start );
                if ( slice.size() > 0 ) {
                    list.push_back( slice );
                }
                start = stop+separator.size();
            }
        }
    }
}


void StringOps::splitrip( std::vector<std::string>& list, const std::string& target_str, const std::string& separator, const std::string& strip )
{
    std::vector<std::string> aux;
    const std::string str_ = StringOps::strip( target_str, strip );
    StringOps::split( aux, str_, separator );
    for ( const std::string& str : aux ) {
        const std::string str_aux = StringOps::strip( str, strip );
        if ( str_aux.size() == 0 ) {
            continue;
        }
        list.push_back( str_aux );
    }
    aux.clear();
}


const std::string StringOps::replace( const std::string& str, const std::string& target, const std::string& replace )
{
    size_t start=0, stop;
    const int size = target.size();
    std::string string = "";
    if ( str.size()-1 >= 0 ) {
        while ( true ) {
            stop = str.find( target, start );
            if ( stop == std::string::npos ) {
                string += str.substr( start );
                break;
            } else {
                string += str.substr( start, stop-start );
                string += replace;
                start = stop+size;
            }
        }
    }
    return string;
}


const std::string StringOps::toUpper( const std::string& str )
{
    std::string up = "";
    for ( const unsigned char& ch : str ) {
        up.push_back( (char)std::toupper( ch ) );
    }
    return up;
}

const std::string StringOps::toLower( const std::string& str )
{
    std::string low = "";
    for ( const unsigned char& ch : str ) {
        low.push_back( (char)std::tolower( ch ) );
    }
    return low;
}

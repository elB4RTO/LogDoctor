
#include "strings.h"


StringOps::StringOps()
{

}


const int StringOps::count(const std::string& str, const std::string& flag, const bool& consecutives )
{
    int start=0, aux_start=0, count=0;
    const int max = str.size()-1;
    while (true) {
        start = str.find( flag, start );
        if ( start >= 0 && start < max ) {
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
        for ( const char& chr : str ) {
            if ( StringOps::isNumeric( chr ) == false ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isNumeric( const char& chr )
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
        for ( const char& chr : str ) {
            if ( StringOps::isAlphabetic( chr ) == false ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isAlphabetic( const char& chr )
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
        for ( const char& chr : str ) {
            if ( StringOps::isAlnum( chr ) == false ) {
                result = false;
                break;
            }
        }
    }
    return result;
}
const bool StringOps::isAlnum( const char& chr )
{
    if ( StringOps::isNumeric( chr ) == false
      && StringOps::isAlphabetic( chr ) == false ) {
        return false;
    } else {
        return true;
    }
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
    int i = str.find( flag );
    if ( i < 0 || i > str.size() ) {
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
        if ( found == false ) {
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
        if ( found == false ) {
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
    int start, aux_start, aux;
    const int max_size = str.size()-1;
    std::string stripped = "";
    if ( max_size >= 0 ) {
        start = str.find( chr );
        if ( inclusive == true ) {
            start += chr.size();
        }
        if ( consecutives == true
          && start >= 0 && start <= max_size ) {
            aux_start = start;
            while (true) {
                aux = str.find( chr, aux_start );
                if ( aux < 0 || aux > max_size ) {
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
            if ( start < 0 || start > max_size+1 ) {
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
    int start=0, stop;
    const int max = target_str.size()-1;
    if ( max >= 0 ) {
        while (true) {
            stop = target_str.find( separator, start );
            if ( stop < 0 || stop > max ) {
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
        if ( str.size() == 0 ) {
            continue;
        }
        list.push_back( StringOps::strip( str, strip ) );
    }
    aux.clear();
}


std::string StringOps::replace( const std::string& str, const std::string& target, const std::string& replace )
{
    int start=0, stop;
    const int max = str.size()-1;
    std::string string = "";
    if ( max >= 0 ) {
        while ( true ) {
            stop = str.find_first_of( target, start );
            if ( stop < 0 || stop > max ) {
                string += str.substr( start );
                break;
            } else {
                string += str.substr( start, stop-start );
                string += replace;
                start = stop+1;
            }
        }
    }
    return string;
}

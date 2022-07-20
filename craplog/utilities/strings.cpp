
#include "strings.h"


StringOps::StringOps()
{

}


int StringOps::count(const std::string& str, const std::string& flag, bool consecutives )
{
    int start=0, aux_start=0, max=str.size()-1, count=0;
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


bool StringOps::isNumeric( const std::string& str )
{
    bool result = true;
    for ( const char& chr : str ) {
        if ( StringOps::isNumeric( chr ) == false ) {
            result = false;

        }
    }
    return result;
}


bool StringOps::isNumeric( char chr )
{
    if ( chr > 47 && chr < 58 ) {
        return true;
    } else {
        return false;
    }
}


bool StringOps::startsWith( const std::string& str, const std::string& flag )
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


bool StringOps::endsWith( const std::string& str, const std::string& flag )
{
    bool result = true;
    int str_size = str.size()-1,
        flg_size = flag.size()-1;
    for ( int i=0; i<flg_size; i++ ) {
        if ( str.at( str_size-i ) != flag.at( flg_size-i ) ) {
            result = false;
            break;
        }
    }
    return result;
}


bool StringOps::contains( const std::string& str, const std::string& flag )
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
    bool found = true;
    int i = 0;
    while ( i < str.size() ) {
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
    bool found = true;
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
    if ( i > 0 ) {
        stripped = str.substr( 0, str.size() - (str.size() - i) + 1 );
    }
    return stripped;
}


std::string StringOps::lstripUntil( const std::string& str, const std::string& chr, bool inclusive, bool consecutives )
{
    int start, aux_start, aux;
    int max_size = str.size()-1;
    std::string stripped = "";
    if ( str != "" ) {
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



std::vector<std::string> StringOps::split( const std::string& str, const std::string& sep )
{
    std::vector<std::string> splitted;
    std::string slice;
    int start=0, stop;
    while (true) {
        stop = str.find( sep, start );
        if ( stop >= str.size() ) {
            slice = str.substr( start );
            if ( slice.size() > 0 ) {
                splitted.push_back( slice );
            }
            break;
        } else {
            slice = str.substr( start, stop-start );
            if ( slice.size() > 0 ) {
                splitted.push_back( slice );
            }
            start = stop+sep.size();
        }
    }
    return splitted;
}


std::vector<std::string> StringOps::splitrip( const std::string& str, const std::string& sep, const std::string& chars )
{
    std::vector<std::string> splitted, aux;
    const std::string str_ = StringOps::strip( str );
    aux = StringOps::split( str_ );
    for ( const std::string& str_ : aux ) {
        if ( str_.size() == 0 ) {
            continue;
        }
        splitted.push_back( StringOps::strip( str_ ) );
    }
    return splitted;
}

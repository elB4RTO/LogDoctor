
#include "strings.h"

using std::string, std::vector;


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


bool StringOps::isNumeric( const string& str )
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


bool StringOps::startsWith( const string& str, const string& flag)
{
    bool result = false;
    if ( flag.size() <= str.size() ) {
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


bool StringOps::endsWith( const string& str, const string& flag )
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


bool StringOps::contains( const string& str, const string& flag )
{
    bool result = true;
    int i = str.find( flag );
    if ( i < 0 || i > str.size() ) {
        result = false;
    }
    return result;
}


string StringOps::strip( const string& str, const string& chars )
{
    string stripped;
    stripped = StringOps::lstrip( str, chars );
    stripped = StringOps::rstrip( stripped, chars );
    return stripped;
}


string StringOps::lstrip( const string& str, const string& chars )
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
    string stripped = "";
    if ( i < str.size() ) {
        stripped = str.substr( i );
    }
    return stripped;
}


string StringOps::rstrip( const string& str, const string& chars )
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
    string stripped = "";
    if ( i > 0 ) {
        stripped = str.substr( 0, str.size() - (str.size() - i) + 1 );
    }
    return stripped;
}


string StringOps::lstripUntil( const string& str, const string& chr, bool inclusive, bool consecutives )
{
    int start, aux_start, aux;
    int max_size = str.size()-1;
    string stripped = "";
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



vector<string> StringOps::split( const string& str, const string& sep )
{
    vector<string> splitted;
    string slice;
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


vector<string> StringOps::splitrip( const string& str, const string& sep, const string& chars )
{
    vector<string> splitted, aux;
    const string str_ = StringOps::strip( str );
    aux = StringOps::split( str_ );
    for ( const string& str_ : aux ) {
        if ( str_.size() == 0 ) {
            continue;
        }
        splitted.push_back( StringOps::strip( str_ ) );
    }
    return splitted;
}

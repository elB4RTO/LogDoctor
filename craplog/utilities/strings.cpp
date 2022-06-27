
#include "strings.h"

using std::string, std::vector;


StringOps::StringOps()
{

}


bool StringOps::isNumeric( string str )
{
    bool result = true;
    for ( char& chr : str ) {
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


bool StringOps::startsWith(string str, string flag)
{
    bool result = true;
    for ( int i=0; i<flag.size(); i++ ) {
        if ( str[i] != flag[i] ) {
            result = false;
            break;
        }
    }
    return result;
}


bool StringOps::endsWith( string str, string flag )
{
    bool result = true;
    int str_size = str.size()-1,
        flg_size = flag.size()-1;
    for ( int i=0; i<flg_size; i++ ) {
        if ( str[str_size-i] != flag[flg_size-i] ) {
            result = false;
            break;
        }
    }
    return result;
}


string StringOps::strip( string str, string chars )
{
    string stripped;
    stripped = StringOps::lstrip( str, chars );
    stripped = StringOps::rstrip( stripped, chars );
    return stripped;
}


string StringOps::lstrip( string str, string chars )
{
    bool found = true;
    int i = 0;
    while ( i < str.size() ) {
        found = false;
        char str_index = str[i];
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


string StringOps::rstrip( string str, string chars )
{
    bool found = true;
    int i = str.size() - 1;
    while ( i >= 0 ) {
        found = false;
        char str_index = str[i];
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



vector<string> StringOps::split( string str, string sep )
{
    vector<string> splitted;
    string slice;
    int start=0, stop=0;
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


vector<string> StringOps::splitrip( string str, string sep, string chars )
{
    vector<string> splitted, aux;
    str = StringOps::strip( str );
    aux = StringOps::split( str );
    for ( string &str : aux ) {
        if ( str.size() == 0 ) {
            continue;
        }
        splitted.push_back( StringOps::strip( str ) );
    }
    return splitted;
}

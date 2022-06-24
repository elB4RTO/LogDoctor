
#include "strings.h"


StringOps::StringOps()
{

}


bool StringOps::isNumeric( std::string str )
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


bool StringOps::startsWith(std::string str, std::string flag)
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


bool StringOps::endsWith(std::string str, std::string flag)
{
    bool result = true;
    int size = str.size()-1;
    for ( int i=size; i>size-flag.size(); i-- ) {
        if ( str[i] != flag[i] ) {
            result = false;
            break;
        }
    }
    return result;
}


std::string StringOps::strip( std::string str, std::string chars )
{
    std::string stripped;
    stripped = StringOps::lstrip( str, chars );
    stripped = StringOps::rstrip( stripped, chars );
    return stripped;
}


std::string StringOps::lstrip( std::string str, std::string chars )
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
    std::string stripped = "";
    if ( i < str.size() ) {
        stripped = str.substr( i );
    }
    return stripped;
}


std::string StringOps::rstrip( std::string str, std::string chars )
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
    std::string stripped = "";
    if ( i > 0 ) {
        stripped = str.substr( 0, str.size() - (str.size() - i) + 1 );
    }
    return stripped;
}



std::vector<std::string> StringOps::split( std::string str, std::string sep )
{
    std::vector<std::string> splitted;
    std::string slice;
    int start=0, stop=0;
    while (true) {
        stop = str.find( sep );
        if ( stop >= str.size() ) {
            slice = str.substr( start );
            if ( slice.empty() == false ) {
                splitted.push_back( slice );
            }
            break;
        } else {
            slice = str.substr( start, stop-start );
            if ( slice.empty() == false ) {
                splitted.push_back( slice );
            }
            start = stop+sep.size();
        }
    }
    return splitted;
}


std::vector<std::string> StringOps::splitrip( std::string str, std::string sep, std::string chars )
{
    std::vector<std::string> splitted, aux;
    str = StringOps::strip( str );
    aux = StringOps::split( str );
    for ( std::string &str : aux ) {
        if ( str.empty() ) {
            continue;
        }
        splitted.push_back( StringOps::strip( str ) );
    }
    return splitted;
}

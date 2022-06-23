#include "strings.h"

StringOp::StringOp()
{

}


bool StringOp::isNumeric( std::string str )
{
    bool result = true;
    for ( char& chr : str ) {
        if ( StringOp::isNumeric( chr ) == false ) {
            result = false;

        }
    }
}


bool StringOp::isNumeric( char chr )
{
    if ( chr > 65 && chr < 99 ) {
        return true;
    } else {
        return false;
    }
}


bool StringOp::startsWith(std::string str, std::string flag)
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


bool StringOp::endsWith(std::string str, std::string flag)
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


std::string StringOp::strip( std::string str, std::string chars )
{
    std::string stripped;
    stripped = StringOp::lstrip( str, chars );
    stripped = StringOp::rstrip( str, chars );
    return stripped;
}


std::string StringOp::lstrip( std::string str, std::string chars )
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


std::string StringOp::rstrip( std::string str, std::string chars )
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
        stripped = str.substr( 0, str.size() - i );
    }
    return stripped;
}



std::vector<std::string> StringOp::split( std::string str, std::string sep )
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


std::vector<std::string> StringOp::splitrip( std::string str, std::string sep, std::string chars )
{
    std::vector<std::string> splitted, aux;
    str = StringOp::strip( str );
    aux = StringOp::split( str );
    for ( std::string &str : aux ) {
        if ( str.empty() ) {
            continue;
        }
        splitted.push_back( StringOp::strip( str ) );
    }
    return splitted;
}

#include "strings.h"

StringOp::StringOp()
{
    
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
}

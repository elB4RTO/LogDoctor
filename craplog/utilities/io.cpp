
#include "io.h"

#include "utilities/strings.h"

#include <fstream>
#include <filesystem>

using std::vector, std::string;


IOutils::IOutils()
{

}

// test the existence of a file/folder
bool IOutils::exists( string path )
{
    return std::filesystem::exists( path );
}

// tests if a path exists and points to a file
bool IOutils::isFile( string path )
{
    if ( std::filesystem::exists( path )) {
        return std::filesystem::is_regular_file( path );
    }
}
// returns whether a file is readable
bool IOutils::checkFile( string path, bool readable, bool writable )
{
    bool result = false;
    if ( IOutils::isFile( path ) == true ) {
        result = true;
        // check the needed permissions
        auto perms = std::filesystem::status( path ).permissions();
        if ( readable == true ) {
            if ( (perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none ) {
                result = false;
            }
        }
        if ( writable == true ) {
            if ( (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none ) {
                result = false;
            }
        }
    }
    return result;
}

// test if a path exists and points to a folder
bool IOutils::isDir( string path )
{
    if ( std::filesystem::exists( path )) {
        return std::filesystem::is_directory( path );
    }
}
// returns whether a file is readable
bool IOutils::checkDir( string path, bool readable, bool writable )
{
    bool result = false;
    if ( IOutils::isDir( path ) == true ) {
        result = true;
        // check the needed permissions
        auto perms = std::filesystem::status( path ).permissions();
        if ( readable == true ) {
            if ( (perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none ) {
                result = false;
            }
        }
        if ( writable == true ) {
            if ( (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none ) {
                result = false;
            }
        }
    }
    return result;
}



vector<string> IOutils::readLines( string path, int n_lines )
{
    // read rhe first line only
    std::ifstream file;
    vector<string> lines;
    string line;
    try {
        constexpr std::size_t read_size = std::size_t(4096);
        file = std::ifstream(path);
        if ( file.is_open() == false ) {
            throw std::ios_base::failure( "file is not open" );
        }
        // add bit exceptions
        file.exceptions(std::ifstream::failbit);
        file.exceptions(std::ios_base::badbit);
        // get non-empty lines
        int n=0;
        while (n < n_lines) {
            if ( file.good() == false ) {
                throw std::ios_base::failure( "file is not good" );
            }
            getline(file, line);
            if ( line.empty() ) {
              continue;
            }
            // succesfully catched a line
            lines.push_back( StringOps::strip( line ) );
            n++;
        }
    } catch (const std::ios_base::failure& err) {
        // failed reading
        // >> err.what() << // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    }
    if ( file.is_open() ) {
        file.close();
    }
    return lines;
}


string IOutils::readFile( string path )
{
    // read the whole file
    std::ifstream file;
    string content = string();
    try {
        constexpr std::size_t read_size = std::size_t(4096);
        std::ifstream file(path);
        if ( file.is_open() == false ) {
            throw std::ios_base::failure( "file is not open" );
        }
        if ( file.good() == false ) {
            throw std::ios_base::failure( "file is not good" );
        }
        // add bit exceptions
        file.exceptions(std::ifstream::failbit);
        file.exceptions(std::ios_base::badbit);
        // read the whole file
        content = string(
            (std::istreambuf_iterator<char>( file )),
            std::istreambuf_iterator<char>() );
    } catch (const std::ios_base::failure& err) {
        // failed reading
        content = "";
        // >> err.what() << // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    }
    if ( file.is_open() ) {
        file.close();
    }
    return content;
}

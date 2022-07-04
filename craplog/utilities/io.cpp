
#include "io.h"

#include "utilities/strings.h"

#include <fstream>
#include <filesystem>


IOutils::IOutils()
{

}

// test the existence of a file/folder
bool IOutils::exists( const std::string& path )
{
    return std::filesystem::exists( path );
}

// tests if a path exists and points to a file
bool IOutils::isFile( const std::string& path )
{
    bool result = false;
    if ( std::filesystem::exists( path )) {
        result = std::filesystem::is_regular_file( path );
    }
    return result;
}
// returns whether a file is readable
bool IOutils::checkFile( const std::string& path, const bool readable, const bool writable )
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
bool IOutils::isDir( const std::string& path )
{
    bool result = false;
    if ( std::filesystem::exists( path )) {
        result = std::filesystem::is_directory( path );
    }
    return result;
}
// returns whether a file is readable
bool IOutils::checkDir( const std::string& path, const bool readable, const bool writable )
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



std::vector<std::string> IOutils::readLines( const std::string& path, const int n_lines, const bool strip_lines ) throw()
{
    // read rhe first line only
    bool result = true;
    std::ifstream file;
    std::vector<std::string> lines;
    std::string line;
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
                // hopefully nothing more to read
                break;
            }
            getline(file, line);
            if ( strip_lines == true ) {
                line = StringOps::strip( line );
            }
            if ( line.size() == 0 ) {
              continue;
            }
            // succesfully catched a line
            lines.push_back( line );
            n++;
        }
    } catch (const std::ios_base::failure& err) {
        // failed reading
        lines.clear();
        result = false;
        // >> err.what() << // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    } catch (...) {
        lines.clear();
        result = false;
    }

    if ( file.is_open() ) {
        file.close();
    }
    return lines;
}


std::string IOutils::readFile( const std::string& path ) throw()
{
    // read the whole file
    std::ifstream file;
    std::string content = std::string();
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
        content = std::string(
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

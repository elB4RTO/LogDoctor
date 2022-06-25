#include "io.h"

#include "utilities/strings.h"


IOutils::IOutils()
{

}


std::vector<std::string> IOutils::readLines( std::string path, int n_lines )
{
    // read rhe first line only
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


std::string IOutils::readFile( std::string path )
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

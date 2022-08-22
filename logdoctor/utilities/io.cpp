
#include "io.h"

#include "modules/exceptions.h"

#include "utilities/gzip.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include <fstream>
#include <filesystem>


IOutils::IOutils()
{

}

// test the existence of a file/folder
const bool IOutils::exists( const std::string& path )
{
    if ( path.size() > 0 ) {
        return std::filesystem::exists( path );
    } else {
        return false;
    }
}

// tests if a path exists and points to a file
const bool IOutils::isFile( const std::string& path )
{
    bool result = false;
    if ( std::filesystem::exists( path )) {
        result = std::filesystem::is_regular_file( path );
    }
    return result;
}
// returns whether a file is readable/writable
const bool IOutils::checkFile( const std::string& path, const bool& readable, const bool& writable )
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
const bool IOutils::isDir( const std::string& path )
{
    bool result = false;
    if ( std::filesystem::exists( path )) {
        result = std::filesystem::is_directory( path );
    }
    return result;
}
// returns whether a folder is readable/writable
const bool IOutils::checkDir( const std::string& path, const bool& readable, const bool& writable )
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


// create a directory
const bool IOutils::makeDir( const std::string& path ) noexcept(true)
{
    bool result = true;
    try {
        result = std::filesystem::create_directory( path );
    } catch (...) {
        result = false;
    }
    return result;
}


// rename an entry with a trailing '.copy'
const bool IOutils::renameAsCopy( const std::string& path ) noexcept(true)
{
    bool result = true;
    try {
        std::string new_path = path;
        // loop until a valid name is found
        while (true) {
            new_path += ".copy";
            if ( IOutils::exists( new_path ) == false ) {
                // available name found
                break;
            }
        }
        std::filesystem::rename( path, new_path );
    } catch (...) {
        result = false;
    }
    return result;
}



void IOutils::randomLines(const std::string& path, std::vector<std::string>& lines, const int& n_lines, const bool& strip_lines )
{
    // read rhe first N lines only
    std::ifstream file;
    std::string line;
    std::vector<std::string> aux_lines;
    try {
        // pick random lines
        std::string aux;
        try {
            // try reading a gzipped file
            GZutils::readFile( path, aux );

        } catch (GenericException& e) {
            // failed closing file pointer
            throw e;

        } catch (...) {
            // fallback in reading as text file
            if ( aux.size() > 0 ) {
                aux.clear();
            }
            IOutils::readFile( path, aux );
        }
        StringOps::split( aux_lines, aux );
        aux.clear();
        int max = aux_lines.size();
        if ( max > 0 ) {
            if ( max <= n_lines ) {
                lines = aux_lines;
            } else {
                time_t nTime;
                srand((unsigned) time(&nTime));
                int index;
                std::vector<int> picked_indexes;
                for( int i=0 ; i<n_lines ; i++ ) {
                    while (true) {
                        index = rand() % max;
                        if ( VecOps::contains( picked_indexes, index ) == true ) {
                            continue;
                        }
                        break;
                    }
                    line = aux_lines.at( index );
                    if ( strip_lines == true ) {
                        line = StringOps::strip( line );
                    }
                    if ( line.size() == 0 || StringOps::startsWith( line, "#" ) ) {
                        i--;
                        continue;
                    }
                    lines.push_back( line );
                }
            }
        }
        aux_lines.clear();
    } catch (const std::ios_base::failure& err) {
        // failed reading
        lines.clear();
        aux_lines.clear();
        if ( file.is_open() ) {
            file.close();
        }
        throw err;
    } catch (...) {
        lines.clear();
        aux_lines.clear();
        if ( file.is_open() ) {
            file.close();
        }
        throw std::exception(); // exception catched in Craplog
    }
    if ( file.is_open() ) {
        file.close();
    }
    aux_lines.clear();
}


void IOutils::readFile( const std::string& path , std::string& content )
{
    // read the whole file
    std::ifstream file;
    try {
        constexpr std::size_t read_size = std::size_t(4096);
        file = std::ifstream(path);
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
        if ( file.is_open() == true ) {
            file.close();
        }
        throw err;
    } catch (...) {
        if ( file.is_open() == true ) {
            file.close();
        }
        throw std::exception(); // already catched
    }

    if ( file.is_open() == true ) {
        file.close();
    }
}


void IOutils::writeOnFile( const std::string& path, const std::string& content )
{
    std::ofstream file;
    try {
        file.open( path );
        if ( file.is_open() == false ) {
            throw std::ios_base::failure( "file is not open" );
        }
        if ( file.good() == false ) {
            throw std::ios_base::failure( "file is not good" );
        }
        // add bit exceptions
        file.exceptions(std::ifstream::failbit);
        file.exceptions(std::ios_base::badbit);
        // write the content
        file << content << std::endl;

    } catch (const std::ios_base::failure& err) {
        // failed writing
        if ( file.is_open() == true ) {
            file.close();
        }
        throw err;
    } catch (...) {
        if ( file.is_open() == true ) {
            file.close();
        }
        throw std::exception(); // already catched
    }

    if ( file.is_open() == true ) {
        file.close();
    }
}

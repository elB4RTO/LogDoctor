
#include "io.h"

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
    return std::filesystem::exists( path );
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


// rename an entry with a trailing '.copy'
const bool IOutils::renameAsCopy( const std::string& path ) noexcept(true)
{
    bool result = true;
    std::string new_path = path;
    // loop until a valid name is found
    while (true) {
        new_path += ".copy";
        if ( IOutils::exists( new_path ) == false ) {
            // available name found
            break;
        }
    }
    try {
        std::filesystem::rename( path, new_path );
    } catch (...) {
        result = false;
    }
    return result;
}



const std::vector<std::string> IOutils::readLines(const std::string& path, const int& n_lines, const bool& random, const bool& strip_lines )
{
    // read rhe first N lines only
    std::ifstream file;
    std::string line;
    std::vector<std::string> lines, aux_lines;
    try {
        if ( random == false ) {
            // read the initial lines of the file
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
        } else {
            // pick random lines
            StringOps::split( aux_lines, IOutils::readFile( path ) );
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
                        lines.push_back( line );
                    }
                }
            }
            aux_lines.clear();
        }
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
        throw std::exception();
    }
    if ( file.is_open() ) {
        file.close();
    }
    aux_lines.clear();
    return lines;
}


const std::string IOutils::readFile( const std::string& path )
{
    // read the whole file
    std::ifstream file;
    std::string content = std::string();
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
        throw std::exception();
    }

    if ( file.is_open() == true ) {
        file.close();
    }
    return content;
}


void writeOnFile( const std::string& path, const std::string& content )
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
        throw std::exception();
    }

    if ( file.is_open() == true ) {
        file.close();
    }
}

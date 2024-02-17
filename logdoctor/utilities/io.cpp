
#include "io.h"

#include "modules/exceptions.h"

#include "utilities/gzip.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include <fstream>


//! RAII class to handle a file stream resource
template<typename Stream>
class FileHandler final
{
    Stream file;

public:
    explicit FileHandler( const std::string& path )
    : file{ path }
    {
        if ( ! this->file.is_open() ) {
            throw std::ios_base::failure( "file is not open" );
        }
        if ( ! this->file.good() ) {
            throw std::ios_base::failure( "file is not good" );
        }
    }

    ~FileHandler()
    {
        if ( this->file.is_open() ) {
            this->file.close();
        }
    }

    Q_DISABLE_COPY_MOVE(FileHandler)

    inline Stream& operator*()
    {
        return this->file;
    }

    inline void setException( const std::ios_base::iostate e )
    {
        this->file.exceptions( e );
    }
};



namespace IOutils
{

bool checkFile( std::string_view path, const bool readable, const bool writable ) noexcept
{
    if ( isFile( path ) ) {
        // check the needed permissions
        const auto perms{ std::filesystem::status( path ).permissions() };
        if ( readable ) {
            if ( (perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none ) {
                return false;
            }
        }
        if ( writable ) {
            if ( (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none ) {
                return false;
            }
        }
        return true;
    }
    return false;
}


bool checkDir( std::string_view path, const bool readable, const bool writable ) noexcept
{
    if ( isDir( path ) ) {
        // check the needed permissions
        const auto perms{ std::filesystem::status( path ).permissions() };
        if ( readable ) {
            if ( (perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none ) {
                return false;
            }
        }
        if ( writable ) {
            if ( (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none ) {
                return false;
            }
        }
        return true;
    }
    return false;
}


bool makeDir( std::string_view path, std::error_code& err ) noexcept
{
    try {
        const bool failed{ !std::filesystem::create_directories( path, err ) };
        if ( failed || err ) {
            return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}


// rename an entry with a trailing '.copy'
bool renameAsCopy( std::string_view path, std::error_code& err ) noexcept
{
    try {
        std::string new_path{ path };
        // loop until a valid name is found
        while (true) {
            new_path += ".copy";
            if ( ! exists( new_path ) ) {
                // available name found
                break;
            }
        }
        std::filesystem::rename( path, new_path, err );
        if ( err ) {
            return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}


void readFile( const std::string& path, std::string& content )
{
    // read the whole file
    try {
        FileHandler<std::ifstream> file{ path }; // throws std::ios_base::failure on failure
        // add bit exceptions
        file.setException( std::ifstream::failbit );
        file.setException( std::ios_base::badbit );
        // read the whole file
        content = std::string(
            std::istreambuf_iterator<char>( *file ),
            std::istreambuf_iterator<char>() );

    } catch ( const std::ios_base::failure& ) {
        // failed reading
        throw;
    } catch (...) {
        throw std::exception(); // already catched
    }
}


void randomLines( const std::string& path, std::vector<std::string>& lines, const size_t n_lines, const bool strip_lines )
{
    // read rhe first N lines only
    try {
        // pick random lines
        std::string aux;
        try {
            // try reading a gzipped file
            GZutils::readFile( path, aux );

        } catch ( const GenericException& ) {
            // failed closing file pointer
            throw;

        } catch (...) {
            // fallback in reading as text file
            if ( ! aux.empty() ) {
                aux.clear();
            }
            readFile( path, aux );
        }
        std::vector<std::string> aux_lines;
        aux_lines.reserve( StringOps::count( aux, '\n' ) );
        if ( strip_lines ) {
            StringOps::splitrip( aux_lines, aux );
        } else {
            StringOps::split( aux_lines, aux );
        }
        aux.clear();
        const size_t max{ aux_lines.size() };
        if ( max > 0ul ) {
            if ( max <= n_lines ) {
                lines = aux_lines;
            } else {
                time_t nTime;
                srand( (unsigned)time(&nTime) );
                size_t index;
                std::vector<size_t> picked_indexes;
                for( size_t i=0ul; i<n_lines ; ++i ) {
                    while (true) {
                        index = static_cast<size_t>(rand()) % max;
                        if ( VecOps::contains<size_t>( picked_indexes, index ) ) {
                            continue;
                        }
                        break;
                    }
                    const std::string& line{ aux_lines.at( index ) };
                    if ( StringOps::startsWith( line, '#' ) ) { // leave the "#" check for IIS logs
                        -- i;
                        continue;
                    }
                    lines.push_back( line );
                }
                // add the first and last lines, to double check for file integrity
                for ( const size_t& index : std::vector<size_t>{0ul,max-1ul} ) {
                    if ( ! VecOps::contains<size_t>( picked_indexes, index ) ) {
                        const std::string& line{ aux_lines.at( index ) };
                        if ( StringOps::startsWith( line, '#' ) ) {
                            continue;
                        }
                        lines.push_back( line );
                    }
                }
            }
        }

    // re-catched in craplog
    } catch ( const GenericException& ) {
        // failed closing gzip file pointer
        lines.clear();
        throw GenericException( "An error occured while reading the gzipped file" );

    } catch ( const std::ios_base::failure& ) {
        // failed reading
        lines.clear();
        throw GenericException( "An error occured while reading the file" );

    } catch (...) {
        lines.clear();
        throw GenericException( "Something failed while handling the file" );
    }
}


void writeOnFile( const std::string& path, std::string_view content )
{
    try {
        FileHandler<std::ofstream> file{ path }; // throws std::ios_base::failure on failure
        // add bit exceptions
        file.setException( std::ios_base::failbit );
        file.setException( std::ios_base::badbit );
        // write the content
        *file << content << std::endl;

    } catch ( const std::ios_base::failure& ) {
        // failed writing
        throw;
    } catch (...) {
        throw std::exception(); // already catched
    }
}

} // namespace IOUtils

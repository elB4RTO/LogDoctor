
#include "gzip.h"

#include "modules/exceptions.h"

#include <errno.h>
#if defined( Q_OS_WIN )
    #include <QtZlib/zlib.h>
#else
    #include <zlib.h>
#endif


// avoid corruption of the data on Windows/MS-DOS systems
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
    #include <fcntl.h>
    #include <io.h>
    #define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
    #define SET_BINARY_MODE(file)
#endif

// buffer size
#define CHUNK 16384


namespace /*private*/
{

//! RAII class to handle s file resource
class FileHandler final
{
    FILE* file;

public:
    explicit FileHandler( const char* path, const char* mode )
    : file{ nullptr }
    {
    #ifdef _MSC_VER
        fopen_s( &this->file, path, mode );
    #else
        this->file = fopen( path, mode );
    #endif
    }

    ~FileHandler()
    {
        if ( this->valid() ) {
            fclose( this->file );
        }
    }

    Q_DISABLE_COPY_MOVE(FileHandler)

    inline operator FILE*()
    {
        return this->file;
    }

    inline bool valid() const noexcept
    {
        return this->file != NULL;
    }

    inline bool error() const noexcept
    {
        return ferror( this->file );
    }

    inline void close()
    {
        if ( fclose( this->file ) ) {
            throw GenericException( "Failed to close file pointer" );
        }
        this->file = nullptr;
    }
};

} // namespace (private)



namespace GZutils
{

void readFile( const std::filesystem::path& path, std::string& content )
{
    bool successful{ true };

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    // allocate inflate state
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    ret = inflateInit2( &strm, 15|32 );
    if ( ret != Z_OK ) {
        successful &= false;
    }

    if ( successful ) {
        FileHandler file{ path.string().c_str(), "rb" };
        if ( ! file.valid() ) {
            // unable to open the file
            return;
        }
        /*FILE *dest = fopen( out_path.c_str(), "wb" );*/
        // decompress until deflate stream ends or end of file is reached
        do {
            strm.avail_in = static_cast<unsigned>(fread( in, 1, CHUNK, file ));
            if ( file.error() ) {
                // error reading
                (void)inflateEnd( &strm );
                /*successful = false;*/
                break;
            }
            if ( strm.avail_in == 0 ) {
                // no more data to read
                break;
            }
            strm.next_in = in;
            // run inflate() on input until output buffer is not full
            do {
                strm.avail_out = CHUNK;
                strm.next_out  = out;
                ret = inflate( &strm, Z_NO_FLUSH );
                switch (ret) {
                    case Z_NEED_DICT:
                        ret = Z_DATA_ERROR; // fall through
                    case Z_STREAM_ERROR: // <- state not clobbered
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        (void)inflateEnd( &strm );
                        successful &= false;
                        break;
                }
                have = CHUNK - strm.avail_out;
                /*if ( fwrite(out, 1, have, dest) != have || ferror( dest ) ) {
                    (void)inflateEnd( &strm );
                    successful = false;
                    break;
                }*/
                for ( unsigned i{0u}; i<have; ++i ) {
                    content.push_back( out[i] );
                }

            } while ( strm.avail_out == 0 );
            // done when inflate() says it's done
            if ( ! successful ) {
                break;
            }
        } while ( ret != Z_STREAM_END );
        // clean up and return
        (void)inflateEnd( &strm );
        successful = (ret == Z_STREAM_END) ? true : false;
        file.close(); // throws GenericException on failure
    }
    if ( content.empty() ) {
        // probably not a gzip compressed file
        throw(""); // do not implement
    }
    if ( ! successful ) {
        content.clear();
    }
}

} // namespace GZutils

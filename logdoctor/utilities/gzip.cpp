
#include "gzip.h"

#include "modules/exceptions.h"

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>


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


GzipOps::GzipOps()
{

}


void GzipOps::readFile( const std::string& path, std::string& content )
{
    bool successful = true;

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
        successful = false;
    }

    if ( successful == true ) {
        FILE *file = fopen ( path.c_str(), "rb" );
        /*FILE *dest = fopen ( out_path.c_str(), "wb" );*/
        // decompress until deflate stream ends or end of file is reached
        do {
            strm.avail_in = fread( in, 1, CHUNK, file );
            if ( ferror( file ) ) {
                // error reading
                (void)inflateEnd( &strm );
                successful = false;
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
                        successful = false;
                        break;
                }
                have = CHUNK - strm.avail_out;
                /*if ( fwrite(out, 1, have, dest) != have || ferror( dest ) ) {
                    (void)inflateEnd( &strm );
                    successful = false;
                    break;
                }*/
                for ( int i=0; i<have; i++ ) {
                    content.push_back( out[i] );
                }

            } while ( strm.avail_out == 0 );
            // done when inflate() says it's done
            if ( successful == false ) {
                break;
            }
        } while ( ret != Z_STREAM_END );
        // clean up and return
        (void)inflateEnd( &strm );
        successful = (ret == Z_STREAM_END) ? true : false;
        if ( fclose( file ) ) {
            // error while trying to close file pointer
            throw GenericException( "Failed to close file pointer" );
        }
        //delete file;
    }
    if ( content.size() == 0 ) {
        // probably not a gzip compressed file
        throw("");
    }
    if ( successful == false ) {
        content = "";
    }
}



/*const bool GzipOps::readFile( const std::string& path, std::string& content )
{
    bool successful = true;

    FILE * file;
    z_stream strm = {0};
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    int windowBits = 15;
    int ENABLE_ZLIB_GZIP = 32;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = in;
    strm.avail_in = 0;
    if ( inflateInit2 (& strm, windowBits | ENABLE_ZLIB_GZIP) < 0 ) {
        std::cout << "err 1" << std::endl;
        successful = false;
    }

    if ( successful == true ) {
        // open the file
        file = fopen ( path.c_str(), "rb");
        if ( !file ) {
            std::cout << "err opening" << std::endl;
            successful = false;
        }
        if ( successful == true ) {
            while (true) {
                int bytes_read;
                int zlib_status;

                bytes_read = fread (in, sizeof (char), sizeof (in), file);
                if ( ferror(file) ) {
                    std::cout << "err reading" << std::endl;
                    successful = false;
                    break;
                }
                strm.avail_in = bytes_read;
                strm.next_in = in;
                do {
                    unsigned have;
                    strm.avail_out = CHUNK;
                    strm.next_out = out;
                    zlib_status = inflate (& strm, Z_NO_FLUSH);
                    switch (zlib_status) {
                        case Z_OK:
                        case Z_STREAM_END:
                            std::cout << "ok" << std::endl;
                            break;
                        case Z_BUF_ERROR:
                            std::cout << "buffer error" << std::endl;
                            successful = false;
                            break;

                        default:
                            inflateEnd (& strm);
                            std::cout << "gzip error" << std::endl;
                            successful = false;
                            break;
                    }
                    have = CHUNK - strm.avail_out;
                    fwrite (out, sizeof (unsigned char), have, stdout);
                }
                while (strm.avail_out == 0);
                if (feof (file)) {
                    inflateEnd (& strm);
                    break;
                }
            }
        }
    }
    if ( fclose( file ) ) {
        std::cout << "err closing" << std::endl;
        throw("");
    }

    return successful;
}*/

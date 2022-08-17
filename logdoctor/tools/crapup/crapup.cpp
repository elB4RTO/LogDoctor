
#include "crapup.h"

#include "utilities/io.h"
#include "utilities/strings.h"

#include <curl/curl.h>


Crapup::Crapup()
{

}


size_t Crapup::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


const std::tuple<bool, float> Crapup::versionCheck( const std::string& link )
{
    bool successful = false;
    float version = -1;

    CURL *curl;
    CURLcode response;

    std::string content;
    std::string URL;
    if ( link.size() > 0 ) {
        URL = link;
    } else {
        //URL = "https://github.com/elB4RTO/LogDoctor/version.txt";
        URL = "https://github.com/elB4RTO/craplog-GUI/blob/main/version_check";
    }

    /*#ifdef SKIP_PEER_VERIFICATION
        /*
         * If you want to connect to a site who is not using a certificate that is
         * signed by one of the certs in the CA bundle you have, you can skip the
         * verification of the server's certificate. This makes the connection
         * A LOT LESS SECURE.
         *
         * If you have a CA cert for the server stored someplace else than in the
         * default bundle, then the CURLOPT_CAPATH option might come handy for
         * you.
         * /
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    #endif

    #ifdef SKIP_HOSTNAME_VERIFICATION
        /*
         * If the site you are connecting to uses a different host name that what
         * they have mentioned in their server certificate's commonName (or
         * subjectAltName) fields, libcurl will refuse to connect. You can skip
         * this check, but this will make the connection less secure.
         * /
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    #endif*/

    // init the curl session
    curl_global_init( CURL_GLOBAL_ALL );
    curl = curl_easy_init();
    // set the URL
    curl_easy_setopt( curl, CURLOPT_URL, URL.c_str() );
    // fetched data will be sent to this function
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
    // fetched data will be stored in this variable
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &content );
    // set the user agent
    curl_easy_setopt( curl, CURLOPT_USERAGENT, "LogDoctor-libcurl/1.0 (version check)" );
    // make the request
    response = curl_easy_perform( curl );

    // check for errors
    if ( response != CURLE_OK ) {
        // failed
        // !!! PUT A DIALOG MESSAGE HERE !!!
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(response));
    }
    else {
        // successful
        const std::string version_mark = ".:!¦version¦!:.";
        int start = content.find( version_mark );
        if ( start != std::string::npos ) {
            // first found
            start += version_mark.size();
            const int stop = content.find( version_mark, start );
            if ( stop != std::string::npos ) {
                // second found too
                try {
                    // get the version
                    version = std::stof( content.substr( start, stop-start ) );

                } catch (std::invalid_argument& err) {
                    // failed to convert string to float
                    // !!! PUT A DIALOG MESSAGE HERE !!!

                }
            } else {
                // second version mark not found
                // !!! PUT A DIALOG MESSAGE HERE !!!
            }
        } else {
            // first version mark not found
            // !!! PUT A DIALOG MESSAGE HERE !!!
        }
    }

    // cleanup curl stuff
    curl_easy_cleanup( curl );
    curl_global_cleanup();

    return std::make_tuple( successful, version );
}

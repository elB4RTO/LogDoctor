
#include "crapup.h"

#include "modules/dialogs.h"

#include "utilities/io.h"
#include "utilities/strings.h"

#include <curl/curl.h>
#include <stdexcept>


Crapup::Crapup()
{

}


size_t Crapup::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void Crapup::versionCheck( const float& v, const int& dialog_level )
{
    bool successful = false;
    float version = -1;

    CURL *curl;
    CURLcode response;

    std::string content;
    std::string URL;

    const std::string links[3] = {"https://raw.githubusercontent.com/elB4RTO/LogDoctor/main/version.txt",
                                  "https://git.disroot.org/elB4RTO/LogDoctor/raw/branch//main/version.txt",
                                  "https://gitlab.com/elB4RTO/LogDoctor/-/raw/main/version.txt"};

    // init the curl session
    curl_global_init( CURL_GLOBAL_ALL );
    curl = curl_easy_init();
    // fetched data will be sent to this function
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
    // fetched data will be stored in this variable
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, &content );
    // set the user agent
    curl_easy_setopt( curl, CURLOPT_USERAGENT, "LogDoctor/"+std::to_string(v)+" (version check)" );

    int err = 0;

    for ( const std::string& URL : links ) {

        // reset the content
        content.clear();

        // set the URL
        curl_easy_setopt( curl, CURLOPT_URL, URL.c_str() );
        // make the request
        response = curl_easy_perform( curl );

        // check for errors
        if ( response != CURLE_OK ) {
            // failed
            err = 1;
            if ( dialog_level == 2 ) {
                DialogSec::warnConnectionFailed( nullptr,
                    QString::fromStdString( URL ),
                    QString::fromStdString( curl_easy_strerror(response)) );
            }
        } else {
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
                        successful = true;
                        break;

                    } catch (std::invalid_argument& e) {
                        // failed to convert string to float
                        err = 11;
                    }
                } else {
                    // second version mark not found
                    err = 10;
                }
            } else {
                // first version mark not found
                err = 10;
            }
        }
    }


    // cleanup curl stuff
    curl_easy_cleanup( curl );
    curl_global_cleanup();

    if ( successful ) {
        // check the versions
        if ( version > v ) {
            // new version available
            DialogSec::msgNewVersion( nullptr );
        } else if ( version == v ) {
            // same version
            DialogSec::msgSameVersion( nullptr );
        } else if ( version > 0 )  {
            // this version is beyond the current upstream version
            DialogSec::warnFutureVersion( nullptr );
        } else {
            // something went wrong, can't be successful if version is less than 0
            successful = false;
        }
    } else {
        DialogSec::errVersionCheckFailed( nullptr, err );
    }
}

#ifndef CRAPUP_H
#define CRAPUP_H

#include <string>
#include <tuple>


class Crapup
{
public:
    Crapup();

    // perform a GET request for a version-check
    const std::tuple<bool, float> versionCheck( const std::string& link );


private:
    // write fetched data
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif // CRAPUP_H

#ifndef CRAPUP_H
#define CRAPUP_H

#include <string>
#include <tuple>


class Crapup
{
public:
    Crapup();

    // perform a GET request for a version-check
    void versionCheck( const float& current_version, const int& dialog_level );


private:
    // write fetched data
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif // CRAPUP_H

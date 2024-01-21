#ifndef LOGDOCTOR__LIB_H
#define LOGDOCTOR__LIB_H


#include <string>


enum class WebServer {
    Apache,
    Nginx,
    IIS
};


inline std::string toString(const WebServer ws) noexcept
{
    switch (ws) {
        case WebServer::Apache:
            return "Apache";
        case WebServer::Nginx:
            return "Nginx";
        case WebServer::IIS:
            return "IIS";
        default:
            // used in exceptions handling, do not throw
            return "?UNKNOWN?";
    }
}

inline WebServer fromString(const std::string& ws)
{
    if ( ws == "Apache" ) {
        return WebServer::Apache;
    } else if ( ws == "Nginx" ) {
        return WebServer::Nginx;
    } else if ( ws == "IIS" ) {
        return WebServer::IIS;
    } else {
        throw("Unexpected WebServer: " + ws);
    }
}


#endif // LOGDOCTOR__LIB_H

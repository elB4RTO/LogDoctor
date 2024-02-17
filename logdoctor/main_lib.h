#ifndef LOGDOCTOR__LIB_H
#define LOGDOCTOR__LIB_H


#include "modules/exceptions.h"

#include <string>


enum class WebServer {
    Apache,
    Nginx,
    IIS
};


enum class DialogsLevel {
    Essential,
    Normal,
    Explanatory
};



inline bool operator<(const DialogsLevel lhs, const DialogsLevel rhs)
{
    switch (lhs) {
        case DialogsLevel::Essential:
            return rhs == DialogsLevel::Normal || rhs == DialogsLevel::Explanatory;
        case DialogsLevel::Normal:
            return rhs == DialogsLevel::Explanatory;
        case DialogsLevel::Explanatory:
            return false;
        default:
            return false;
    }
}
inline bool operator>=(const DialogsLevel lhs, const DialogsLevel rhs)
{
    return lhs == rhs || !(lhs < rhs);
}



inline std::string toString(const WebServer ws)
{
    switch (ws) {
        case WebServer::Apache:
            return "Apache";
        case WebServer::Nginx:
            return "Nginx";
        case WebServer::IIS:
            return "IIS";
        default:
            throw DoNotCatchException( "Unexpected WebServer", std::to_string(static_cast<int>(ws)) );
    }
}


inline std::string toString(const DialogsLevel lvl)
{
    switch (lvl) {
        case DialogsLevel::Essential:
            return "0";
        case DialogsLevel::Normal:
            return "1";
        case DialogsLevel::Explanatory:
            return "2";
        default:
            throw DoNotCatchException( "Unexpected DialogsLevel", std::to_string(static_cast<int>(lvl)) );
    }
}


#define WS_APACHE WebServer::Apache
#define WS_NGINX  WebServer::Nginx
#define WS_IIS    WebServer::IIS


#define DL_ESSENTIAL   DialogsLevel::Essential
#define DL_NORMAL      DialogsLevel::Normal
#define DL_EXPLANATORY DialogsLevel::Explanatory


#endif // LOGDOCTOR__LIB_H

#ifndef LOGDOCTOR__CRAPLOG__LIB_H
#define LOGDOCTOR__CRAPLOG__LIB_H


typedef unsigned char iis_logs_module_t;

enum class IISLogsModule : iis_logs_module_t {
    W3C  = 0,
    NCSA = 1,
    IIS  = 2
};


#endif // LOGDOCTOR__CRAPLOG__LIB_H

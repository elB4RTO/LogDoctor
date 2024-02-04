#ifndef LOGDOCTOR__CRAPLOG__LIB_H
#define LOGDOCTOR__CRAPLOG__LIB_H


#include <QMetaType>


enum class LogField
{
    Year,
    Month,
    Day,
    Hour,
    Minute,
    Second,
    // request
    Protocol,
    Method,
    Uri,
    Query,
    // server
    ResponseCode,
    TimeTaken,
    BytesSent,
    BytesReceived,
    // client
    Client,
    Cookie,
    UserAgent,
    Referrer
};

Q_DECLARE_METATYPE(LogField)


#endif // LOGDOCTOR__CRAPLOG__LIB_H

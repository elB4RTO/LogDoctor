#ifndef LOGDOCTOR__CRAPVIEW__LIB_H
#define LOGDOCTOR__CRAPVIEW__LIB_H


#include <QMetaType>


enum class LogField
{
    Warning,
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


#endif // LOGDOCTOR__CRAPVIEW__LIB_H


#include "formats.h"

#include "main_lib.h"

#include "formats/check.h"
#include "formats/parse.h"
#include "formats/samples.h"

#include "modules/exceptions.h"


namespace FormatOps
{

LogsFormat processApacheFormatString( const std::string& format_string )
{
    return Private::checkFormat( Private::parseApacheFormatString( format_string ) );
}

LogsFormat processNginxFormatString( const std::string& format_string )
{
    return Private::checkFormat( Private::parseNginxFormatString( format_string ) );
}

LogsFormat processIisFormatString( const std::string& format_string, const IISLogsModule logs_module )
{
    return Private::checkFormat( Private::parseIisFormatString( format_string, logs_module ) );
}


QString getLogLineSample( const WebServer& web_server,const LogsFormat& logs_format )
{
    switch ( web_server ) {
        case WS_APACHE:
            return Private::getApacheLogSample( logs_format );
        case WS_NGINX:
            return Private::getNginxLogSample( logs_format );
        case WS_IIS:
            return Private::getIisLogSample( logs_format );
        default:
            throw DoNotCatchException( "Unexpected WebServer", std::to_string(static_cast<int>(web_server)) );
    }
}

} // namespace FormatOps

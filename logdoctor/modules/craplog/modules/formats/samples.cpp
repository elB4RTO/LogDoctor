
#include "samples.h"

#include "modules/craplog/modules/lib.h"

#include <QString>

#include <unordered_map>


using samples_umap_t = std::unordered_map<LogsFormatField, QString>;


namespace /*private*/
{

//! Builds the log line sample using the given map and logs format
/*!
    \return The sample log line
*/
QString makeSample( const samples_umap_t& map, const LogsFormat& logs_format );


//! Returns the map of all Apache fields samples
/*!
    \return The map of Apache fields
*/
const samples_umap_t& getApacheSamples();


//! Returns the map of all Nginx fields samples
/*!
    \return The map of Nginx fields
*/
const samples_umap_t& getNginxSamples();


//! Returns the map of all IIS fields samples
/*!
    \return The map of IIS fields
*/
const samples_umap_t& getIisSamples();

} // namespace (private)



namespace FormatOps::Private
{

QString getApacheLogSample( const LogsFormat& logs_format )
{
    return makeSample( getApacheSamples(), logs_format );
}


QString getNginxLogSample( const LogsFormat& logs_format )
{
    return makeSample( getNginxSamples(), logs_format );
}


QString getIisLogSample( const LogsFormat& logs_format )
{
    return makeSample( getIisSamples(), logs_format );
}

} // namespace FormatOps::Private



namespace /*private*/
{

QString makeSample( const samples_umap_t& map, const LogsFormat& logs_format )
{
    QString sample;
    // append the initial characters
    sample += QString::fromStdString( logs_format.initial );
    for ( size_t i{0ul}; i<logs_format.separators.size(); ++i ) {
        // append fields and separators
        sample += map.at( logs_format.fields.at( i ) );
        sample += QString::fromStdString( logs_format.separators.at( i ) );
    }
    // add the last field
    if ( logs_format.fields.size() > 0ul ) {
        sample += map.at( logs_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( logs_format.final );
    return sample;
}


const samples_umap_t& getApacheSamples()
{
    static const samples_umap_t apache_samples{
        {_DISCARDED,            R"(<span style="color:#7f7f7f">DISCARDED</span>)"},
        {date_time_epoch_s,     R"(<b><span style="color:#00cc6f">946771199</span></b>)"},
        {date_time_epoch_ms,    R"(<b><span style="color:#00cc6f">946771199000</span></b>)"},
        {date_time_epoch_us,    R"(<b><span style="color:#00cc6f">946771199000000</span></b>)"},
        {date_time_ncsa,        R"(<b><span style="color:#00cc6f">01/Jan/2000:23:59:59 +0000</span></b>)"},
        {date_time_mcs,         R"(<b><span style="color:#00cc6f">Sat Jan 01 23:59:59 2000</span></b>)"},
        {date_time_yyyymmdd,    R"(<b><span style="color:#00cc6f">2000-01-01</span></b>)"},
        {date_time_mmddyy,      R"(<b><span style="color:#00cc6f">01/01/00</span></b>)"},
        {date_time_year,        R"(<b><span style="color:#00cc6f">2000</span></b>)"},
        {date_time_year_short,  R"(<b><span style="color:#00cc6f">00</span></b>)"},
        {date_time_month_str,   R"(<b><span style="color:#00cc6f">January</span></b>)"},
        {date_time_month,       R"(<b><span style="color:#00cc6f">01</span></b>)"},
        {date_time_day,         R"(<b><span style="color:#00cc6f">01</span></b>)"},
        {date_time_clock_12,    R"(<b><span style="color:#00cc6f">11:59:59 pm</span></b>)"},
        {date_time_clock_24,    R"(<b><span style="color:#00cc6f">23:59:59</span></b>)"},
        {date_time_clock_short, R"(<b><span style="color:#00cc6f">23:59</span></b>)"},
        {date_time_hour,        R"(<b><span style="color:#00cc6f">23</span></b>)"},
        {date_time_minute,      R"(<b><span style="color:#00cc6f">59</span></b>)"},
        {date_time_second,      R"(<b><span style="color:#00cc6f">59</span></b>)"},
        {request_full,          R"(<b><span style="color:#00cc6f">GET /index.php?query=x HTTP/1.1</span></b>)"},
        {request_protocol,      R"(<b><span style="color:#00cc6f">HTTP/1.1</span></b>)"},
        {request_method,        R"(<b><span style="color:#00cc6f">GET</span></b>)"},
        {request_uri,           R"(<b><span style="color:#00cc6f">/index.php</span></b>)"},
        {request_query,         R"(<b><span style="color:#00cc6f">query=x</span></b>)"},
        {response_code,         R"(<b><span style="color:#00cc6f">404</span></b>)"},
        {bytes_sent,            R"(<b><span style="color:#00cc6f">1234</span></b>)"},
        {bytes_received,        R"(<b><span style="color:#00cc6f">123</span></b>)"},
        {time_taken_s,          R"(<b><span style="color:#00cc6f">1</span></b>)"},
        {time_taken_ms,         R"(<b><span style="color:#00cc6f">1000</span></b>)"},
        {time_taken_us,         R"(<b><span style="color:#00cc6f">1000000</span></b>)"},
        {referer,               R"(<b><span style="color:#00cc6f">http://www.referrer.site</span></b>)"},
        {cookie,                R"(<b><span style="color:#00cc6f">aCookie=abc123</span></b>)"},
        {user_agent,            R"(<b><span style="color:#00cc6f">UserAgent/3.0 (Details stuff) Info/123</span></b>)"},
        {client,                R"(<b><span style="color:#00cc6f">192.168.1.123</span></b>)"} };
    return apache_samples;
}


const samples_umap_t& getNginxSamples()
{
    static const samples_umap_t nginx_samples{
        {_DISCARDED,            R"(<span style="color:#7f7f7f">DISCARDED</span>)"},
        {date_time_epoch_s_ms,  R"(<b><span style="color:#00cc6f">946771199.000</span></b>)"},
        {date_time_ncsa,        R"(<b><span style="color:#00cc6f">01/Jan/2000:23:59:59 +0000</span></b>)"},
        {date_time_iso,         R"(<b><span style="color:#00cc6f">2000-01-01T23:59:59+00:00</span></b>)"},
        {date_time_gmt,         R"(<b><span style="color:#00cc6f">Saturday, 01-Jan-2000 23:59:59 UTC</span></b>)"},
        {request_full,          R"(<b><span style="color:#00cc6f">GET /index.php?query=x HTTP/1.1</span></b>)"},
        {request_protocol,      R"(<b><span style="color:#00cc6f">HTTP/1.1</span></b>)"},
        {request_method,        R"(<b><span style="color:#00cc6f">GET</span></b>)"},
        {request_uri_query,     R"(<b><span style="color:#00cc6f">/index.php?query=x</span></b>)"},
        {request_uri,           R"(<b><span style="color:#00cc6f">/index.php</span></b>)"},
        {request_query,         R"(<b><span style="color:#00cc6f">query=x</span></b>)"},
        {response_code,         R"(<b><span style="color:#00cc6f">404</span></b>)"},
        {bytes_sent,            R"(<b><span style="color:#00cc6f">1234</span></b>)"},
        {bytes_received,        R"(<b><span style="color:#00cc6f">123</span></b>)"},
        {time_taken_s_ms,       R"(<b><span style="color:#00cc6f">1.000</span></b>)"},
        {referer,               R"(<b><span style="color:#00cc6f">http://www.referrer.site</span></b>)"},
        {cookie,                R"(<b><span style="color:#00cc6f">aCookie=abc123</span></b>)"},
        {user_agent,            R"(<b><span style="color:#00cc6f">UserAgent/3.0 (Details stuff) Info/123</span></b>)"},
        {client,                R"(<b><span style="color:#00cc6f">192.168.1.123</span></b>)"} };
    return nginx_samples;
}


const samples_umap_t& getIisSamples()
{
    static const samples_umap_t iis_samples{
        {_DISCARDED,         R"(<span style="color:#7f7f7f">DISCARDED</span>)"},
        {date_time_ncsa,     R"(<b><span style="color:#00cc6f">01/Jan/2000:23:59:59 +0000</span></b>)"},
        {date_time_mdyyyy,   R"(<b><span style="color:#00cc6f">1/1/2000</span></b>)"},
        {date_time_utc_d,    R"(<b><span style="color:#00cc6f">2000-01-01</span></b>)"},
        {date_time_utc_t,    R"(<b><span style="color:#00cc6f">23:59:59</span></b>)"},
        {request_full,       R"(<b><span style="color:#00cc6f">GET /index.php?query=x HTTP/1.1</span></b>)"},
        {request_protocol,   R"(<b><span style="color:#00cc6f">HTTP/1.1</span></b>)"},
        {request_method,     R"(<b><span style="color:#00cc6f">GET</span></b>)"},
        {request_uri,        R"(<b><span style="color:#00cc6f">/index.php</span></b>)"},
        {request_query,      R"(<b><span style="color:#00cc6f">query=x</span></b>)"},
        {response_code,      R"(<b><span style="color:#00cc6f">404</span></b>)"},
        {bytes_sent,         R"(<b><span style="color:#00cc6f">1234</span></b>)"},
        {bytes_received,     R"(<b><span style="color:#00cc6f">123</span></b>)"},
        {time_taken_ms,      R"(<b><span style="color:#00cc6f">1000</span></b>)"},
        {referer,            R"(<b><span style="color:#00cc6f">http://www.referrer.site</span></b>)"},
        {cookie,             R"(<b><span style="color:#00cc6f">aCookie=abc123</span></b>)"},
        {user_agent,         R"(<b><span style="color:#00cc6f">UserAgent/3.0+(Details+stuff)+Info/123</span></b>)"},
        {client,             R"(<b><span style="color:#00cc6f">192.168.1.123</span></b>)"} };
    return iis_samples;
}

} // namespace (private)

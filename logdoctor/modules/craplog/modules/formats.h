#ifndef LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H
#define LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H


#include "lib.h"

#include "modules/craplog/lib.h"

#include <QString>

#include <unordered_map>


struct LogsFormat;


//! FormatOps
/*!
    Operations for the logs formats
*/
class FormatOps final
{
    using simple_fields_umap_t = std::unordered_map<std::string, LogsFormatField>;
    using composed_fields_umap_t = std::unordered_map<std::string, std::unordered_map<std::string, LogsFormatField>>;
    using samples_umap_t = std::unordered_map<LogsFormatField, QString>;

public:

    //! Processes the given string to extrapolate the format for Apache2
    /*!
        \param format_string The format string to process
        \return The logs format
        \throw LogFormatException
        \see LogsFormat
    */
    LogsFormat processApacheFormatString( const std::string& format_string ) const;

    //! Processes the given string to extrapolate the format for Nginx
    /*!
        \param format_string The format string to process
        \return The logs format
        \throw LogFormatException
        \see LogsFormat
    */
    LogsFormat processNginxFormatString( const std::string& format_string ) const;

    //! Processes the given string to extrapolate the format for the IIS
    /*!
        \param format_string The format string to process
        \param log_module The ID of the log module to use
        \return The logs format
        \throw LogFormatException
        \see LogsFormat
    */
    LogsFormat processIisFormatString( const std::string& format_string, const IISLogsModule log_module ) const;


    /////////////////
    //// SAMPLES ////

    //! Returns a log line sample based on the given format
    /*!
        \param log_format The logs format to use to build the sample
        \return The sample line
        \see LogsFormat, Craplog::getLogsFormatSample()
    */
    QString getApacheLogSample( const LogsFormat& log_format ) const noexcept;

    //! Returns a log line sample based on the given format
    /*!
        \param log_format The logs format to use to build the sample
        \return The sample line
        \see LogsFormat, Craplog::getLogsFormatSample()
    */
    QString getNginxLogSample( const LogsFormat& log_format ) const noexcept;

    //! Returns a log line sample based on the given format
    /*!
        \param log_format The logs format to use to build the sample
        \return The sample line
        \see LogsFormat, Craplog::getLogsFormatSample()
    */
    QString getIisLogSample( const LogsFormat& log_format ) const noexcept;


private:

    /////////////////
    //// APACHE2 ////

    //! Access logs fields formats
    const simple_fields_umap_t APACHE_ALF{
        {"a",  _DISCARDED},
        {"A",  _DISCARDED},
        {"b",  _DISCARDED},
        {"B",  _DISCARDED},
        {"C",  _DISCARDED},
        {"D",  time_taken_ms},
        {"e",  _DISCARDED},
        {"f",  _DISCARDED},
        {"h",  client},
        {"H",  request_protocol},
        {"i",  _DISCARDED},
        {"I",  bytes_received},
        {"k",  _DISCARDED},
        {"l",  _DISCARDED},
        {"L",  _DISCARDED},
        {"m",  request_method},
        {"n",  _DISCARDED},
        {"o",  _DISCARDED},
        {"O",  bytes_sent},
        {"p",  _DISCARDED},
        {"P",  _DISCARDED},
        {"q",  request_query},
        {"r",  request_full},
        {"R",  _DISCARDED},
        {"s",  response_code},
        {"<s", response_code},
        {">s", response_code},
        {"S",  _DISCARDED},
        {"t",  date_time_ncsa},
        {"T",  time_taken_s},
        {"u",  _DISCARDED},
        {"U",  request_uri},
        {"v",  _DISCARDED},
        {"V",  _DISCARDED},
        {"X",  _DISCARDED} };

    //! Composed access logs fields formats
    const composed_fields_umap_t APACHE_ALF_V{
        {"a", { {"",           client}, // as %h (from single fields)
                {"c",          client}}},
        {"h", { {"",           client}, // as %h (from single fields)
                {"c",          client}}},
        {"i", { {"",           _DISCARDED},
                {"Cookie",     cookie},
                {"Referer",    referer},
                {"User-agent", user_agent}}},
        {"t", { {"",           date_time_ncsa}, // as %t (from single fields)
              //{"%%",         _DISCARDED}, // the percent sign
              //{"%n",         _DISCARDED}, // line feed
              //{"%t",         _DISCARDED}, // horizontal tab
                {"sec",        date_time_epoch_s},
                {"msec",       date_time_epoch_ms},
                {"usec",       date_time_epoch_us},
                {"msec_frac",  _DISCARDED}, // milliseconds fraction
                {"usec_frac",  _DISCARDED}, // microseconds fraction
                {"%a",         _DISCARDED}, // abbreviated weekday name
                {"%A",         _DISCARDED}, // weekday name
                {"%b",         date_time_month_str},
                {"%B",         date_time_month_str},
                {"%c",         date_time_mcs},
                {"%C",         _DISCARDED}, // year (first 2 digits, aka centuries)
                {"%d",         date_time_day},
                {"%D",         date_time_mmddyy},
                {"%e",         date_time_day},
                {"%F",         date_time_yyyymmdd},
                {"%g",         _DISCARDED}, // weel-based year (last 2 digits)
                {"%G",         _DISCARDED}, // week-based year, namely the year which contains the current week
                {"%h",         date_time_month_str},
                {"%H",         date_time_hour},
                {"%I",         _DISCARDED}, // hour (12h format)
                {"%j",         _DISCARDED}, // day of the year number
                {"%k",         date_time_hour}, // ?!? hour (24h format) !?! no documentation ?!?
                {"%l",         _DISCARDED}, // hour (12h format)
                {"%m",         date_time_month},
                {"%M",         date_time_minute},
                {"%p",         _DISCARDED}, // AM or PM
                {"%r",         date_time_clock_12},
                {"%R",         date_time_clock_short},
                {"%s",         date_time_epoch_s},
                {"%S",         date_time_second},
                {"%T",         date_time_clock_24},
                {"%u",         _DISCARDED}, // weekday number (1-7, Monday is 1)
                {"%U",         _DISCARDED}, // week of the year number, with the first Sunday as the first day of week one
                {"%V",         _DISCARDED}, // week of the year number
                {"%w",         _DISCARDED}, // weekday number (0-6, Sunday is 0)
                {"%W",         _DISCARDED}, // week of the year number, with the first Monday as the first day of week one
                {"%x",         date_time_mmddyy},
                {"%X",         date_time_clock_24},
                {"%y",         date_time_year_short},
                {"%Y",         date_time_year},
                {"%z",         _DISCARDED}, // timezone offset from UTC (1 minute=1, 1 hour=100)
                {"%Z",         _DISCARDED}}}, // timezone name abbreviation
        {"T", { {"",           time_taken_s}, // as s
                {"s",          time_taken_s},
                {"ms",         time_taken_ms},
                {"us",         time_taken_us}}},
        // composed not in use
        {"C",   {}},
        {"e",   {}},
        {"L",   {}},
        {"n",   {}},
        {"o",   {}},
        {"p",   {{"",          _DISCARDED}, // as canonical
                 {"canonical", _DISCARDED},
                 {"local",     _DISCARDED},
                 {"remote",    _DISCARDED}}},
        {"P",   {{"",          _DISCARDED}, // as pid
                 {"pid",       _DISCARDED},
                 {"tid",       _DISCARDED},
                 {"hextid",    _DISCARDED}}},
        {"^ti", {}},
        {"^to", {}} };

    // Access logs fields formats samples
    const samples_umap_t APACHE_ALF_SAMPLES{
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


    ///////////////
    //// NGINX ////

    //! Access logs fields formats
    const simple_fields_umap_t NGINX_ALF{
        {"remote_addr",        client},
        {"realip_remote_addr", client},
        {"time_local",         date_time_ncsa},
        {"time_iso8601",       date_time_iso},
        {"date_gmt",           date_time_gmt},
        {"msec",               date_time_epoch_s_ms},
        {"request",            request_full},
        {"server_protocol",    request_protocol},
        {"request_method",     request_method},
        {"request_uri",        request_uri_query},
        {"uri",                request_uri},
        {"query_string",       request_query},
        {"status",             response_code},
        {"bytes_sent",         bytes_sent},
        {"request_length",     bytes_received},
        {"request_time",       time_taken_s_ms},
        {"http_referer",       referer},
        {"cookie_",            cookie},
        {"http_user_agent",    user_agent},
        // not in use, will be discarded
        {"ancient_browser",           _DISCARDED},
        {"arg_",                      _DISCARDED},
        {"args",                      _DISCARDED},
        {"binary_remote_addr",        _DISCARDED},
        {"body_bytes_sent",           _DISCARDED},
        {"connection",                _DISCARDED},
        {"connection_requests",       _DISCARDED},
        {"connections_active",        _DISCARDED},
        {"connections_reading",       _DISCARDED},
        {"connections_waiting",       _DISCARDED},
        {"connections_writing",       _DISCARDED},
        {"content_length",            _DISCARDED},
        {"content_type",              _DISCARDED},
        {"date_local",                _DISCARDED},
        {"document_root",             _DISCARDED},
        {"document_uri",              _DISCARDED},
        {"fastcgi_path_info",         _DISCARDED},
        {"fastcgi_script_name",       _DISCARDED},
        {"geoip_area_code",           _DISCARDED},
        {"geoip_city",                _DISCARDED},
        {"geoip_city_continent_code", _DISCARDED},
        {"geoip_city_country_code",   _DISCARDED},
        {"geoip_city_country_code3",  _DISCARDED},
        {"geoip_city_country_name",   _DISCARDED},
        {"geoip_country_code",        _DISCARDED},
        {"geoip_country_code3",       _DISCARDED},
        {"geoip_country_name",        _DISCARDED},
        {"geoip_dma_code",            _DISCARDED},
        {"geoip_latitude",            _DISCARDED},
        {"geoip_longitude",           _DISCARDED},
        {"geoip_org",                 _DISCARDED},
        {"geoip_postal_code",         _DISCARDED},
        {"geoip_region",              _DISCARDED},
        {"geoip_region_name",         _DISCARDED},
        {"gzip_ratio",                _DISCARDED},
        {"host",                      _DISCARDED},
        {"hostname",                  _DISCARDED},
        {"http2",                     _DISCARDED},
        {"http_",                     _DISCARDED},
        {"https",                     _DISCARDED},
        {"invalid_referer",           _DISCARDED},
        {"is_args",                   _DISCARDED},
        {"limit_rate",                _DISCARDED},
        {"memcached_key",             _DISCARDED},
        {"modern_browser",            _DISCARDED},
        {"msie",                      _DISCARDED},
        {"nginx_version",             _DISCARDED},
        {"pid",                       _DISCARDED},
        {"pipe",                      _DISCARDED},
        {"proxy_add_x_forwarded_for", _DISCARDED},
        {"proxy_host",                _DISCARDED},
        {"proxy_port",                _DISCARDED},
        {"proxy_protocol_addr",       _DISCARDED},
        {"proxy_protocol_port",       _DISCARDED},
        {"realip_remote_port",        _DISCARDED},
        {"realpath_root",             _DISCARDED},
        {"remote_port",               _DISCARDED},
        {"remote_user",               _DISCARDED},
        {"request_body",              _DISCARDED},
        {"request_body_file",         _DISCARDED},
        {"request_completion",        _DISCARDED},
        {"request_filename",          _DISCARDED},
        {"request_id",                _DISCARDED},
        {"scheme",                    _DISCARDED},
        {"secure_link",               _DISCARDED},
        {"secure_link_expires",       _DISCARDED},
        {"sent_http_",                _DISCARDED},
        {"server_addr",               _DISCARDED},
        {"server_name",               _DISCARDED},
        {"server_port",               _DISCARDED},
        {"session_log_binary_id",     _DISCARDED},
        {"session_log_id",            _DISCARDED},
        {"slice_range",               _DISCARDED},
        {"spdy",                      _DISCARDED},
        {"spdy_request_priority",     _DISCARDED},
        {"ssl_cipher",                _DISCARDED},
        {"ssl_client_cert",           _DISCARDED},
        {"ssl_client_fingerprint",    _DISCARDED},
        {"ssl_client_i_dn",           _DISCARDED},
        {"ssl_client_raw_cert",       _DISCARDED},
        {"ssl_client_s_dn",           _DISCARDED},
        {"ssl_client_serial",         _DISCARDED},
        {"ssl_client_verify",         _DISCARDED},
        {"ssl_protocol",              _DISCARDED},
        {"ssl_server_name",           _DISCARDED},
        {"ssl_session_id",            _DISCARDED},
        {"ssl_session_reused",        _DISCARDED},
        {"tcpinfo_rtt",               _DISCARDED},
        {"tcpinfo_rttvar",            _DISCARDED},
        {"tcpinfo_snd_cwnd",          _DISCARDED},
        {"tcpinfo_rcv_space",         _DISCARDED},
        {"uid_got",                   _DISCARDED},
        {"uid_reset",                 _DISCARDED},
        {"uid_set",                   _DISCARDED},
        {"upstream_addr",             _DISCARDED},
        {"upstream_cache_status",     _DISCARDED},
        {"upstream_connect_time",     _DISCARDED},
        {"upstream_cookie_",          _DISCARDED},
        {"upstream_header_time",      _DISCARDED},
        {"upstream_http_",            _DISCARDED},
        {"upstream_response_length",  _DISCARDED},
        {"upstream_response_time",    _DISCARDED},
        {"upstream_status",           _DISCARDED} };

    // Access logs fields formats samples
    const samples_umap_t NGINX_ALF_SAMPLES{
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



    /////////////
    //// IIS ////

    //! Access logs fields formats (W3C)
    const simple_fields_umap_t IIS_ALF{
        {"date",             date_time_utc_d},
        {"time",             date_time_utc_t},
        {"cs-version",       request_protocol},
        {"cs-method",        request_method},
        {"cs-uri-stem",      request_uri},
        {"cs-uri-query",     request_query},
        {"sc-status",        response_code},
        {"sc-bytes",         bytes_sent},
        {"cs-bytes",         bytes_received},
        {"time-taken",       time_taken_ms},
        {"cs(Referer)",      referer},
        {"cs(Cookie)",       cookie},
        {"cs(User-Agent)",   user_agent},
        {"c-ip",             client},
        // not in use, will be discarded
        {"s-sitename",        _DISCARDED},
        {"s-computername",    _DISCARDED},
        {"s-ip",              _DISCARDED},
        {"s-port",            _DISCARDED},
        {"cs-username",       _DISCARDED},
        {"cs-host",           _DISCARDED},
        {"sc-substatus",      _DISCARDED},
        {"sc-win32-status",   _DISCARDED},
        {"streamid",          _DISCARDED} };

    // Access logs fields formats samples
    const samples_umap_t IIS_ALF_SAMPLES{
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

};


#endif // LOGDOCTOR__CRAPLOG__MODULES__FORMATS_H

#ifndef LOGDOCTOR__CRAPLOG__FORMATS_H
#define LOGDOCTOR__CRAPLOG__FORMATS_H


#include <QString>

#include <unordered_map>

struct LogsFormat;


//! FormatOps
/*!
    Operations for the logs formats
*/
class FormatOps final
{
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
    LogsFormat processIisFormatString( const std::string& format_string, const int& log_module ) const;


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

    //!< Access logs fields formats
    const std::unordered_map<std::string, std::string> APACHE_ALF{
        {"a",  "NONE"},
        {"A",  "NONE"},
        {"b",  "NONE"},
        {"B",  "NONE"},
        {"C",  "NONE"},
        {"D",  "time_taken_ms"},
        {"e",  "NONE"},
        {"f",  "NONE"},
        {"h",  "client"},
        {"H",  "request_protocol"},
        {"i",  "NONE"},
        {"I",  "bytes_received"},
        {"k",  "NONE"},
        {"l",  "NONE"},
        {"L",  "NONE"},
        {"m",  "request_method"},
        {"n",  "NONE"},
        {"o",  "NONE"},
        {"O",  "bytes_sent"},
        {"p",  "NONE"},
        {"P",  "NONE"},
        {"q",  "request_query"},
        {"r",  "request_full"},
        {"R",  "NONE"},
        {"s",  "response_code"},
        {"<s", "response_code"},
        {">s", "response_code"},
        {"S",  "NONE"},
        {"t",  "date_time_ncsa"},
        {"T",  "time_taken_s"},
        {"u",  "NONE"},
        {"U",  "request_uri"},
        {"v",  "NONE"},
        {"V",  "NONE"},
        {"X",  "NONE"} };

    //!< Composed access logs fields formats
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> APACHE_ALF_V{
        {"a", { {"",           "client"}, // as %h (from single fields)
                {"c",          "client"}}},
        {"h", { {"",           "client"}, // as %h (from single fields)
                {"c",          "client"}}},
        {"i", { {"",           "NONE"},
                {"Cookie",     "cookie"},
                {"Referer",    "referer"},
                {"User-agent", "user_agent"}}},
        {"t", { {"",           "date_time_ncsa"}, // as %t (from single fields)
              //{"%%",         "NONE"}, // the percent sign
              //{"%n",         "NONE"}, // line feed
              //{"%t",         "NONE"}, // horizontal tab
                {"sec",        "date_time_epoch_s"},
                {"msec",       "date_time_epoch_ms"},
                {"usec",       "date_time_epoch_us"},
                {"msec_frac",  "NONE"}, // milliseconds fraction
                {"usec_frac",  "NONE"}, // microseconds fraction
                {"%a",         "NONE"}, // abbreviated weekday name
                {"%A",         "NONE"}, // weekday name
                {"%b",         "date_time_month_str"},
                {"%B",         "date_time_month_str"},
                {"%c",         "date_time_mcs"},
                {"%C",         "NONE"}, // year (first 2 digits, aka centuries)
                {"%d",         "date_time_day"},
                {"%D",         "date_time_MMDDYY"},
                {"%e",         "date_time_day"},
                {"%F",         "date_time_YYYYMMDD"},
                {"%g",         "NONE"}, // weel-based year (last 2 digits)
                {"%G",         "NONE"}, // week-based year, namely the year which contains the current week
                {"%h",         "date_time_month_str"},
                {"%H",         "date_time_hour"},
                {"%I",         "NONE"}, // hour (12h format)
                {"%j",         "NONE"}, // day of the year number
                {"%k",         "date_time_hour"}, // ?!? hour (24h format) !?! no documentation ?!?
                {"%l",         "NONE"}, // hour (12h format)
                {"%m",         "date_time_month"},
                {"%M",         "date_time_minute"},
                {"%p",         "NONE"}, // AM or PM
                {"%r",         "date_time_clock_12"},
                {"%R",         "date_time_clock_short"},
                {"%s",         "date_time_epoch_s"},
                {"%S",         "date_time_second"},
                {"%T",         "date_time_clock_24"},
                {"%u",         "NONE"}, // weekday number (1-7, Monday is 1)
                {"%U",         "NONE"}, // week of the year number, with the first Sunday as the first day of week one
                {"%V",         "NONE"}, // week of the year number
                {"%w",         "NONE"}, // weekday number (0-6, Sunday is 0)
                {"%W",         "NONE"}, // week of the year number, with the first Monday as the first day of week one
                {"%x",         "date_time_MMDDYY"},
                {"%X",         "date_time_clock_24"},
                {"%y",         "date_time_year_short"},
                {"%Y",         "date_time_year"},
                {"%z",         "NONE"}, // timezone offset from UTC (1 minute=1, 1 hour=100)
                {"%Z",         "NONE"}}}, // timezone name abbreviation
        {"T", { {"",           "time_taken_s"}, // as s
                {"s",          "time_taken_s"},
                {"ms",         "time_taken_ms"},
                {"us",         "time_taken_us"}}},
        // composed not in use
        {"C",   {}},
        {"e",   {}},
        {"L",   {}},
        {"n",   {}},
        {"o",   {}},
        {"p",   {{"",          "NONE"}, // as canonical
                 {"canonical", "NONE"},
                 {"local",     "NONE"},
                 {"remote",    "NONE"}}},
        {"P",   {{"",          "NONE"}, // as pid
                 {"pid",       "NONE"},
                 {"tid",       "NONE"},
                 {"hextid",    "NONE"}}},
        {"^ti", {}},
        {"^to", {}} };

    // Access logs fields formats samples
    const std::unordered_map<std::string, QString> APACHE_ALF_SAMPLES{
        {"NONE",                  "<span style=\"color:#7f7f7f\">DISCARDED</span>"},
        {"date_time_epoch_s",     "<b><span style=\"color:#00cc6f\">946771199</span></b>"},
        {"date_time_epoch_ms",    "<b><span style=\"color:#00cc6f\">946771199000</span></b>"},
        {"date_time_epoch_us",    "<b><span style=\"color:#00cc6f\">946771199000000</span></b>"},
        {"date_time_ncsa",        "<b><span style=\"color:#00cc6f\">01/Jan/2000:23:59:59 +0000</span></b>"},
        {"date_time_mcs",         "<b><span style=\"color:#00cc6f\">Sat Jan 01 23:59:59 2000</span></b>"},
        {"date_time_YYYYMMDD",    "<b><span style=\"color:#00cc6f\">2000-01-01</span></b>"},
        {"date_time_MMDDYY",      "<b><span style=\"color:#00cc6f\">01/01/00</span></b>"},
        {"date_time_year",        "<b><span style=\"color:#00cc6f\">2000</span></b>"},
        {"date_time_year_short",  "<b><span style=\"color:#00cc6f\">00</span></b>"},
        {"date_time_month_str",   "<b><span style=\"color:#00cc6f\">January</span></b>"},
        {"date_time_month",       "<b><span style=\"color:#00cc6f\">01</span></b>"},
        {"date_time_day",         "<b><span style=\"color:#00cc6f\">01</span></b>"},
        {"date_time_clock_12",    "<b><span style=\"color:#00cc6f\">11:59:59 pm</span></b>"},
        {"date_time_clock_24",    "<b><span style=\"color:#00cc6f\">23:59:59</span></b>"},
        {"date_time_clock_short", "<b><span style=\"color:#00cc6f\">23:59</span></b>"},
        {"date_time_hour",        "<b><span style=\"color:#00cc6f\">23</span></b>"},
        {"date_time_minute",      "<b><span style=\"color:#00cc6f\">59</span></b>"},
        {"date_time_second",      "<b><span style=\"color:#00cc6f\">59</span></b>"},
        {"request_full",          "<b><span style=\"color:#00cc6f\">GET /index.php?query=x HTTP/1.1</span></b>"},
        {"request_protocol",      "<b><span style=\"color:#00cc6f\">HTTP/1.1</span></b>"},
        {"request_method",        "<b><span style=\"color:#00cc6f\">GET</span></b>"},
        {"request_uri",           "<b><span style=\"color:#00cc6f\">/index.php</span></b>"},
        {"request_query",         "<b><span style=\"color:#00cc6f\">query=x</span></b>"},
        {"response_code",         "<b><span style=\"color:#00cc6f\">404</span></b>"},
        {"bytes_sent",            "<b><span style=\"color:#00cc6f\">1234</span></b>"},
        {"bytes_received",        "<b><span style=\"color:#00cc6f\">123</span></b>"},
        {"time_taken_s",          "<b><span style=\"color:#00cc6f\">1</span></b>"},
        {"time_taken_ms",         "<b><span style=\"color:#00cc6f\">1000</span></b>"},
        {"time_taken_us",         "<b><span style=\"color:#00cc6f\">1000000</span></b>"},
        {"referer",               "<b><span style=\"color:#00cc6f\">http://www.referrer.site</span></b>"},
        {"cookie",                "<b><span style=\"color:#00cc6f\">aCookie=abc123</span></b>"},
        {"user_agent",            "<b><span style=\"color:#00cc6f\">UserAgent/3.0 (Details stuff) Info/123</span></b>"},
        {"client",                "<b><span style=\"color:#00cc6f\">192.168.1.123</span></b>"} };


    ///////////////
    //// NGINX ////

    //!< Access logs fields formats
    const std::unordered_map<std::string, std::string> NGINX_ALF{
        {"remote_addr",        "client"},
        {"realip_remote_addr", "client"},
        {"time_local",         "date_time_ncsa"},
        {"time_iso8601",       "date_time_iso"},
        {"date_gmt",           "date_time_gmt"},
        {"msec",               "date_time_epoch_s.ms"},
        {"request",            "request_full"},
        {"server_protocol",    "request_protocol"},
        {"request_method",     "request_method"},
        {"request_uri",        "request_uri_query"},
        {"uri",                "request_uri"},
        {"query_string",       "request_query"},
        {"status",             "response_code"},
        {"bytes_sent",         "bytes_sent"},
        {"request_length",     "bytes_received"},
        {"request_time",       "time_taken_s.ms"},
        {"http_referer",       "referer"},
        {"cookie_",            "cookie"},
        {"http_user_agent",    "user_agent"},
        // not in use, will be discarded
        {"ancient_browser",           "NONE"},
        {"arg_",                      "NONE"},
        {"args",                      "NONE"},
        {"binary_remote_addr",        "NONE"},
        {"body_bytes_sent",           "NONE"},
        {"connection",                "NONE"},
        {"connection_requests",       "NONE"},
        {"connections_active",        "NONE"},
        {"connections_reading",       "NONE"},
        {"connections_waiting",       "NONE"},
        {"connections_writing",       "NONE"},
        {"content_length",            "NONE"},
        {"content_type",              "NONE"},
        {"date_local",                "NONE"},
        {"document_root",             "NONE"},
        {"document_uri",              "NONE"},
        {"fastcgi_path_info",         "NONE"},
        {"fastcgi_script_name",       "NONE"},
        {"geoip_area_code",           "NONE"},
        {"geoip_city",                "NONE"},
        {"geoip_city_continent_code", "NONE"},
        {"geoip_city_country_code",   "NONE"},
        {"geoip_city_country_code3",  "NONE"},
        {"geoip_city_country_name",   "NONE"},
        {"geoip_country_code",        "NONE"},
        {"geoip_country_code3",       "NONE"},
        {"geoip_country_name",        "NONE"},
        {"geoip_dma_code",            "NONE"},
        {"geoip_latitude",            "NONE"},
        {"geoip_longitude",           "NONE"},
        {"geoip_org",                 "NONE"},
        {"geoip_postal_code",         "NONE"},
        {"geoip_region",              "NONE"},
        {"geoip_region_name",         "NONE"},
        {"gzip_ratio",                "NONE"},
        {"host",                      "NONE"},
        {"hostname",                  "NONE"},
        {"http2",                     "NONE"},
        {"http_",                     "NONE"},
        {"https",                     "NONE"},
        {"invalid_referer",           "NONE"},
        {"is_args",                   "NONE"},
        {"limit_rate",                "NONE"},
        {"memcached_key",             "NONE"},
        {"modern_browser",            "NONE"},
        {"msie",                      "NONE"},
        {"nginx_version",             "NONE"},
        {"pid",                       "NONE"},
        {"pipe",                      "NONE"},
        {"proxy_add_x_forwarded_for", "NONE"},
        {"proxy_host",                "NONE"},
        {"proxy_port",                "NONE"},
        {"proxy_protocol_addr",       "NONE"},
        {"proxy_protocol_port",       "NONE"},
        {"realip_remote_port",        "NONE"},
        {"realpath_root",             "NONE"},
        {"remote_port",               "NONE"},
        {"remote_user",               "NONE"},
        {"request_body",              "NONE"},
        {"request_body_file",         "NONE"},
        {"request_completion",        "NONE"},
        {"request_filename",          "NONE"},
        {"request_id",                "NONE"},
        {"scheme",                    "NONE"},
        {"secure_link",               "NONE"},
        {"secure_link_expires",       "NONE"},
        {"sent_http_",                "NONE"},
        {"server_addr",               "NONE"},
        {"server_name",               "NONE"},
        {"server_port",               "NONE"},
        {"session_log_binary_id",     "NONE"},
        {"session_log_id",            "NONE"},
        {"slice_range",               "NONE"},
        {"spdy",                      "NONE"},
        {"spdy_request_priority",     "NONE"},
        {"ssl_cipher",                "NONE"},
        {"ssl_client_cert",           "NONE"},
        {"ssl_client_fingerprint",    "NONE"},
        {"ssl_client_i_dn",           "NONE"},
        {"ssl_client_raw_cert",       "NONE"},
        {"ssl_client_s_dn",           "NONE"},
        {"ssl_client_serial",         "NONE"},
        {"ssl_client_verify",         "NONE"},
        {"ssl_protocol",              "NONE"},
        {"ssl_server_name",           "NONE"},
        {"ssl_session_id",            "NONE"},
        {"ssl_session_reused",        "NONE"},
        {"tcpinfo_rtt",               "NONE"},
        {"tcpinfo_rttvar",            "NONE"},
        {"tcpinfo_snd_cwnd",          "NONE"},
        {"tcpinfo_rcv_space",         "NONE"},
        {"uid_got",                   "NONE"},
        {"uid_reset",                 "NONE"},
        {"uid_set",                   "NONE"},
        {"upstream_addr",             "NONE"},
        {"upstream_cache_status",     "NONE"},
        {"upstream_connect_time",     "NONE"},
        {"upstream_cookie_",          "NONE"},
        {"upstream_header_time",      "NONE"},
        {"upstream_http_",            "NONE"},
        {"upstream_response_length",  "NONE"},
        {"upstream_response_time",    "NONE"},
        {"upstream_status",           "NONE"} };

    // Access logs fields formats samples
    const std::unordered_map<std::string, QString> NGINX_ALF_SAMPLES{
        {"NONE",                  "<span style=\"color:#7f7f7f\">DISCARDED</span>"},
        {"date_time_epoch_s.ms",  "<b><span style=\"color:#00cc6f\">946771199.000</span></b>"},
        {"date_time_ncsa",        "<b><span style=\"color:#00cc6f\">01/Jan/2000:23:59:59 +0000</span></b>"},
        {"date_time_iso",         "<b><span style=\"color:#00cc6f\">2000-01-01T23:59:59+00:00</span></b>"},
        {"date_time_gmt",         "<b><span style=\"color:#00cc6f\">Saturday, 01-Jan-2000 23:59:59 UTC</span></b>"},
        {"request_full",          "<b><span style=\"color:#00cc6f\">GET /index.php?query=x HTTP/1.1</span></b>"},
        {"request_protocol",      "<b><span style=\"color:#00cc6f\">HTTP/1.1</span></b>"},
        {"request_method",        "<b><span style=\"color:#00cc6f\">GET</span></b>"},
        {"request_uri_query",     "<b><span style=\"color:#00cc6f\">/index.php?query=x</span></b>"},
        {"request_uri",           "<b><span style=\"color:#00cc6f\">/index.php</span></b>"},
        {"request_query",         "<b><span style=\"color:#00cc6f\">query=x</span></b>"},
        {"response_code",         "<b><span style=\"color:#00cc6f\">404</span></b>"},
        {"bytes_sent",            "<b><span style=\"color:#00cc6f\">1234</span></b>"},
        {"bytes_received",        "<b><span style=\"color:#00cc6f\">123</span></b>"},
        {"time_taken_s.ms",       "<b><span style=\"color:#00cc6f\">1.000</span></b>"},
        {"referer",               "<b><span style=\"color:#00cc6f\">http://www.referrer.site</span></b>"},
        {"cookie",                "<b><span style=\"color:#00cc6f\">aCookie=abc123</span></b>"},
        {"user_agent",            "<b><span style=\"color:#00cc6f\">UserAgent/3.0 (Details stuff) Info/123</span></b>"},
        {"client",                "<b><span style=\"color:#00cc6f\">192.168.1.123</span></b>"} };



    /////////////
    //// IIS ////

    //!< Access logs fields formats (W3C)
    const std::unordered_map<std::string, std::string> IIS_ALF{
        {"date",             "date_time_utc_d"},
        {"time",             "date_time_utc_t"},
        {"cs-version",       "request_protocol"},
        {"cs-method",        "request_method"},
        {"cs-uri-stem",      "request_uri"},
        {"cs-uri-query",     "request_query"},
        {"sc-status",        "response_code"},
        {"sc-bytes",         "bytes_sent"},
        {"cs-bytes",         "bytes_received"},
        {"time-taken",       "time_taken_ms"},
        {"cs(Referer)",      "referer"},
        {"cs(Cookie)",       "cookie"},
        {"cs(User-Agent)",   "user_agent"},
        {"c-ip",             "client"},
        // not in use, will be discarded
        {"s-sitename",        "NONE"},
        {"s-computername",    "NONE"},
        {"s-ip",              "NONE"},
        {"s-port",            "NONE"},
        {"cs-username",       "NONE"},
        {"cs-host",           "NONE"},
        {"sc-substatus",      "NONE"},
        {"sc-win32-status",   "NONE"},
        {"streamid",          "NONE"} };

    // Access logs fields formats samples
    const std::unordered_map<std::string, QString> IIS_ALF_SAMPLES{
        {"NONE",               "<span style=\"color:#7f7f7f\">DISCARDED</span>"},
        {"date_time_ncsa",     "<b><span style=\"color:#00cc6f\">01/Jan/2000:23:59:59 +0000</span></b>"},
        {"date_time_MDYYYY",   "<b><span style=\"color:#00cc6f\">1/1/2000</span></b>"},
        {"date_time_utc_d",    "<b><span style=\"color:#00cc6f\">2000-01-01</span></b>"},
        {"date_time_utc_t",    "<b><span style=\"color:#00cc6f\">23:59:59</span></b>"},
        {"request_full",       "<b><span style=\"color:#00cc6f\">GET /index.php?query=x HTTP/1.1</span></b>"},
        {"request_protocol",   "<b><span style=\"color:#00cc6f\">HTTP/1.1</span></b>"},
        {"request_method",     "<b><span style=\"color:#00cc6f\">GET</span></b>"},
        {"request_uri",        "<b><span style=\"color:#00cc6f\">/index.php</span></b>"},
        {"request_query",      "<b><span style=\"color:#00cc6f\">query=x</span></b>"},
        {"response_code",      "<b><span style=\"color:#00cc6f\">404</span></b>"},
        {"bytes_sent",         "<b><span style=\"color:#00cc6f\">1234</span></b>"},
        {"bytes_received",     "<b><span style=\"color:#00cc6f\">123</span></b>"},
        {"time_taken_ms",      "<b><span style=\"color:#00cc6f\">1000</span></b>"},
        {"referer",            "<b><span style=\"color:#00cc6f\">http://www.referrer.site</span></b>"},
        {"cookie",             "<b><span style=\"color:#00cc6f\">aCookie=abc123</span></b>"},
        {"user_agent",         "<b><span style=\"color:#00cc6f\">UserAgent/3.0+(Details+stuff)+Info/123</span></b>"},
        {"client",             "<b><span style=\"color:#00cc6f\">192.168.1.123</span></b>"} };

};


#endif // LOGDOCTOR__CRAPLOG__FORMATS_H

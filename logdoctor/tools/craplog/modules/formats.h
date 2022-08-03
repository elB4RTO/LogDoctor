#ifndef FORMAT_H
#define FORMAT_H

#include <QString>

#include <string>
#include <vector>
#include <unordered_map>


class FormatOps
{
public:
    FormatOps();

    // logs formats
    struct LogsFormat {
        std::string string;
        std::string initial;
        std::string final;
        std::vector<std::string> separators;
        std::vector<std::string> fields;
    };

    const LogsFormat processApacheFormatString(const std::string& formatted_string, const int log_type );
    const LogsFormat processNginxFormatString(const std::string& formatted_string, const int log_type );
    const LogsFormat processIisFormatString(const std::string& formatted_string, const int log_module );

    const QString getApacheLogSample( const LogsFormat& log_format, const int log_type );
    const QString getNginxLogSample( const LogsFormat& log_format, const int log_type );
    const QString getIisLogSample( const LogsFormat& log_format/*, const int log_type*/ );

private:
    /*std::unordered_map<std::string, std::string>
        APACHE_ALF,   APACHE_ELF,
        NGINX_ALF,    NGINX_ELF,
        IIS_ALF,      IIS_ELF;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
        APACHE_ALF_V,  APACHE_ELF_V;*/

    /*
    const std::vector<std::string> A_ALFs = {"%h", "%t", "%t_u", "%t_ctime", "%r", "%>s", "%O", "%I", "%T", "%D", "%T/%D", "%{Referer}i", "%{User-agent}i",
                             /*NOT IN USE->* /"%l", "%u", "%b", "%B", "%S", "%a", "h_p", "%{x-forwarded-for}", "%radd", "%v", "%V", "%rh"};

    const std::vector<std::string> A_ELFs = {"%a", "%l", "%M", "%F", "%t", "%{u}t", "%{cu}t",
                             /*NOT IN USE->* /"%{c}a", "%A", "%{name}e", "%E", "%F", "%{name}i", "%k", "%L", "%{c}L", "%{C}L", "%m", "%{name}n", "%P", "%T", "%{g}T", "%v", "%V"};

    */

    // list of access logs fields formats
    const std::unordered_map<std::string, std::vector<std::string>> A_ALFs_v = {
        {"h", {"c"} },
        {"t", {"sec","msec","usec","\%b","\%B","\%c","\%d","\%D","\%e","\%F","\%h","\%H","\%m","\%M","\%r","\%R","\%S","\%T","\%x","\%X","\%y","\%Y"} },
        {"i", {"Cookie","User-agent","Referer"} },
        {"T", {"s","ms","us"} },
        // composed not in use
        {"a",   {}},
        {"c",   {}},
        {"e",   {}},
        {"n",   {}},
        {"o",   {}},
        {"p",   {}},
        {"P",   {}},
        {"^ti", {}},
        {"^to", {}}
        };

    const std::vector<std::string> N_ALFs = {"$remote_addr", "$time_local", "$time_iso8601", "$msec", "$request", "$status", "$bytes_sent", "$request_length", "$request_time", "$http_referer", "$http_user_agent",
                             /*NOT IN USE->*/"$remote_user", "$gzip_ratio", "$connection", "$connection_requests", "$pipe"};

    const std::vector<std::string> I_ALFs = {"c-ip", "time", "cs-version", "cs-method", "cs-uri-stem", "cs-uri-query", "sc-status", "sc-bytes", "cs-bytes", "time-taken", "cs(Referrer)", "cs(Cookie)", "cs(User-Agent)",
                             /*NOT IN USE->*/"s-sitename", "s-computername", "s-ip", "s-port", "cs-username", "cs-host", "sc-substatus", "sc-win32-status", "streamid"};

    // list error logs fields formats
    const std::vector<std::string> N_ELFs = {"$time_iso8601", "$error_level", "$error_message",
                             /*NOT IN USE->*/"$pid", "$cid"};

    const std::vector<std::string> I_ELFs = {"",
                             /*NOT IN USE->*/};





    /////////////////
    //// APACHE2 ////
    // access logs fields formats
    const std::unordered_map<std::string, std::string> APACHE_ALF = {
        {"\%h",  "client"},
        {"\%t",  "date_time_ncsa"},
        {"\%r",  "request_full"},
        {"\%H",  "request_protocol"},
        {"\%m",  "request_method"},
        {"\%U",  "request_uri"},
        {"\%q",  "request_query"},
        {"\%s",  "response_code"},
        {"\%>s", "response_code"},
        {"\%O",  "bytes_sent"},
        {"\%I",  "bytes_received"},
        {"\%T",  "time_taken_s"},
        {"\%D",  "time_taken_ms"},
        // not in use, will be discarded
        {"\%a",  "NONE"},
        {"\%A",  "NONE"},
        {"\%b",  "NONE"},
        {"\%B",  "NONE"},
        {"\%f",  "NONE"},
        {"\%k",  "NONE"},
        {"\%l",  "NONE"},
        {"\%L",  "NONE"},
        {"\%p",  "NONE"},
        {"\%P",  "NONE"},
        {"\%R",  "NONE"},
        {"\%S",  "NONE"},
        {"\%u",  "NONE"},
        {"\%v",  "NONE"},
        {"\%V",  "NONE"},
        {"\%X",  "NONE"},
        {"\%{x-forwarded-for}",  "NONE"} };

    // composed items
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> APACHE_ALF_V = {
        {"h", { {"c",          "client"} } },
        {"t", { {"sec",        "date_time_epoch_s"},
                {"msec",       "date_time_epoch_ms"},
                {"usec",       "date_time_epoch_us"},
                {"\%b",         "date_time_month_str"},
                {"\%B",         "date_time_month_str"},
                {"\%c",         "date_time_mcs"},
                {"\%d",         "date_time_day"},
                {"\%D",         "date_time_MMDDYY"},
                {"\%e",         "date_time_day"},
                {"\%F",         "date_time_YYYYMMDD"},
                {"\%h",         "date_time_month_str"},
                {"\%H",         "date_time_hour"},
                {"\%m",         "date_time_month"},
                {"\%M",         "date_time_minute"},
                //{"\%p",         "date_time_clock_meridian"},
                {"\%r",         "date_time_clock_12"},
                {"\%R",         "date_time_clock_short"},
                {"\%S",         "date_time_second"},
                {"\%T",         "date_time_clock_24"},
                {"\%x",         "date_time_MMDDYY"},
                {"\%X",         "date_time_clock_24"},
                {"\%y",         "date_time_year_short"},
                {"\%Y",         "date_time_year"} } },
        {"i", { {"Cookie",     "cookie"},
                {"User-agent", "user_agent"},
                {"Referer",    "referer"} } },
        {"T", { {"s",          "time_taken_s"},
                {"ms",         "time_taken_ms"},
                {"us",         "time_taken_us"} } },
        // composed not in use
        {"a",   {}},
        {"c",   {}},
        {"e",   {}},
        {"n",   {}},
        {"o",   {}},
        {"p",   {}},
        {"P",   {}},
        {"^ti", {}},
        {"^to", {}} };

    // access logs fields formats samples
    const std::unordered_map<std::string, QString> APACHE_ALF_SAMPLES = {
        {"NONE",                  "DISCARDED"},
        {"date_time_epoch_s",     "946771199"},
        {"date_time_epoch_ms",    "946771199000"},
        {"date_time_epoch_us",    "946771199000000"},
        {"date_time_ncsa",        "[01/Jan/2000:23:59:59 +0000]"},
        {"date_time_mcs",         "Sat Jan 01 23:59:59 2000"},
        {"date_time_YYYYMMDD",    "2000-01-01"},
        {"date_time_MMDDYY",      "01/01/00"},
        {"date_time_year",        "2000"},
        {"date_time_year_short",  "00"},
        {"date_time_month_str",   "January"},
        {"date_time_month",       "01"},
        {"date_time_day",         "01"},
        {"date_time_clock_12",    "11:59:59 pm"},
        {"date_time_clock_24",    "23:59:59"},
        {"date_time_clock_short", "23:59"},
        {"date_time_hour",        "23"},
        {"date_time_minute",      "59"},
        {"date_time_second",      "59"},
        {"request_full",          "GET /index.php?query=x HTTP/1.1"},
        {"request_protocol",      "HTTP/1.1"},
        {"request_method",        "GET"},
        {"request_uri",           "/index.php"},
        {"request_query",         "query=x"},
        {"response_code",         "404"},
        {"bytes_sent",            "1234"},
        {"bytes_received",        "123"},
        {"time_taken_s",          "1"},
        {"time_taken_ms",         "1000"},
        {"time_taken_us",         "1000000"},
        {"referer",               "http://www.referrer.site"},
        {"cookie",                "aCookie=abc123"},
        {"user_agent",            "UserAgent/3.0 (Details stuff) Info/123"},
        {"client",                "192.168.1.123"} };

    // error logs fields formats
    const std::unordered_map<std::string, std::string> APACHE_ELF = {
        {"\%a", "client:port"},
        {"\%l", "error_level"},
        {"\%M", "error_message"},
        {"\%F", "source_file"},
        {"\%t", "date_time_mcs"},
        // not in use, will be discarded
        {"\%A", "NONE"},
        {"\%E", "NONE"},
        {"\%F", "NONE"},
        {"\%k", "NONE"},
        {"\%L", "NONE"},
        {"\%m", "NONE"},
        {"\%P", "NONE"},
        {"\%T", "NONE"},
        {"\%v", "NONE"},
        {"\%V", "NONE"} };

    // composed
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> APACHE_ELF_V = {
        {"t", { {"u", "date_time_mcs"},
                {"cu","date_time_iso_mcs"} } },
        // composed not in use
        {"a", {}},
        {"e", {}},
        {"i", {}},
        {"L", {}},
        {"n", {}},
        {"T", {}} };

    // error logs fields formats samples
    const std::unordered_map<std::string, QString> APACHE_ELF_SAMPLES = {
        {"NONE",              "DISCARDED"},
        {"date_time_mcs",     "Sat Jan 01 23:59:59.123456 0000"},
        {"date_time_iso_mcs", "2000-01-01 23:59:59.123456"},
        {"client:port",       "192.168.1.123:54321"},
        {"error_level",       "group:level"},
        {"error_message",     "AH00123: some infoes: some details"},
        {"source_file",       "file.c"} };


    ///////////////
    //// NGINX ////
    // access logs fields formats (only the ones considered)
    const std::unordered_map<std::string, std::string> NGINX_ALF = {
        {"$remote_addr",       "client"},
        {"$time_local",        "date_time_ncsa"},
        {"$time_iso8601",      "date_time_iso"},
        {"msec",               "date_time_epoch_s.ms"},
        {"$request",           "request_full"},
        {"$status",            "response_code"},
        {"$bytes_sent",        "bytes_sent"},
        {"$request_length",    "bytes_received"},
        {"$request_time",      "time_taken_s.ms"},
        {"$http_referer",      "referer"},
        {"$http_user_agent",   "user_agent"},
        // not in use, will be discarded
        {"$remote_user",           "NONE"},
        {"$gzip_ratio",            "NONE"},
        {"$connection",            "NONE"},
        {"$connection_requests",   "NONE"},
        {"pipe",                   "NONE"} };

    // nginx logs fields formats samples
    const std::unordered_map<std::string, QString> NGINX_ALF_SAMPLES = {
        {"NONE",                  "DISCARDED"},
        {"date_time_epoch_s.ms",  "946771199.000"},
        {"date_time_iso",         "2000-01-01T23:59:59+00:00]"},
        {"date_time_mcs",         "Sat Jan 01 23:59:59 2000"},
        {"request_full",          "GET /index.php?query=x HTTP/1.1"},
        {"response_code",         "404"},
        {"bytes_sent",            "1234"},
        {"bytes_received",        "123"},
        {"time_taken_s.ms",       "1.000"},
        {"referer",               "http://www.referrer.site"},
        {"user_agent",            "UserAgent/3.0 (Details stuff) Info/123"},
        {"client",                "192.168.1.123"} };

    // error logs fields formats (only the ones considered)
    const std::unordered_map<std::string, std::string> NGINX_ELF = {
        {"$time_iso8601",    "date_time_iso"},
        {"$error_level",     "error_level"},
        {"$error_message",   "error_message"},
        // not in use, will be discarded
        {"$pid",             "NONE"},
        {"$cid",             "NONE"} };

    // error logs fields formats samples
    const std::unordered_map<std::string, QString> NGINX_ELF_SAMPLES = {
        {"NONE",             "DISCARDED"},
        {"date_time_iso",    "2000-01-01T23:59:59+00:00]"},
        {"error_level",      "[level]"},
        {"error_message",    "123#456: some infoes"} };


    /////////////
    //// IIS ////
    // access logs fields formats
    const std::unordered_map<std::string, std::string> IIS_ALF = {
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
        {"cs(Referrer)",     "referer"},
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

    // access logs fields formats samples
    const std::unordered_map<std::string, QString> IIS_ALF_SAMPLES = {
        {"NONE",               "DISCARDED"},
        {"date_time_utc_d",    "2000-01-01"},
        {"date_time_utc_t",    "23:59:59"},
        {"request_protocol",   "HTTP/1.1"},
        {"request_method",     "GET"},
        {"request_uri",        "/index.php"},
        {"request_query",      "query=x"},
        {"response_code",      "404"},
        {"bytes_sent",         "1234"},
        {"bytes_received",     "123"},
        {"time_taken_ms",      "1000"},
        {"referer",            "http://www.referrer.site"},
        {"cookie",             "aCookie=abc123"},
        {"user_agent",         "UserAgent/3.0+(Details+stuff)+Info/123"},
        {"client",             "192.168.1.123"} };

    // error logs fields formats
    const std::unordered_map<std::string, std::string> IIS_ELF = {
        {"",   ""},
        // not in use, will be discarded
        {"",   "NONE"} };

    // error logs fields formats samples
    const std::unordered_map<std::string, QString> IIS_ELF_SAMPLES = {
        {"NONE", "DISCARDED"} };

};

#endif // FORMAT_H

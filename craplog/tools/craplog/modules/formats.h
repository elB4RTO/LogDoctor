#ifndef FORMAT_H
#define FORMAT_H


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

    FormatOps::LogsFormat processFormatString( std::string formatted_string, int log_type, int web_server_id );

private:
    std::unordered_map<std::string, std::string>
        APACHE2_ALF,  APACHE2_ELF,
        NGINX_ALF,    NGINX_ELF,
        IIS_ALF,      IIS_ELF;

    // list of access logs fields formats (only the ones considered)
    std::vector<std::string> A_ALFs = {"%h", "%t", "%t_u", "%t_ctime", "%r", "%>s", "%O", "%I", "%T", "%D", "%T/%D", "%{Referer}i", "%{User-agent}i",
                                       /*NOT IN USE->*/"%l", "%u", "%b", "%B", "%S", "%a", "h_p", "%{x-forwarded-for}", "%radd", "%v", "%V", "%rh"};

    std::vector<std::string> N_ALFs = {"$remote_addr", "$time_local", "$time_iso8601", "$request", "$status", "$bytes_sent", "$request_length", "$request_time", "$http_referer", "$http_user_agent",
                                       /*NOT IN USE->*/"$remote_user", "$gzip_ratio", "$connection", "$connection_requests", "$msec", "$pipe"};

    std::vector<std::string> I_ALFs = {"c-ip", "time", "cs-method", "cs-uri-stem", "sc-status", "sc-bytes", "cs-bytes", "time-taken", "cs(Referrer)", "cs(User-Agent)",
                                       /*NOT IN USE->*/"s-sitename", "s-computername", "s-ip", "cs-uri-query", "s-port", "cs-username", "cs-version", "cs(Cookie)", "cs-host", "sc-substatus", "sc-win32-status", "streamid"};

    // list error logs fields formats (only the ones considered)
    std::vector<std::string> A_ELFs = {"%a", "%l", "%M", "%F", "%t", "%{u}t", "%{cu}t",
                                       /*NOT IN USE->*/"%{c}a", "%A", "%{name}e", "%E", "%F", "%{name}i", "%k", "%L", "%{c}L", "%{C}L", "%m", "%{name}n", "%P", "%T", "%{g}T", "%v", "%V"};

    std::vector<std::string> N_ELFs = {"", "", "", "", "",
                                       /*NOT IN USE->*/};

    std::vector<std::string> I_ELFs = {"", "", "", "", "",
                                       /*NOT IN USE->*/};

};

#endif // FORMAT_H

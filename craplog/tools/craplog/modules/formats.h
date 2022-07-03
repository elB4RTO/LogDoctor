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

    LogsFormat processFormatString(const std::string& formatted_string, const int log_type, const int web_server_id ) const;
    LogsFormat processApacheFormatString(const std::string& formatted_string, const int log_type ) const;

private:
    std::unordered_map<std::string, std::string>
        APACHE_ALF,   APACHE_ELF,
        NGINX_ALF,    NGINX_ELF,
        IIS_ALF,      IIS_ELF;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
        APACHE_ALF_V,  APACHE_ELF_V;

    /*
    const std::vector<std::string> A_ALFs = {"%h", "%t", "%t_u", "%t_ctime", "%r", "%>s", "%O", "%I", "%T", "%D", "%T/%D", "%{Referer}i", "%{User-agent}i",
                             /*NOT IN USE->* /"%l", "%u", "%b", "%B", "%S", "%a", "h_p", "%{x-forwarded-for}", "%radd", "%v", "%V", "%rh"};

    const std::vector<std::string> A_ELFs = {"%a", "%l", "%M", "%F", "%t", "%{u}t", "%{cu}t",
                             /*NOT IN USE->* /"%{c}a", "%A", "%{name}e", "%E", "%F", "%{name}i", "%k", "%L", "%{c}L", "%{C}L", "%m", "%{name}n", "%P", "%T", "%{g}T", "%v", "%V"};

    */

    // list of access logs fields formats
   const  std::vector<std::string> N_ALFs = {"$remote_addr", "$time_local", "$time_iso8601", "$msec", "$request", "$status", "$bytes_sent", "$request_length", "$request_time", "$http_referer", "$http_user_agent",
                             /*NOT IN USE->*/"$remote_user", "$gzip_ratio", "$connection", "$connection_requests", "$pipe"};

    const std::vector<std::string> I_ALFs = {"c-ip", "time", "cs-version", "cs-method", "cs-uri-stem", "cs-uri-query", "sc-status", "sc-bytes", "cs-bytes", "time-taken", "cs(Referrer)", "cs(Cookie)", "cs(User-Agent)",
                             /*NOT IN USE->*/"s-sitename", "s-computername", "s-ip", "s-port", "cs-username", "cs-host", "sc-substatus", "sc-win32-status", "streamid"};

    // list error logs fields formats
    const std::vector<std::string> N_ELFs = {"$time_iso8601", "$error_level", "$pid", "$cid", "$error_message",
                             /*NOT IN USE->*/};

    const std::vector<std::string> I_ELFs = {"", "", "", "", "",
                             /*NOT IN USE->*/};

};

#endif // FORMAT_H

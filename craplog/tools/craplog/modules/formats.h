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

    FormatOps::LogsFormat processApacheALF( std::string fs );

private:
    std::unordered_map<std::string, std::string> APACHE2_ALF, APACHE2_ELF;
    std::unordered_map<std::string, std::string> NGINX_ALF,   NGINX_ELF;
    std::unordered_map<std::string, std::string> IIS_ALF,     IIS_ELF;

    // list of access logs fields formats (only the ones considered)
    std::vector<std::string> A_ALFs = {"%h","%t","%t_u","%t_ctime","%r","%>s","%I","%O","%T","%D","%T/%D","%{Referer}i","%{User-agent}i"};
    // list error logs fields formats (only the ones considered)
    std::vector<std::string> A_ELFs = {"%a","%l","%M","%t","%{u}t","%{cu}t"};

};

#endif // FORMAT_H

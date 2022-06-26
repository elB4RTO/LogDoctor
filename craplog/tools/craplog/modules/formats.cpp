
#include "formats.h"



FormatOps::FormatOps()
{
    // access logs fields formats (only the ones considered)
    this->APACHE2_ALF["%h"] = "client";
    this->APACHE2_ALF["%t"] = "date_time";
    this->APACHE2_ALF["%t_u"] = "date_time";
    this->APACHE2_ALF["%t_ctime"] = "date_time";
    this->APACHE2_ALF["%r"] = "request";
    this->APACHE2_ALF["%>s"] = "response_code";
    this->APACHE2_ALF["%I"] = "bytes_received";
    this->APACHE2_ALF["%O"] = "bytes_sent";
    this->APACHE2_ALF["%T"] = "time_taken";
    this->APACHE2_ALF["%D"] = "time_taken";
    this->APACHE2_ALF["%T/%D"] = "time_taken";
    this->APACHE2_ALF["%{Referer}i"] = "referer";
    this->APACHE2_ALF["%{User-agent}i"] = "user_agent";
    // error logs fields formats (only the ones considered)
    this->APACHE2_ELF["%a"] = "client + port";
    this->APACHE2_ELF["%l"] = "error_level";
    this->APACHE2_ELF["%M"] = "error_message";
    this->APACHE2_ELF["%t"] = "date_time";
    this->APACHE2_ELF["%{u}t"] = "date_time";
    this->APACHE2_ELF["%{cu}t"] = "date_time";
}


FormatOps::LogsFormat FormatOps::processApacheALF( std::string fs )
{
    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    int n_fld=0,
        start=0, aux=0, stop=-1,
        min_dist=0, max_dist=fs.size();
    std::string cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last field
        start = stop;
        cur_fld = "";
        min_dist = max_dist;
        // find the next field
        for ( std::string& fld : this->A_ALFs ) {
            while (true) {
                // run untill a valid field is found
                aux = fs.find( fld, stop );
                if ( aux >= 0 && aux < min_dist ) {
                    if ( fs[min_dist-1] == '\\' ) {
                        // false positive, backslashed percent sign
                        stop = aux+1;
                        continue;
                    }
                    // a nearer field has been found
                    min_dist = aux;
                    cur_fld  = fld;
                }
                // break if found or not found, not in case of false positive
                break;
            }
        }
        // break if no more fields was found
        if ( cur_fld == "" ) {
            // append the last section as final separator
            final = fs.substr( start );
            break;
        }
        // get the current separator
        cur_sep = fs.substr( start, min_dist-start-1 );
        if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = cur_sep;
        } else {
            // append to separators list
            separators.push_back( cur_sep );
        }
        n_fld++;
        // append the current field, converted
        fields.push_back( this->APACHE2_ALF[ cur_fld ] );
        // step at the end of the current field for the next start
        stop = min_dist + cur_fld.size();
    }

    return FormatOps::LogsFormat{
            .string  = fs,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };
}


#include "formats.h"

using std::string, std::vector, std::unordered_map;


FormatOps::FormatOps()
{
    /////////////////
    //// APACHE2 ////
    // access logs fields formats (only the ones considered)
    this->APACHE2_ALF["%h"]             = "client";
    this->APACHE2_ALF["%t"]             = "date_time";
    this->APACHE2_ALF["%t_u"]           = "date_time_utc";
    this->APACHE2_ALF["%t_ctime"]       = "date_time_ct";
    this->APACHE2_ALF["%r"]             = "request";
    this->APACHE2_ALF["%>s"]            = "response_code";
    this->APACHE2_ALF["%O"]             = "bytes_sent";
    this->APACHE2_ALF["%I"]             = "bytes_received";
    this->APACHE2_ALF["%T"]             = "time_taken";
    this->APACHE2_ALF["%D"]             = "time_taken";
    this->APACHE2_ALF["%T/%D"]          = "time_taken";
    this->APACHE2_ALF["%{Referer}i"]    = "referer";
    this->APACHE2_ALF["%{User-agent}i"] = "user_agent";
    // not in use, will be discarded
    this->APACHE2_ALF["%l"]                 = "NONE";
    this->APACHE2_ALF["%u"]                 = "NONE";
    this->APACHE2_ALF["%b"]                 = "NONE";
    this->APACHE2_ALF["%B"]                 = "NONE";
    this->APACHE2_ALF["%S"]                 = "NONE";
    this->APACHE2_ALF["%a"]                 = "NONE";
    this->APACHE2_ALF["h_p"]                = "NONE";
    this->APACHE2_ALF["%{x-forwarded-for}"] = "NONE";
    this->APACHE2_ALF["%radd"]              = "NONE";
    this->APACHE2_ALF["%v"]                 = "NONE";
    this->APACHE2_ALF["%V"]                 = "NONE";
    this->APACHE2_ALF["%rh"]                = "NONE";
    // error logs fields formats (only the ones considered)
    this->APACHE2_ELF["%a"]     = "client:port";
    this->APACHE2_ELF["%l"]     = "error_level";
    this->APACHE2_ELF["%M"]     = "error_message";
    this->APACHE2_ELF["%F"]     = "source_file";
    this->APACHE2_ELF["%t"]     = "date_time";
    this->APACHE2_ELF["%{u}t"]  = "date_time_mcs";
    this->APACHE2_ELF["%{cu}t"] = "date_time_iso_mcs";
    // not in use, will be discarded
    this->APACHE2_ELF["%{c}a"]    = "NONE";
    this->APACHE2_ELF["%A"]       = "NONE";
    this->APACHE2_ELF["%{name}e"] = "NONE";
    this->APACHE2_ELF["%E"]       = "NONE";
    this->APACHE2_ELF["%F"]       = "NONE";
    this->APACHE2_ELF["%{name}i"] = "NONE";
    this->APACHE2_ELF["%k"]       = "NONE";
    this->APACHE2_ELF["%L"]       = "NONE";
    this->APACHE2_ELF["%{c}L"]    = "NONE";
    this->APACHE2_ELF["%{C}L"]    = "NONE";
    this->APACHE2_ELF["%m"]       = "NONE";
    this->APACHE2_ELF["%{name}n"] = "NONE";
    this->APACHE2_ELF["%P"]       = "NONE";
    this->APACHE2_ELF["%T"]       = "NONE";
    this->APACHE2_ELF["%{g}T"]    = "NONE";
    this->APACHE2_ELF["%v"]       = "NONE";
    this->APACHE2_ELF["%V"]       = "NONE";

    ///////////////
    //// NGINX ////
    // access logs fields formats (only the ones considered)
    this->NGINX_ALF["$remote_addr"]     = "client";
    this->NGINX_ALF["$time_local"]      = "date_time";
    this->NGINX_ALF["$time_iso8601"]    = "date_time_iso";
    this->NGINX_ALF["$request"]         = "request";
    this->NGINX_ALF["$status"]          = "response_code";
    this->NGINX_ALF["$bytes_sent"]      = "bytes_sent";
    this->NGINX_ALF["$request_length"]  = "bytes_received";
    this->NGINX_ALF["$request_time"]    = "time_taken";
    this->NGINX_ALF["$http_referer"]    = "referer";
    this->NGINX_ALF["$http_user_agent"] = "user_agent";
    // not in use, will be discarded
    this->NGINX_ALF["$remote_user"]         = "NONE";
    this->NGINX_ALF["$gzip_ratio"]          = "NONE";
    this->NGINX_ALF["$connection"]          = "NONE";
    this->NGINX_ALF["$connection_requests"] = "NONE";
    this->NGINX_ALF["msec"]                 = "NONE";
    this->NGINX_ALF["pipe"]                 = "NONE";
    // error logs fields formats (only the ones considered)
    this->NGINX_ELF[""] = "client:port";
    this->NGINX_ELF[""] = "error_level";
    this->NGINX_ELF[""] = "error_message";
    this->NGINX_ELF[""] = "source_file";
    this->NGINX_ELF[""] = "date_time";
    // not in use, will be discarded
    this->NGINX_ELF[""] = "NONE";

    /////////////
    //// IIS ////
    // access logs fields formats (only the ones considered)
    this->IIS_ALF["c-ip"]           = "client";
    this->IIS_ALF["time"]           = "date_time_utc";
    this->IIS_ALF["cs-uri-stem"]    = "request";
    this->IIS_ALF["sc-status"]      = "response_code";
    this->IIS_ALF["sc-bytes"]       = "bytes_sent";
    this->IIS_ALF["cs-bytes"]       = "bytes_received";
    this->IIS_ALF["time-taken"]     = "time_taken";
    this->IIS_ALF["cs(Referrer)"]   = "referer";
    this->IIS_ALF["cs(User-Agent)"] = "user_agent";
    // not in use, will be discarded
    this->IIS_ALF["s-sitename"]      = "NONE";
    this->IIS_ALF["s-computername"]  = "NONE";
    this->IIS_ALF["s-ip"]            = "NONE";
    this->IIS_ALF["cs-uri-query"]    = "NONE";
    this->IIS_ALF["s-port"]          = "NONE";
    this->IIS_ALF["cs-username"]     = "NONE";
    this->IIS_ALF["cs-version"]      = "NONE";
    this->IIS_ALF["cs(Cookie)"]      = "NONE";
    this->IIS_ALF["cs-host"]         = "NONE";
    this->IIS_ALF["sc-substatus"]    = "NONE";
    this->IIS_ALF["sc-win32-status"] = "NONE";
    this->IIS_ALF["streamid"]        = "NONE";
    // error logs fields formats (only the ones considered)
    this->IIS_ELF[""] = "client:port";
    this->IIS_ELF[""] = "error_level";
    this->IIS_ELF[""] = "error_message";
    this->IIS_ELF[""] = "source_file";
    this->IIS_ELF[""] = "date_time";
    // not in use, will be discarded
    this->IIS_ELF[""] = "NONE";
}


FormatOps::LogsFormat FormatOps::processFormatString(string f_str, int l_type, int ws_id )
{
    unordered_map<string, string> f_map;
    vector<string> f_flds;
    switch ( ws_id )  {
        case 11:
            switch ( l_type ) {
                case 1:
                    f_map  = this->APACHE2_ALF;
                    f_flds = this->A_ALFs;
                    break;
                case 2:
                    f_map  = this->APACHE2_ELF;
                    f_flds = this->A_ELFs;
                    break;
                default:
                    // shouldn't be here
                    throw (&"Wrong LogType for Apache: "[l_type]);
                }
            break;
        case 12:
            switch ( l_type ) {
                case 1:
                    f_map  = this->NGINX_ALF;
                    f_flds = this->N_ALFs;
                    break;
                case 2:
                    f_map  = this->NGINX_ELF;
                    f_flds = this->N_ELFs;
                    break;
                default:
                    // shouldn't be here
                    throw (&"Wrong LogType for Nginx: "[l_type]);
                }
            break;
        case 13:
            switch ( l_type ) {
                case 1:
                    f_map  = this->IIS_ALF;
                    f_flds = this->I_ALFs;
                    break;
                case 2:
                    f_map  = this->IIS_ELF;
                    f_flds = this->I_ELFs;
                    break;
                default:
                    // shouldn't be here
                    throw (&"Wrong LogType for IIS: "[l_type]);
                }
            break;
        default:
            // shouldn't be here
            throw (&"Wrong WebServerID: "[ws_id]);
    }

    string initial="", final="";
    vector<string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    int n_fld=0,
        start=0, aux=0, stop=0,
        min_dist=0, max_dist=f_str.size();
    string cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last field
        start = stop;
        cur_fld = "";
        min_dist = max_dist;
        // find the next field
        for ( string& fld : f_flds ) {
            while (true) {
                // run untill a valid field is found
                aux = f_str.find( fld, stop );
                if ( aux >= 0 && aux < min_dist ) {
                    if ( f_str[min_dist-1] == '\\' ) {
                        // backslashed, may be a false positive
                        int n_slashes = 1, j=min_dist-2;
                        while (true) {
                            if ( j < 0 ) {
                                break;
                            }
                            if ( f_str[ j ] == '\\' ) {
                                n_slashes ++;
                                j --;
                            } else {
                                break;
                            }
                        }
                        if ( n_slashes % 2 == 1 ) {
                            // odd number of BS, supposed to result in a backslashed keyword
                            stop = aux+1;
                            continue;
                        }
                    }
                    // a nearer field has been found
                    min_dist = aux;
                    cur_fld  = fld;
                }
                // break if found or not found, not reached in case of false positive
                break;
            }
        }
        // break if no more fields was found
        if ( cur_fld == "" ) {
            // append the last section as final separator
            final = f_str.substr( start );
            break;
        }
        // get the current separator
        cur_sep = f_str.substr( start, min_dist-start );
        if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = f_str.substr( start, min_dist-start );
        } else {
            // append to separators list
            separators.push_back( cur_sep );
        }
        n_fld++;
        // append the current field, converted
        if ( f_map[ cur_fld ] == "client:port" ) {
            fields.push_back( "client" );
            separators.push_back( ":" );
            fields.push_back( "port" );
        } else {
            fields.push_back( f_map[ cur_fld ] );
        }
        // step at the end of the current field for the next start
        stop = min_dist + cur_fld.size();
    }

    return FormatOps::LogsFormat{
            .string  = f_str,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };
}


#include "formats.h"

#include "modules/dialogs.h"


FormatOps::FormatOps()
{
    /*
    /////////////////
    //// APACHE2 ////
    // access logs fields formats (only the ones considered)
    this->APACHE_ALF["\%h"]  = "client";
    this->APACHE_ALF["\%t"]  = "date_time_ncsa";
    this->APACHE_ALF["\%r"]  = "request_full";
    this->APACHE_ALF["\%H"]  = "request_protocol";
    this->APACHE_ALF["\%m"]  = "request_method";
    this->APACHE_ALF["\%U"]  = "request_uri";
    this->APACHE_ALF["\%q"]  = "request_query";
    this->APACHE_ALF["\%s"]  = "response_code";
    this->APACHE_ALF["\%>s"] = "response_code";
    this->APACHE_ALF["\%O"]  = "bytes_sent";
    this->APACHE_ALF["\%I"]  = "bytes_received";
    this->APACHE_ALF["\%T"]  = "time_taken_s";
    this->APACHE_ALF["\%D"]  = "time_taken_ms";
    // composed items (only the ones considered)
    this->APACHE_ALF_V["h"] = { {"c",          "client"} };
    this->APACHE_ALF_V["t"] = { {"sec",        "date_time_epoch_s"},
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
                                {"\%Y",         "date_time_year"} };
    this->APACHE_ALF_V["i"] = { {"Cookie",     "cookie"},
                                {"User-agent", "user_agent"},
                                {"Referer",    "referer"} };
    this->APACHE_ALF_V["T"] = { {"s",          "time_taken_s"},
                                {"ms",         "time_taken_ms"},
                                {"us",         "time_taken_us"} };
    // not in use, will be discarded
    this->APACHE_ALF["\%a"] = "NONE";
    this->APACHE_ALF["\%A"] = "NONE";
    this->APACHE_ALF["\%b"] = "NONE";
    this->APACHE_ALF["\%B"] = "NONE";
    this->APACHE_ALF["\%f"] = "NONE";
    this->APACHE_ALF["\%k"] = "NONE";
    this->APACHE_ALF["\%l"] = "NONE";
    this->APACHE_ALF["\%L"] = "NONE";
    this->APACHE_ALF["\%p"] = "NONE";
    this->APACHE_ALF["\%P"] = "NONE";
    this->APACHE_ALF["\%R"] = "NONE";
    this->APACHE_ALF["\%S"] = "NONE";
    this->APACHE_ALF["\%u"] = "NONE";
    this->APACHE_ALF["\%v"] = "NONE";
    this->APACHE_ALF["\%V"] = "NONE";
    this->APACHE_ALF["\%X"] = "NONE";
    this->APACHE_ALF["\%{x-forwarded-for}"] = "NONE";
    // composed not in use
    this->APACHE_ALF_V["a"]   = {};
    this->APACHE_ALF_V["c"]   = {};
    this->APACHE_ALF_V["e"]   = {};
    this->APACHE_ALF_V["n"]   = {};
    this->APACHE_ALF_V["o"]   = {};
    this->APACHE_ALF_V["p"]   = {};
    this->APACHE_ALF_V["P"]   = {};
    this->APACHE_ALF_V["^ti"] = {};
    this->APACHE_ALF_V["^to"] = {};

    // error logs fields formats (only the ones considered)
    this->APACHE_ELF["\%a"] = "client:port";
    this->APACHE_ELF["\%l"] = "error_level";
    this->APACHE_ELF["\%M"] = "error_message";
    this->APACHE_ELF["\%F"] = "source_file";
    this->APACHE_ELF["\%t"] = "date_time_mcs";
    // composed
    this->APACHE_ELF_V["t"] = { {"u", "date_time_mcs"},
                                {"cu","date_time_iso_mcs"}};
    // not in use, will be discarded
    this->APACHE_ELF["\%A"] = "NONE";
    this->APACHE_ELF["\%E"] = "NONE";
    this->APACHE_ELF["\%F"] = "NONE";
    this->APACHE_ELF["\%k"] = "NONE";
    this->APACHE_ELF["\%L"] = "NONE";
    this->APACHE_ELF["\%m"] = "NONE";
    this->APACHE_ELF["\%P"] = "NONE";
    this->APACHE_ELF["\%T"] = "NONE";
    this->APACHE_ELF["\%v"] = "NONE";
    this->APACHE_ELF["\%V"] = "NONE";
    // composed not in use
    this->APACHE_ELF_V["a"] = {};
    this->APACHE_ELF_V["e"] = {};
    this->APACHE_ELF_V["i"] = {};
    this->APACHE_ELF_V["L"] = {};
    this->APACHE_ELF_V["n"] = {};
    this->APACHE_ELF_V["T"] = {};

    ///////////////
    //// NGINX ////
    // access logs fields formats (only the ones considered)
    this->NGINX_ALF["$remote_addr"]     = "client";
    this->NGINX_ALF["$time_local"]      = "date_time_ncsa";
    this->NGINX_ALF["$time_iso8601"]    = "date_time_iso";
    this->NGINX_ALF["msec"]             = "date_time_epoch_s.ms";
    this->NGINX_ALF["$request"]         = "request_full";
    this->NGINX_ALF["$status"]          = "response_code";
    this->NGINX_ALF["$bytes_sent"]      = "bytes_sent";
    this->NGINX_ALF["$request_length"]  = "bytes_received";
    this->NGINX_ALF["$request_time"]    = "time_taken_s.ms";
    this->NGINX_ALF["$http_referer"]    = "referer";
    this->NGINX_ALF["$http_user_agent"] = "user_agent";
    // not in use, will be discarded
    this->NGINX_ALF["$remote_user"]         = "NONE";
    this->NGINX_ALF["$gzip_ratio"]          = "NONE";
    this->NGINX_ALF["$connection"]          = "NONE";
    this->NGINX_ALF["$connection_requests"] = "NONE";
    this->NGINX_ALF["pipe"]                 = "NONE";

    // error logs fields formats (only the ones considered)
    this->NGINX_ELF["$time_iso8601"]  = "date_time_iso";
    this->NGINX_ELF["$error_level"]   = "error_level";
    this->NGINX_ELF["$error_message"] = "error_message";
    this->NGINX_ELF["$pid"]           = "NONE";
    this->NGINX_ELF["$cid"]           = "NONE";

    /////////////
    //// IIS ////
    // access logs fields formats (only the ones considered)
    this->IIS_ALF["c-ip"]           = "client";
    this->IIS_ALF["date"]           = "date_time_utc_d";
    this->IIS_ALF["time"]           = "date_time_utc_t";
    this->IIS_ALF["cs-version"]     = "request_protocol";
    this->IIS_ALF["cs-method"]      = "request_method";
    this->IIS_ALF["cs-uri-stem"]    = "request_uri";
    this->IIS_ALF["cs-uri-query"]   = "request_query";
    this->IIS_ALF["sc-status"]      = "response_code";
    this->IIS_ALF["sc-bytes"]       = "bytes_sent";
    this->IIS_ALF["cs-bytes"]       = "bytes_received";
    this->IIS_ALF["time-taken"]     = "time_taken_ms";
    this->IIS_ALF["cs(Referrer)"]   = "referer";
    this->IIS_ALF["cs(Cookie)"]     = "cookie";
    this->IIS_ALF["cs(User-Agent)"] = "user_agent";
    // not in use, will be discarded
    this->IIS_ALF["s-sitename"]      = "NONE";
    this->IIS_ALF["s-computername"]  = "NONE";
    this->IIS_ALF["s-ip"]            = "NONE";
    this->IIS_ALF["s-port"]          = "NONE";
    this->IIS_ALF["cs-username"]     = "NONE";
    this->IIS_ALF["cs-host"]         = "NONE";
    this->IIS_ALF["sc-substatus"]    = "NONE";
    this->IIS_ALF["sc-win32-status"] = "NONE";
    this->IIS_ALF["streamid"]        = "NONE";
    // error logs fields formats (only the ones considered)
    this->IIS_ELF[""] = "";
    // not in use, will be discarded
    this->IIS_ELF[""] = "NONE";*/
}



const FormatOps::LogsFormat FormatOps::processApacheFormatString( const std::string& f_str, const int l_type )
{
    const std::unordered_map<std::string, std::string> *f_map;
    const std::unordered_map<std::string ,std::unordered_map<std::string, std::string>> *f_map_v;
    switch ( l_type ) {
        case 1:
            f_map   = &this->APACHE_ALF;
            f_map_v = &this->APACHE_ALF_V;
            break;
        case 2:
            f_map   = &this->APACHE_ELF;
            f_map_v = &this->APACHE_ELF_V;
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogType for Apache: "[l_type]);
    }

    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    int n_fld=0,
        start, stop=0, aux, aux_start, aux_stop,
        max=f_str.size()-1;
    std::string aux_fld, aux_fld_v, cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        cur_sep = "";
        while (true) {
            // loop until a valid field is found (doens't matter if considered or not)
            while (true) {
                // hunt the next field
                aux = f_str.find_first_of( '%', stop );
                // check if false positive
                if ( aux > 0 && aux <= max ) {
                    if ( f_str.at(aux-1) == '%' || f_str.at(aux-1) == '\\' ) {
                        // the percent sign character
                        stop = aux + 1;
                        continue;
                    }
                }
                break;
            }

            if ( aux < 0 || aux > max ) {
                // no more fields, append the last section as final separator
                final = f_str.substr( start );
                n_fld = -1;
                break;
            }

            // append the current separator
            cur_sep += f_str.substr( start, aux-start );
            stop = aux;

            // define if normal or composed
            if ( f_str.at( aux+1 ) == '{' ) {
                // composed
                aux_start = aux + 2;
                aux = f_str.find_first_of( '}', aux_start );
                aux_stop = aux + 2;
                // get the varname
                aux_fld = f_str.substr(
                    aux_start,
                    aux - aux_start );
                // get the module
                aux_fld_v = f_str.at( aux+1 );
                try {
                    // try if the module is valid
                    cur_fld = f_map_v->at( aux_fld_v ).at( aux_fld );
                    // if here, is valid. stop hunting and append
                    stop = aux_stop;
                    break;

                } catch (...) {
                    // invalid, append all as separator and restart hunting
                    cur_sep += f_str.substr( stop, aux_stop-stop );
                    stop = aux_stop;
                    continue;
                }

            } else {
                // normal
                try {
                    // try if the module is valid
                    aux_fld = f_str.substr( aux, 3 );
                    aux_stop = aux+3;
                    if ( aux_fld != "%>s" ) {
                        aux_fld = f_str.substr( aux, 2 );
                        aux_stop = aux+2;
                    }
                    cur_fld = f_map->at( aux_fld );
                    // if here, is valid. stop hunting and append
                    stop = aux_stop;
                    break;

                } catch (...) {
                    // invalid, append all as separator and restart hunting
                    stop ++;
                    cur_sep += f_str.at( stop );
                    continue;
                }

            }
        }
        // outside hunting loop

        if ( n_fld < 0 ) {
            // final reached, stop looping
            break;

        } else if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = cur_sep;

        } else {
            // append to separators list
            separators.push_back( cur_sep );
        }

        // append the field
        if ( cur_fld == "client:port" ) {
            fields.push_back( "client" );
            separators.push_back( ":" );
            fields.push_back( "port" );
        } else {
            fields.push_back( cur_fld );
        }
        n_fld++;

    }

    return FormatOps::LogsFormat{
            .string  = f_str,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };

}



const FormatOps::LogsFormat FormatOps::processNginxFormatString( const std::string& f_str, const int l_type )
{
    const std::unordered_map<std::string, std::string> *f_map;
    const std::vector<std::string> *f_flds;

    switch ( l_type ) {
        case 1:
            f_map  = &this->NGINX_ALF;
            f_flds = &this->N_ALFs;
            break;
        case 2:
            f_map  = &this->NGINX_ELF;
            f_flds = &this->N_ELFs;
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogType for Nginx: "[l_type]);
    }

    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    int n_fld=0,
        start, aux=0, stop=0,
        min_dist=0, max_dist=f_str.size()-1;
    std::string cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        cur_fld = "";
        min_dist = max_dist;
        // find the next field
        for ( const std::string& fld : *f_flds ) {
            // run untill a valid field is found
            aux = f_str.find( fld, start );
            if ( aux < 0 | aux > min_dist ) {
                // not found, skip to the next
                continue;
            }
            if ( aux > 0 ) {
                if ( f_str.at(aux-1) == '\\' ) {
                    // backslashed, may be a false positive
                    int n_slashes = 1, j=aux-2;
                    while (true) {
                        if ( j < 0 ) {
                            break;
                        }
                        if ( f_str.at( j ) == '\\' ) {
                            n_slashes ++;
                            j --;
                        } else {
                            break;
                        }
                    }
                    if ( n_slashes % 2 == 1 ) {
                        // odd number of BS, supposed to result in a backslashed keyword
                        continue;
                    }
                }
            }
            if ( aux < min_dist ) {
                // a nearer field has been found
                min_dist = aux;
                cur_fld  = fld;
            }
            if ( min_dist == start ) {
                // starter position, closest field found
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
            initial = cur_sep;
        } else {
            // append to separators list
            separators.push_back( cur_sep );
        }

        n_fld++;
        // append the current field, converted
        if ( f_map->at( cur_fld ) == "client:port" ) {
            fields.push_back( "client" );
            separators.push_back( ":" );
            fields.push_back( "port" );
        } else {
            fields.push_back( f_map->at( cur_fld ) );
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



const FormatOps::LogsFormat FormatOps::processIisFormatString( const std::string& f_str, const int l_mod )
{
    int n_fld=0,
        start, aux=0, stop=0,
        min_dist=0, max_dist=f_str.size()-1;
    std::string cur_fld, cur_sep;
    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    const std::unordered_map<std::string, std::string> &f_map = this->IIS_ALF;
    const std::vector<std::string> &f_flds = this->I_ALFs;
    switch ( l_mod ) {
        case 1:
            // IIS logging module
            final = ",";
            separators = {", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", "};
            fields = {"client","NONE","date_time_MMDDYY","date_time_clock_24","NONE","NONE","NONE","time_taken_ms","bytes_received","bytes_sent","response_code","NONE","request_method","request_uri","request_query"};
            break;
        case 2:
            // NCSA logging module
            separators = {" "," "," [","] "," \"","\" "," "};
            fields = {"client","NONE","NONE","date_time_ncsa","request_full","response_code","bytes_sent"};
            break;
        case 3:
            // W3C logging module
            // parse the string to convert keyargs in craplog's fields format
            // find and convert any field
            while (true) {
                // start after the last found field
                start = stop;
                cur_fld = "";
                min_dist = max_dist;
                // find the next field
                for ( const std::string& fld : f_flds ) {
                    // run untill a valid field is found
                    aux = f_str.find( fld, start );
                    if ( aux < 0 | aux > min_dist ) {
                        // not found, skip to the next
                        continue;
                    }
                    if ( aux > 0 ) {
                        if ( f_str.at(aux-1) == '\\' ) {
                            // backslashed, may be a false positive
                            int n_slashes = 1, j=aux-2;
                            while (true) {
                                if ( j < 0 ) {
                                    break;
                                }
                                if ( f_str.at( j ) == '\\' ) {
                                    n_slashes ++;
                                    j --;
                                } else {
                                    break;
                                }
                            }
                            if ( n_slashes % 2 == 1 ) {
                                // odd number of BS, supposed to result in a backslashed keyword
                                continue;
                            }
                        }
                    }
                    if ( aux < min_dist ) {
                        // a nearer field has been found
                        min_dist = aux;
                        cur_fld  = fld;
                    }
                    if ( min_dist == start ) {
                        // starter position, closest field found
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
                    initial = cur_sep;
                } else {
                    // append to separators list
                    separators.push_back( cur_sep );
                }

                n_fld++;
                // append the current field, converted
                if ( f_map.at( cur_fld ) == "client:port" ) {
                    fields.push_back( "client" );
                    separators.push_back( ":" );
                    fields.push_back( "port" );
                } else {
                    fields.push_back( f_map.at( cur_fld ) );
                }
                // step at the end of the current field for the next start
                stop = min_dist + cur_fld.size();
            }
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogModule for IIS: "[l_mod]);
    }


    return FormatOps::LogsFormat{
            .string  = f_str,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };
}

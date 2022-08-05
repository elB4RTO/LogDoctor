
#include "formats.h"

#include "modules/dialogs.h"


FormatOps::FormatOps()
{

}

// process escapes like apache
const std::string FormatOps::parseApacheEscapes( const std::string& string )
{
    int i = 0,
        max = string.size()-1;
    std::string str1 = "",
                str2 = "";
    char c, cc;
    // parse the first time, no control-character added
    while (true) {
        if ( i >= max ) {
            // no need to check the final char
            if ( i == max ) {
                str1.push_back( string.at( i ) );
            }
            break;
        }
        c = string.at( i );
        cc = string.at( i+1 );
        if ( c == '\\' && cc == '\\' ) {
            str1.push_back( c );
            i++;
        } else if ( c == '%' && cc == '%' ) {
            str1.push_back( c );
            i++;
        } else {
            str1.push_back( c );
        }
        i++;
    }
    i = 0;
    max = str1.size()-1;
    // parse the second time, adding control-characters
    while (true) {
        if ( i >= max ) {
            // no need to check the final char
            if ( i == max ) {
                str2.push_back( str1.at( i ) );
            }
            break;
        }
        c = str1.at( i );
        cc = str1.at( i+1 );
        if ( c == '\\' ) {
            // just the ones supported by apache
            if ( cc == '\\' ) {
                str2.push_back( c );
                i++;
            } else if ( cc == 'n' ) {
                str2.push_back( '\n' );
                i++;
            } else if ( cc == 'r' ) {
                str2.push_back( '\r' );
                i++;
            } else if ( cc == 't' ) {
                str2.push_back( '\t' );
                i++;
            } else {
                if ( cc != '"' ) {
                    // any other backslashed characters result in a backslash+character, except for double-quotes
                    str2.push_back( c );
                }
                str2.push_back( cc );
                i++;
            }
        } else {
            str2.push_back( c );
        }
        i++;
    }

    return str2;
}

// process escapes like nginx
const std::string FormatOps::parseNginxEscapes( const std::string& string )
{
    int i = 0,
        max = string.size()-1;
    char c, cc;
    std::string str = "";
    // parse once
    while (true) {
        if ( i >= max ) {
            // no need to check the final char
            if ( i == max ) {
                str.push_back( string.at( i ) );
            }
            break;
        }
        c = string.at( i );
        cc = string.at( i+1 );
        if ( c == '\\' ) {
            // just the ones supported by nginx
            if ( cc == '\\' ) {
                str.push_back( c );
                i++;
            } else if ( cc == 'n' ) {
                str.push_back( '\n' );
                i++;
            } else if ( cc == 'r' ) {
                str.push_back( '\r' );
                i++;
            } else if ( cc == 't' ) {
                str.push_back( '\t' );
                i++;
            } else if ( cc == '\'' || cc == '"' ) {
                str.push_back( cc );
                i++;
            } else {
                // not a control-character, resulting in a backslash+character
                str.push_back( c );
                str.push_back( cc );
                i++;
            }
        } else {
            str.push_back( c );
        }
        i++;
    }

    return str;
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
    std::string aux_fld, aux_fld_v, cur_fld, cur_sep="";
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        cur_sep = "";
        if ( cur_fld == "date_time_ncsa" ) {
            // NCAS time format is always enclosed inside brackets
            cur_sep += "]";
        }
        while (true) {
            // loop until a valid field is found (doens't matter if considered or not)
            while (true) {
                // hunt the next field
                aux = f_str.find_first_of( '%', stop );
                // check if false positive
                if ( aux >= 0 && aux <= max ) {
                    // apache only escapes a format field using the double percent sign
                    // backslashes are valid for control-characters only, or get reduced to 1 escape only
                    if ( f_str.at(aux+1) == '%' ) {
                        // the percent sign character
                        stop = aux + 2;
                        continue;
                    }
                }
                break;
            }

            if ( aux < 0 || aux > max ) {
                // no more fields, append the last section as final separator
                cur_sep += f_str.substr( start );
                /*final = cur_sep + f_str.substr( start );
                final = this->parseApacheEscapes( final );*/
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
                if ( aux_fld_v == "^" ) {
                    aux_stop += 2;
                    aux_fld_v = f_str.substr( aux+1, 3 );
                }
                if ( f_map_v->find( aux_fld_v ) == f_map_v->end() ) {
                    // invalid, append all as separator and restart hunting
                    cur_sep += f_str.substr( stop, aux_stop-stop );
                    start = stop = aux_stop;
                    continue;
                } else {
                    // module is valud
                    const auto &aux_map = f_map_v->at( aux_fld_v );
                    bool skip;
                    int aux_aux_start,
                        aux_aux_stop = 0,
                        aux_aux;
                    std::string aux_aux_fld;
                    while (true) {
                        // loop inside the composed field
                        skip = true;
                        aux_aux_start = aux_aux_stop;
                        for ( const std::string& fld : this->A_ALFs_v.at( aux_fld_v ) ) {
                            aux_aux = aux_fld.find( fld, aux_aux_start );
                            // check if has been found
                            if ( aux_aux >= 0 && aux_aux <= aux_fld.size() ) {
                                // check if false positive
                                if ( aux_fld.at(aux_aux) == '%' ) {
                                    if ( aux_fld.at(aux_aux-1) == '%' || aux_fld.at(aux_aux-1) == '\\' ) {
                                        // the percent sign character
                                        aux_aux_start = aux_aux + 1;
                                        continue;
                                    } else if ( f_str.at(aux+1) == '%' ) {
                                        // the percent sign character
                                        aux_aux_start = aux_aux + 2;
                                        continue;
                                    }
                                }
                                skip = false;
                                aux_aux_stop = aux_aux + fld.size();
                                break;
                            } else {
                                continue;
                            }
                        }
                        if ( skip == true ) {
                            break;
                        }
                        // append the current separator
                        cur_sep += aux_fld.substr( aux_aux_start, aux_aux-aux_aux_start );
                        aux_aux_fld = aux_fld.substr( aux_aux, aux_aux_stop-aux_aux );
                        // check if the module is valid
                        if ( aux_map.size() == 0 ) {
                            // not considered
                            fields.push_back( "NONE" );
                            // append to separators list
                            separators.push_back( cur_sep );
                            cur_sep = "";

                        } else if ( aux_map.find( aux_aux_fld ) != aux_map.end() ) {
                            // valid, append
                            cur_fld = aux_map.at( aux_aux_fld );
                            fields.push_back( cur_fld );
                            // append to separators list
                            separators.push_back( cur_sep );
                            cur_sep = "";

                        } else {
                            // invalid, append as separator and keep hunting
                            cur_sep += aux_fld.substr( aux_aux, aux_aux_stop-aux_aux );
                        }
                    }
                    // items already appended as needed, next main hunting loop round
                    start = stop = aux_stop;
                    continue;
                }

            } else {
                // normal
                aux_fld = f_str.substr( aux, 2 );
                aux_stop = aux+2;
                if ( aux_fld == "%>" ) {
                    aux_fld = f_str.substr( aux, 3 );
                    aux_stop = aux+3;
                }
                // check if the module is valid
                if ( f_map->find( aux_fld ) != f_map->end() ) {
                    // valid
                    cur_fld = f_map->at( aux_fld );
                    if ( cur_fld == "date_time_ncsa" ) {
                        // NCAS time format is always enclosed inside brackets
                        cur_sep += "[";
                    }
                    stop = aux_stop;
                    break;
                } else {
                    // invalid, append all as separator and restart hunting
                    cur_sep += aux_fld;
                    start = stop = aux_stop;
                    continue;
                }
            }
        }
        // outside hunting loop

        if ( n_fld < 0 ) {
            // final reached, stop looping
            final = this->parseApacheEscapes( cur_sep );
            break;

        } else if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = this->parseApacheEscapes( cur_sep );

        } else {
            // append to separators list
            separators.push_back( this->parseApacheEscapes( cur_sep ) );
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
    //delete f_map;
    //delete f_map_v;

    return FormatOps::LogsFormat{
            .string  = f_str,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };

}
// sample
const QString FormatOps::getApacheLogSample( const LogsFormat& log_format, const int log_type )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>* map;
    switch ( log_type ) {
        case 1:
            map  = &this->APACHE_ALF_SAMPLES;
            break;
        case 2:
            map  = &this->APACHE_ELF_SAMPLES;
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogType for Apache: "[log_type]);
    }
    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( int i=0; i<log_format.separators.size(); i++ ) {
        // append fields and separators
        sample += map->at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0 ) {
        sample += map->at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
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
        start, aux, stop=0,
        min_dist, max_dist=f_str.size()-1;
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
            if ( aux < 0 || aux > min_dist ) {
                // not found, skip to the next
                continue;
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
            final = this->parseNginxEscapes( f_str.substr( start ) );
            break;
        }

        // get the current separator
        cur_sep = this->parseNginxEscapes( f_str.substr( start, min_dist-start ) );
        if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = cur_sep;
        } else {
            // append to separators list
            separators.push_back( cur_sep );
        }

        n_fld++;
        // append the current field, converted
        /*if ( f_map->at( cur_fld ) == "client:port" ) {
            fields.push_back( "client" );
            separators.push_back( ":" );
            fields.push_back( "port" );
        } else {*/
            fields.push_back( f_map->at( cur_fld ) );
        /*}*/
        // step at the end of the current field for the next start
        stop = min_dist + cur_fld.size();
    }
    //delete f_map;
    //delete f_flds;

    return FormatOps::LogsFormat{
            .string  = f_str,
            .initial = initial,
            .final   = final,
            .separators = separators,
            .fields     = fields
        };
}
// sample
const QString FormatOps::getNginxLogSample( const LogsFormat& log_format, const int log_type )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>* map;
    switch ( log_type ) {
        case 1:
            map  = &this->NGINX_ALF_SAMPLES;
            break;
        case 2:
            map  = &this->NGINX_ELF_SAMPLES;
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogType for Apache: "[log_type]);
    }
    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( int i=0; i<log_format.separators.size(); i++ ) {
        // append fields and separators
        sample += map->at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0 ) {
        sample += map->at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
}



const FormatOps::LogsFormat FormatOps::processIisFormatString( const std::string& f_str, const int l_mod )
{
    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    switch ( l_mod ) {
        case 2:
            // IIS logging module
            final = ",";
            separators = {", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", "};
            fields = {"client","NONE","date_time_MDYY","date_time_utc_t","NONE","NONE","NONE","time_taken_ms","bytes_received","bytes_sent","response_code","NONE","request_method","request_uri","request_query"};
            break;
        case 1:
            // NCSA logging module
            separators = {" "," "," [","] \"","\" "," "};
            fields = {"client","NONE","NONE","date_time_ncsa","request_full","response_code","bytes_sent"};
            break;
        case 0:
            // W3C logging module
            if ( f_str.size() > 0 ) {
                bool finished = false;
                int start, stop=0,
                    max=f_str.size()-1;
                std::string aux_fld, cur_sep;
                const std::unordered_map<std::string, std::string> &f_map = this->IIS_ALF;
                const std::vector<std::string> &f_flds = this->I_ALFs;
                // parse the string to convert keyargs in craplog's fields format
                while (true) {
                    // start after the last found separator
                    start = stop;
                    cur_sep = " ";
                    // find the next separator, which is always a single whitespace, in this case
                    stop = f_str.find( " ", start );
                    if ( stop < 0 || stop > max ) {
                        // not found, this is the last field
                        stop = max+1;
                        finished = true;
                    }

                    // set the current field
                    aux_fld = f_str.substr( start, stop-start );
                    // step over the separator
                    stop++;

                    // check if the module is valid
                    if ( f_map.find( aux_fld ) != f_map.end() ) {
                        // valid, append
                        fields.push_back( f_map.at( aux_fld ) );
                        if ( finished == false ) {
                            separators.push_back( cur_sep );
                        } else {
                            // this was the last field
                            break;
                        }
                    } else {
                        // shouldn't be here, but...
                        // invalid, append all as separator and restart searching
                        if ( finished == false ) {
                            // not the last one, append to the last separator
                            separators.at( separators.size()-1 ) += aux_fld + " ";
                        } else {
                            // no more separators, set the last one as final
                            final = separators.at( separators.size()-1 ) + aux_fld;
                            separators.pop_back();
                            break;
                        }
                    }
                }
            }
            // outside search loop, killing the switch
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
// sample
const QString FormatOps::getIisLogSample( const LogsFormat& log_format/*, const int log_type*/ )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>& map = this->IIS_ALF_SAMPLES;
    /*switch ( log_type ) {
        case 1:
            map  = &this->IIS_ALF_SAMPLES;
            break;
        case 2:
            map  = &this->IIS_ELF_SAMPLES;
            break;
        default:
            // shouldn't be here
            throw (&"Unexpected LogType for Apache: "[log_type]);
    }*/
    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( int i=0; i<log_format.separators.size(); i++ ) {
        // append fields and separators
        sample += map.at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0 ) {
        sample += map.at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
}

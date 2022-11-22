
#include "formats.h"

#include "modules/exceptions.h"
#include "utilities/strings.h"


FormatOps::FormatOps()
{

}


// count the new lines
const int FormatOps::countNewLines( const std::string& initial, const std::string& final, const std::vector<std::string>& separators )
{
    int nl = 0;
    nl += StringOps::count( initial, "\n" );
    nl += StringOps::count( final, "\n" );
    for ( const std::string& sep : separators ) {
        nl += StringOps::count( sep, "\n" );
    }
    return nl;
}


// process escapes like apache
const std::string FormatOps::parseApacheEscapes( const std::string& string , const bool& strftime )
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
        if ( c == '\\' && (cc == '\\' || cc == '"') ) {
            str1.push_back( cc );
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
            // just the ones supported
            if ( cc == '\\' || cc == '"' ) {
                str2.push_back( cc );
                i++;
            } else {
                if ( strftime ) {
                    // when parsing for strftime, any other backslashed characters result in a backslash+character
                    str2.push_back( c );
                    str2.push_back( cc );
                    i++;

                } else {
                    if ( cc == 'n' ) {
                        str2.push_back( '\n' );
                        i++;
                    } else if ( cc == 'r' ) {
                        // not supported
                        throw LogFormatException( "LogDoctor doesn't support the usage of the Carriage Return: '\\r'." );
                    } else if ( cc == 't' ) {
                        str2.push_back( '\t' );
                        i++;
                    } else {
                        // any other backslashed characters result in a backslash+character
                        str2.push_back( c );
                        str2.push_back( cc );
                        i++;
                    }
                }
            }
        } else if ( c == '%' && strftime ) {
            // strftime control-characters
            if ( cc == 'n' ) {
                str2.push_back( '\n' );
                i++;
            } else if ( cc == 't' ) {
                str2.push_back( '\t' );
                i++;
            } else {
                // any other characters result in a percent+character
                str2.push_back( c );
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
            if ( cc == '\\' || cc == '\'' || cc == '"' ) {
                str.push_back( cc );
                i++;
            } else if ( cc == 'n' ) {
                str.push_back( '\n' );
                i++;
            } else if ( cc == 'r' ) {
                // not supported
                throw LogFormatException( "LogDoctor doesn't support the usage of the Carriage Return: '\\r'." );
            } else if ( cc == 't' ) {
                str.push_back( '\t' );
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

// find where the field ends
const size_t FormatOps::findNginxFieldEnd( const std::string& string, const int& start )
{
    size_t stop=start;
    const int max=string.size()-1;
    if ( start < max ) { // if start equals max there's no need to loop
        for ( int i=start; i<=max; i++ ) {
            const char& c = string.at( i );
            if ( StringOps::isAlnum( c ) || c == '_' ) {
                stop = i;
            } else {
                break;
            }
        }
    }
    return stop;
}

// check the given format string for unwanted characters
void FormatOps::checkIisString( const std::string& string )
{
    for ( const char& chr : string ) {
        if ( !(StringOps::isAlnum( chr ) || chr == ' ' || chr == '-' || chr == ',' || chr == ':' || chr == '(' || chr == ')' || chr == '[' || chr == ']') ) {
            // unwanted character
            const std::string c( 1, chr );
            throw LogFormatException( "Unexpected character found: "+c );
        }
    }
}



const FormatOps::LogsFormat FormatOps::processApacheFormatString( const std::string& f_str )
{
    const auto &f_map   = this->APACHE_ALF;
    const auto &f_map_v = this->APACHE_ALF_V;

    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    int n_fld=0;
    size_t start, stop=0, aux, aux_start, aux_stop;
    const size_t max=f_str.size()-1;
    std::string aux_fld, aux_fld_v, cur_fld, cur_sep;
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
                aux = f_str.find( '%', stop );
                // check if false positive
                if ( aux == max ) {
                    // invalid, can't end with a single '%'
                    throw LogFormatException( "Invalid format string: ending with a single '%'." );

                } else if ( aux != std::string::npos ) {
                    // apache only escapes a format field using the double percent sign
                    // backslashes are valid for control-characters only, or get reduced
                    // single percent-signs are considered invalid
                    const char c = f_str.at(aux+1);
                    if ( c == ',' || c == '{' || c == '<' || c == '>' ) {
                        // in the first case: status code(s) may follow, or may not
                        // in the second case: a composed format code may follow
                        // in the third and fourth cases: expressing a status code field
                        // in any case is considered valid
                        ;
                    } else if ( c == '%' ) {
                        // the percent sign character, will be used as separator, skip
                        stop = aux + 2;
                        continue;
                    } else if ( ! StringOps::isAlnum( c ) ) {
                        // invalid, there must be a field code, a status code or a percent sign after a '%'
                        const std::string chr( 1, c );
                        throw LogFormatException( "Invalid format: there must be a valid format code, a status code or a percent sign character after a '%'.\nFound: '%"+chr+"'." );
                    }
                }
                break;
            }

            if ( aux == std::string::npos ) {
                // no more fields, append the last section as final separator
                cur_sep += f_str.substr( start );
                n_fld = -1;
                break;
            }

            // append the current separator
            cur_sep += f_str.substr( start, aux-start );
            aux ++;

            char c = f_str.at( aux );
            // remove the per-status directives (if any)
            if ( StringOps::isNumeric( c )
              || c == ',' ) {
                // per-status, not important for LogDoctor
                size_t aux_aux = aux+1;
                while (true) {
                    if ( aux_aux > max ) {
                        break;
                    }
                    c = f_str.at( aux_aux );
                    if ( StringOps::isNumeric( c )
                      || c == ',' ) {
                        // skip these chars
                        aux_aux ++;
                        continue;
                    } else {
                        // stop
                        aux = aux_aux;
                        break;
                    }
                }
            }

            c = f_str.at( aux );
            // define if normal or composed
            if ( c == '{' ) {
                // composed
                aux_start = aux + 1;
                aux = f_str.find( '}', aux_start );
                if ( aux == std::string::npos ) {
                    // closer bracket not found, resulting in an invalid field
                    throw LogFormatException( "Invalid format code, no closing bracket found: '%{'." );
                }
                aux_stop = aux + 2;
                // get the varname(s)
                aux_fld = f_str.substr(
                    aux_start,
                    aux - aux_start );
                // get the module
                aux_fld_v = f_str.at( aux+1 );
                if ( aux_fld_v == "^" ) {
                    aux_stop += 2;
                    aux_fld_v = f_str.substr( aux+1, 3 );
                }
                if ( f_map_v.find( aux_fld_v ) == f_map_v.end() ) {
                    // invalid module, abort
                    throw LogFormatException( "Invalid format code found: '%{...}"+aux_fld_v+"'." );
                } else {
                    // module is valud
                    const auto &aux_map = f_map_v.at( aux_fld_v );
                    if ( aux_map.size() == 0 ) {
                        // module not considered and always giving out something, even if invalid varname is passed
                        fields.push_back( "NONE" );
                        separators.push_back( this->parseApacheEscapes( cur_sep ) );
                        cur_sep = "";

                    } else if ( aux_fld_v == "p" || aux_fld_v == "P" || aux_fld_v == "T" ) {
                        // still not considered (except 'T'), but invalid fields get used as text
                        // field concatenation not allowed, whole content used as varname
                        if ( aux_map.find( aux_fld ) != aux_map.end() ) {
                            // valid varname
                            fields.push_back( aux_map.at( aux_fld ) );
                        } else {
                            // invalid varname, use as text
                            fields.push_back( "NONE" );
                            cur_sep += aux_fld;
                        }
                        separators.push_back( this->parseApacheEscapes( cur_sep ) );
                        cur_sep = "";

                    } else if ( aux_fld_v == "a" || aux_fld_v == "h" ) {
                        // client, in any case
                        fields.push_back( "client" );
                        separators.push_back( this->parseApacheEscapes( cur_sep ) );
                        cur_sep = "";

                    } else if ( aux_fld_v == "i" ) {
                        // always giving a result, may the varname be valid or not ('-' if invalid)
                        // field concatenation not allowed, the entire content is used as varname
                        if ( aux_map.find( aux_fld ) != aux_map.end() ) {
                            fields.push_back( aux_map.at( aux_fld ) );
                        } else {
                            fields.push_back( "NONE" );
                        }
                        separators.push_back( this->parseApacheEscapes( cur_sep ) );
                        cur_sep = "";

                    } else /*if ( aux_fld_v == "t" )*/ {
                        // only 't' remaining
                        size_t aux_aux = aux_fld.find( '%' );
                        if ( aux_aux == std::string::npos ) {
                            // no concatenation, only valid fields used, anything else used as text
                            // whole content used as varname
                            if ( aux_map.find( aux_fld ) != aux_map.end() ) {
                                // valid
                                fields.push_back( aux_map.at( aux_fld ) );
                                separators.push_back( this->parseApacheEscapes( cur_sep, true ) );
                                cur_sep = "";
                            } else {
                                // invalid, append to current separator and restart hunting
                                cur_sep += aux_fld;
                            }

                        } else {
                            // concatenation allowed, only strftime() value used as fields, everything else treated as text
                            size_t aux_aux_start,
                                   aux_aux_stop = 0;
                            std::string aux_aux_fld;
                            while (true) {
                                // loop inside the composed field
                                aux_aux_start = aux_aux_stop;
                                while (true) {
                                    // hunt the next field
                                    aux_aux = aux_fld.find( '%', aux_aux_stop );
                                    // check if false positive
                                    if ( aux_aux != std::string::npos ) {
                                        // same escape rules as before, but single percent-signs are considered valid and treated as text
                                        const char c_ = aux_fld.at( aux_aux+1 );
                                        if ( c_ == '%' || c_ == 'n' || c_ == 't' ) {
                                            // control characters, will be used as separator, skip
                                            aux_aux_stop = aux_aux + 2;
                                            continue;
                                        }
                                    }
                                    break;
                                }

                                if ( aux_aux == std::string::npos ) {
                                    // no more fields, append the last section as separator
                                    cur_sep += aux_fld.substr( aux_aux_start );
                                    break;
                                }

                                // append the current separator
                                cur_sep += aux_fld.substr( aux_aux_start, aux_aux-aux_aux_start );
                                // and get the possible field
                                aux_aux_fld = aux_fld.substr( aux_aux, 2 );
                                aux_aux_stop = aux_aux+2;
                                // check if the field is valid
                                if ( aux_map.find( aux_aux_fld ) != aux_map.end() ) {
                                    // valid, append
                                    cur_fld = aux_map.at( aux_aux_fld );
                                    fields.push_back( cur_fld );
                                    // append to separators list
                                    separators.push_back( this->parseApacheEscapes( cur_sep, true ) );
                                    cur_sep = "";

                                } else {
                                    // invalid, append as separator and keep hunting
                                    cur_sep += aux_aux_fld;
                                }
                            }
                        }
                    }
                    // items already appended as needed, next main hunting loop round
                    start = stop = aux_stop; // re-starting after the previously found module
                    continue;
                }

            } else {
                // normal
                aux_fld = c;
                aux_stop = aux+1;
                if ( aux_fld == ">" || aux_fld == "<" ) {
                    aux_fld += f_str.at( aux+1 );
                    aux_stop ++;
                }
                // check if the module is valid
                if ( f_map.find( aux_fld ) != f_map.end() ) {
                    // valid
                    cur_fld = f_map.at( aux_fld );
                    if ( cur_fld == "date_time_ncsa" ) {
                        // apache's NCSA time format is always enclosed inside brackets
                        cur_sep += "[";
                    }
                    stop = aux_stop;
                    break;
                } else {
                    // invalid format field, abort
                    throw LogFormatException( "Invalid format code found: '%"+aux_fld+"'." );
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
        fields.push_back( cur_fld );
        n_fld++;
    }

    return FormatOps::LogsFormat{
            .string     = f_str,
            .initial    = initial,
            .final      = final,
            .separators = separators,
            .fields     = fields,
            .new_lines  = this->countNewLines( initial, final, separators )
        };

}
// sample
const QString FormatOps::getApacheLogSample( const LogsFormat& log_format )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>& map = this->APACHE_ALF_SAMPLES;

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



const FormatOps::LogsFormat FormatOps::processNginxFormatString( const std::string& f_str )
{
    const auto& f_map  = this->NGINX_ALF;

    std::string initial="", final="";
    std::vector<std::string> separators, fields;
    // parse the string to convert keyargs in craplog's fields format
    bool finished = false;
    size_t start, aux, stop=0;
    const size_t max=f_str.size()-1;
    std::string cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        // find the next field
        aux = f_str.find( '$', start );
        if ( aux == std::string::npos ) {
            // not found, append as final and stop searching
            final = this->parseNginxEscapes( f_str.substr( start ) );
            break;
        }
        aux ++;
        // find the end of the current field
        stop = this->findNginxFieldEnd( f_str, aux ) + 1;
        if ( stop == max ) {
            // this is the last field, and ther's no final separator
            finished = true;
        }

        cur_sep = f_str.substr( start, aux-start-1 );
        cur_fld = f_str.substr( aux, stop-aux );

        // fixes for varnames
        if ( StringOps::startsWith( cur_fld, "cookie_" ) ) {
            cur_fld = "cookie_";
        } else if ( StringOps::startsWith( cur_fld, "http_" )
                 && cur_fld != "http_user_agent" && cur_fld != "http_referer" ) {
            cur_fld = "http_";
        } else if ( StringOps::startsWith( cur_fld, "arg_" ) ) {
            cur_fld = "arg_";
        } else if ( StringOps::startsWith( cur_fld, "sent_http_" ) ) {
            cur_fld = "sent_http_";
        } else if ( StringOps::startsWith( cur_fld, "upstream_cookie_" ) ) {
            cur_fld = "upstream_cookie_";
        } else if ( StringOps::startsWith( cur_fld, "upstream_http_" ) ) {
            cur_fld = "upstream_http_";
        }

        // check if the field is valid
        if ( f_map.find( cur_fld ) != f_map.end() ) {
            // valid, append
            if ( start == 0 ) {
                initial = this->parseNginxEscapes( cur_sep );
            } else {
                separators.push_back( this->parseNginxEscapes( cur_sep ) );
            }
            fields.push_back( f_map.at( cur_fld ) );
            if ( finished ) {
                // this was the last field
                break;
            }
        } else {
            // invalid, abort
            throw LogFormatException( "Invalid format code found: '$"+cur_fld+"'." );
        }
    }

    return FormatOps::LogsFormat{
            .string     = f_str,
            .initial    = initial,
            .final      = final,
            .separators = separators,
            .fields     = fields,
            .new_lines  = this->countNewLines( initial, final, separators )
        };
}
// sample
const QString FormatOps::getNginxLogSample( const LogsFormat& log_format )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>& map = this->NGINX_ALF_SAMPLES;

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



const FormatOps::LogsFormat FormatOps::processIisFormatString( const std::string& f_str, const int& l_mod )
{
    this->checkIisString( f_str );
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
                size_t start, stop=0;
                const size_t max=f_str.size()-1;
                std::string cur_fld;
                const std::string cur_sep = " ";
                const auto &f_map = this->IIS_ALF;
                // parse the string to convert keyargs in craplog's fields format
                while (true) {
                    // start after the last found separator
                    start = stop;
                    // find the next separator, which is always a single whitespace, in this case
                    stop = f_str.find( cur_sep, start );
                    if ( stop == std::string::npos ) {
                        // not found, this is the last field
                        stop = max+1;
                        finished = true;
                    }

                    // set the current field
                    cur_fld = f_str.substr( start, stop-start );
                    // step over the separator
                    stop++;

                    // check if the module is valid
                    if ( f_map.find( cur_fld ) != f_map.end() ) {
                        // valid, append
                        fields.push_back( f_map.at( cur_fld ) );
                        if ( ! finished ) {
                            separators.push_back( cur_sep );
                        } else {
                            // this was the last field
                            break;
                        }
                    } else {
                        // invalid, abort
                        throw LogFormatException( "Invalid format code found: '"+cur_fld+"'." );
                    }
                }
            }
            // outside search loop, killing the switch
            break;

        default:
            // shouldn't be here
            throw LogFormatException( "Unexpected LogModule for IIS: "+std::to_string( l_mod ) );
    }

    return FormatOps::LogsFormat{
            .string     = f_str,
            .initial    = initial,
            .final      = final,
            .separators = separators,
            .fields     = fields,
            .new_lines  = 0
        };
}
// sample
const QString FormatOps::getIisLogSample( const LogsFormat& log_format )
{
    QString sample = "";
    const std::unordered_map<std::string, QString>& map = this->IIS_ALF_SAMPLES;

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

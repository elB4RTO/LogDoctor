
#include "formats.h"

#include "modules/exceptions.h"
#include "utilities/strings.h"

#include <vector>


namespace /*private*/
{

//! Conuts how many new lines are there in the format
/*!
    Used to join log lines which refer to the same log line
    \param initial The initial separator
    \param final The final separator
    \param separators The separators in the middle
    \return The number of new lines in a single log line
    \see LogsFormat, processApacheFormatString(), processNginxFormatString()
*/
size_t countNewLines( std::string_view initial, std::string_view final, const std::vector<std::string>& separators )
{
    size_t nl{ 0ul };
    nl += StringOps::count( initial, '\n' );
    nl += StringOps::count( final, '\n' );
    for ( const std::string& sep : separators ) {
        nl += StringOps::count( sep, '\n' );
    }
    return nl;
}


//! Parses the escapes (backslashes) and returns the resulting string
/*!
    Used to obtain the same result as on Apache2
    \param string The string to parse
    \param strftime Whether to apply the strftime special rule when parsing or not
    \return The resulting string
    \throw LogFormatException
    \see processApacheFormatString()
*/
std::string parseApacheEscapes( std::string_view string , const bool strftime=false )
{
    if ( string.empty() ) {
        return std::string{string};
    }
    size_t i{ 0ul },
           max{ string.size()-1ul };
    std::string str1;
    str1.reserve( string.size() );
    std::string str2;
    str2.reserve( string.size() );
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
        cc = string.at( i+1ul );
        if ( c == '\\' && (cc == '\\' || cc == '"') ) {
            str1.push_back( cc );
            ++i;
        } else if ( c == '%' && cc == '%' ) {
            str1.push_back( c );
            ++i;
        } else {
            str1.push_back( c );
        }
        ++i;
    }
    i = 0ul;
    max = str1.size()-1ul;
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
                ++i;
            } else {
                if ( strftime ) {
                    // when parsing for strftime, any other backslashed characters result in a backslash+character
                    str2.push_back( c );
                    str2.push_back( cc );
                    ++i;

                } else {
                    if ( cc == 'n' ) {
                        str2.push_back( '\n' );
                        ++i;
                    } else if ( cc == 'r' ) {
                        // not supported
                        throw LogFormatException( "LogDoctor doesn't support the usage of the Carriage Return: '\\r'" );
                    } else if ( cc == 't' ) {
                        str2.push_back( '\t' );
                        ++i;
                    } else {
                        // any other backslashed characters result in a backslash+character
                        str2.push_back( c );
                        str2.push_back( cc );
                        ++i;
                    }
                }
            }
        } else if ( c == '%' && strftime ) {
            // strftime control-characters
            if ( cc == 'n' ) {
                str2.push_back( '\n' );
                ++i;
            } else if ( cc == 't' ) {
                str2.push_back( '\t' );
                ++i;
            } else {
                // any other characters result in a percent+character
                str2.push_back( c );
                str2.push_back( cc );
                ++i;
            }

        } else {
            str2.push_back( c );
        }
        ++i;
    }

    str2.shrink_to_fit();
    return str2;
}


//! Parses the escapes (backslashes) and returns the resulting string
/*!
    Used to obtain the same result as on Nginx
    \param string The string to parse
    \return The resulting string
    \throw LogFormatException
    \see processNginxFormatString()
*/
std::string parseNginxEscapes( std::string_view string )
{
    if ( string.empty() ) {
        return std::string{string};
    }
    size_t i{ 0ul };
    const size_t max{ string.size()-1ul };
    char c, cc;
    std::string str;
    str.reserve( string.size() );
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
        cc = string.at( i+1ul );
        if ( c == '\\' ) {
            // just the ones supported by nginx
            if ( cc == '\\' || cc == '\'' || cc == '"' ) {
                str.push_back( cc );
                ++i;
            } else if ( cc == 'n' ) {
                str.push_back( '\n' );
                ++i;
            } else if ( cc == 'r' ) {
                // not supported
                throw LogFormatException( "LogDoctor doesn't support the usage of the Carriage Return: '\\r'" );
            } else if ( cc == 't' ) {
                str.push_back( '\t' );
                ++i;
            } else {
                // not a control-character, resulting in a backslash+character
                str.push_back( c );
                str.push_back( cc );
                ++i;
            }
        } else {
            str.push_back( c );
        }
        ++i;
    }

    str.shrink_to_fit();
    return str;
}


//! Checks whether the given character is valid or not
/*!
    \param chr The target character
    \return The result of the check
    \see findNginxFieldEnd
*/
bool checkNginxFieldChar( const char& chr )
{
    return CharOps::isAlnum( chr ) || chr == '_';
}

//! Finds the end of a Nginx log field
/*!
    \param string The format string
    \param start The starting point of the field in the string
    \return The ending poin of the field in the string
    \see processNginxFormatString()
*/
size_t findNginxFieldEnd( const std::string& string, const size_t start )
{
    if ( string.empty() || start >= string.size()-1ul ) {
        return start;
    }
    return std::distance( std::cbegin(string), std::find_if_not( string.cbegin()+start, string.cend(), checkNginxFieldChar ) ) - 1ul;
}


//! Checks whether the given character is valid or not
/*!
    \param chr The target character
    \return The result of the check
    \see checkIisString
*/
bool checkIisChar( const char& chr )
{
    return CharOps::isAlnum( chr )
        || chr == ' ' || chr == '-' || chr == ',' || chr == ':'
        || chr == '(' || chr == ')' || chr == '[' || chr == ']';
}

//! Checks whether the format string contains invalid characters or not
/*!
    \param string The format string
    \throw LogFormatException
    \see processIisFormatString
*/
void checkIisString( std::string_view string )
{
    if ( const auto it{ std::find_if_not( string.cbegin(), string.cend(), checkIisChar ) };
         it != string.cend() ) {
        // unwanted character
        throw LogFormatException( "Unexpected character found in string: '"+std::string{*it}+"'" );
    }
}

} // namespace (private)



LogsFormat FormatOps::processApacheFormatString( const std::string& f_str ) const
{
    if ( f_str.empty() ) {
        return LogsFormat();
    }

    const auto& f_map   { this->APACHE_ALF   };
    const auto& f_map_v { this->APACHE_ALF_V };

    std::string initial, final;
    std::vector<std::string> separators;
    std::vector<LogsFormatField> fields;
    // parse the string to convert keyargs in craplog's fields format
    bool is_strftime_sep{ false };
    int n_fld{ 0 };
    size_t start, stop{0ul}, aux, aux_start, aux_stop;
    const size_t max{ f_str.size()-1ul };
    std::string aux_fld, aux_fld_v, cur_sep;
    LogsFormatField cur_fld{ _INVALID };
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        if ( cur_fld == date_time_ncsa ) {
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
                    throw LogFormatException( "Invalid format string: ending with a single '%'" );

                } else if ( aux != std::string::npos ) {
                    // apache only escapes a format field using the double percent sign
                    // backslashes are valid for control-characters only, or get reduced
                    // single percent-signs are considered invalid
                    const char c{ f_str.at(aux+1ul) };
                    if ( c == '{' || c == '<' || c == '>' || c == '!' || c == ',' ) {
                        // in the first case: a composed format code must follow
                        // in the second case: expressing a status code format field
                        // in the last two cases: status code(s) may follow (or may not), which must be followed by a composed format code
                        // in any case is considered valid
                        break;
                    } else if ( c == '%' ) {
                        // the percent sign character, will be used as separator, skip
                        stop = aux + 2ul;
                        continue;
                    } else if ( ! CharOps::isAlnum( c ) ) {
                        // invalid, there must be a field code, a status code or a percent sign after a '%'
                        throw LogFormatException( "Invalid format: there must be a valid format code, a status code or a percent sign character after a '%', found: '%"+std::string{c}+"'" );
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
            ++ aux;

            char c = f_str.at( aux );
            // remove the per-status directives (if any)
            if ( CharOps::isNumeric( c ) || c == '!' || c == ',' ) {
                // per-status, not important for LogDoctor
                size_t aux_aux{ aux+1ul };
                while (true) {
                    if ( aux_aux > max ) {
                        break;
                    }
                    c = f_str.at( aux_aux );
                    if ( CharOps::isNumeric( c ) || c == '!' || c == ',' ) {
                        // skip these chars
                        ++ aux_aux;
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
                aux_start = aux + 1ul;
                aux = f_str.find( '}', aux_start );
                if ( aux == std::string::npos ) {
                    // closer bracket not found, resulting in an invalid field
                    throw LogFormatException( "Invalid format code, no closing bracket found: '%{...'" );
                }
                aux_stop = aux + 2ul;
                // get the varname(s)
                aux_fld = f_str.substr( aux_start, aux-aux_start );
                // get the module
                aux_fld_v = f_str.at( aux+1ul );
                if ( aux_fld_v == "^" ) {
                    aux_stop += 2ul;
                    aux_fld_v = f_str.substr( aux+1ul, 3ul );
                }
                if ( f_map_v.find( aux_fld_v ) == f_map_v.cend() ) {
                    // invalid module, abort
                    throw LogFormatException( "Invalid format code found: '%{...}"+aux_fld_v+"'" );
                } else {
                    // module is valud
                    const auto& aux_map{ f_map_v.at( aux_fld_v ) };
                    if ( aux_map.empty() ) {
                        // module not considered and always giving out something, even if invalid varname is passed
                        cur_fld = _DISCARDED;

                    } else if ( aux_fld.empty() ) {
                        // no need to check further, the dafault is used in this case
                        cur_fld = aux_map.at( aux_fld );
                        if ( aux_fld_v == "t" ) {
                            cur_sep += "[";
                        }

                    } else if ( aux_fld_v == "p" || aux_fld_v == "P" || aux_fld_v == "T" ) {
                        // still not considered (except 'T'), but invalid fields get used as text
                        // field concatenation not allowed, whole content used as varname
                        if ( aux_map.contains( aux_fld ) ) {
                            // valid varname
                            cur_fld = aux_map.at( aux_fld );
                        } else {
                            // invalid varname, use as text
                            cur_fld = _INVALID;
                            cur_sep += aux_fld;
                            start = stop = aux_stop;
                            continue;
                        }

                    } else if ( aux_fld_v == "a" || aux_fld_v == "h" ) {
                        // whatever the varname is (valid, invalid, empty), always returns the client
                        // field concatenation not allowed, the entire content is used as varname
                        cur_fld = client;

                    } else if ( aux_fld_v == "i" ) {
                        // always giving a result, may the varname be valid or not ('-' if invalid/empty)
                        // field concatenation not allowed, the entire content is used as varname
                        if ( aux_map.contains( aux_fld ) ) {
                            cur_fld = aux_map.at( aux_fld );
                        } else {
                            cur_fld = _DISCARDED;
                        }

                    } else /*if ( aux_fld_v == "t" )*/ {
                        // only 't' remaining
                        size_t aux_aux{ aux_fld.find( '%' ) };
                        if ( aux_aux == std::string::npos ) {
                            // no concatenation, only valid fields used, anything else used as text
                            // whole content used as varname
                            if ( aux_map.contains( aux_fld ) ) {
                                // valid
                                cur_fld = aux_map.at( aux_fld );
                                is_strftime_sep = true;
                            } else {
                                // invalid, append to current separator
                                cur_fld = _INVALID;
                                cur_sep += aux_fld;
                                start = stop = aux_stop;
                                continue;
                            }

                        } else {
                            // concatenation allowed, only strftime() value used as fields, everything else treated as text
                            size_t aux_aux_start,
                                   aux_aux_stop{0};
                            std::string aux_aux_fld;
                            while (true) {
                                // loop inside the composed field
                                aux_aux_start = aux_aux_stop;
                                while (true) {
                                    // hunt the next field
                                    aux_aux = aux_fld.find( '%', aux_aux_stop );
                                    // check if false positive
                                    if ( aux_aux != std::string::npos ) {
                                        const size_t aux_aux_{ aux_aux+1ul };
                                        if ( aux_aux_ >= aux_fld.size() ) {
                                            aux_aux = std::string::npos;
                                            break;
                                        }
                                        // same escape rules as before, but single percent-signs are considered valid and treated as text
                                        const char c_ = aux_fld.at( aux_aux_ );
                                        if ( c_ == '%' || c_ == 'n' || c_ == 't' ) {
                                            // control characters, will be used as separator, skip
                                            aux_aux_stop = aux_aux + 2ul;
                                            continue;
                                        }
                                    }
                                    break;
                                }

                                if ( aux_aux == std::string::npos ) {
                                    // no more fields, append the last section as separator
                                    cur_sep += parseApacheEscapes( aux_fld.substr( aux_aux_start ), true );
                                    break;
                                }

                                // append to the current separator
                                cur_sep += parseApacheEscapes( aux_fld.substr( aux_aux_start, aux_aux-aux_aux_start ), true );
                                // and get the possible field
                                aux_aux_fld = aux_fld.substr( aux_aux, 2ul );
                                aux_aux_stop = aux_aux+2ul;
                                // check if the field is valid
                                if ( aux_map.contains( aux_aux_fld ) ) {
                                    // valid, append
                                    cur_fld = aux_map.at( aux_aux_fld );
                                    // append the separator
                                    if ( n_fld == 0 ) {
                                        // first field found, assign the separator as the initial one
                                        initial = cur_sep;

                                    } else {
                                        // append to separators list
                                        if ( cur_sep.empty() ) {
                                            throw LogFormatException( "Empty separator found, it may be impossible to distinguish two contiguous fields" );
                                        }
                                        separators.push_back( cur_sep );
                                    }
                                    cur_sep.clear();
                                    // append the field
                                    fields.push_back( cur_fld );
                                    ++ n_fld;

                                } else {
                                    // invalid, append as separator and keep hunting
                                    cur_sep += parseApacheEscapes( aux_aux_fld, true );
                                }
                            }
                            start = stop = aux_stop; // everything appended already, restart hunting
                            continue;
                        }
                    }
                    stop = aux_stop; // re-starting after the previously found module
                    break;
                }

            } else {
                // normal
                aux_fld = c;
                aux_stop = aux+1ul;
                if ( aux_fld == ">" || aux_fld == "<" ) {
                    aux_fld += f_str.at( aux+1 );
                    ++ aux_stop;
                }
                // check if the module is valid
                if ( f_map.contains( aux_fld ) ) {
                    // valid
                    cur_fld = f_map.at( aux_fld );
                    if ( cur_fld == date_time_ncsa ) {
                        // apache's NCSA time format is always enclosed inside brackets
                        cur_sep += "[";
                    }
                    stop = aux_stop;
                    break;
                } else {
                    // invalid format field, abort
                    throw LogFormatException( "Invalid format code found: '%"+aux_fld+"'" );
                }
            }
        }
        // outside hunting loop

        if ( n_fld < 0 ) {
            // final reached, stop looping
            if ( fields.empty() ) {
                initial = parseApacheEscapes( cur_sep, is_strftime_sep );
            } else {
                final = parseApacheEscapes( cur_sep, is_strftime_sep );
            }
            break;
        }

        if ( cur_fld == _INVALID ) {
            // invalid field, used as text (namely, added to current separator)
            continue;
        }

        // append the separator
        if ( n_fld == 0 ) {
            // first field found, assign the separator as the initial one
            initial = parseApacheEscapes( cur_sep, is_strftime_sep );

        } else {
            // append to separators list
            if ( cur_sep.empty() ) {
                throw LogFormatException( "Empty separator found, it may be impossible to distinguish two contiguous fields" );
            }
            separators.push_back( parseApacheEscapes( cur_sep, is_strftime_sep ) );
        }
        cur_sep.clear();
        is_strftime_sep = false;

        // append the field
        fields.push_back( cur_fld );
        ++ n_fld;
    }

    return LogsFormat(
        f_str, std::move(initial), std::move(final),
        std::move(separators), std::move(fields),
        countNewLines( initial, final, separators ) );

}
// sample
QString FormatOps::getApacheLogSample( const LogsFormat& log_format ) const noexcept
{
    QString sample;
    const auto& map{ this->APACHE_ALF_SAMPLES };

    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( size_t i{0ul}; i<log_format.separators.size(); ++i ) {
        // append fields and separators
        sample += map.at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0ul ) {
        sample += map.at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
}



LogsFormat FormatOps::processNginxFormatString( const std::string& f_str ) const
{
    if ( f_str.empty() ) {
        return LogsFormat();
    }

    const auto& f_map{ this->NGINX_ALF };

    std::string initial, final;
    std::vector<std::string> separators;
    std::vector<LogsFormatField> fields;
    // parse the string to convert keyargs in craplog's fields format
    bool finished{ false };
    size_t start, aux, stop{0ul};
    const size_t max{ f_str.size()-1ul };
    std::string cur_fld, cur_sep;
    // find and convert any field
    while (true) {
        // start after the last found field
        start = stop;
        // find the next field
        aux = f_str.find( '$', start );
        if ( aux == std::string::npos ) {
            // not found, append as final and stop searching
            final = parseNginxEscapes( f_str.substr( start ) );
            break;
        }
        ++ aux;
        // find the end of the current field
        stop = findNginxFieldEnd( f_str, aux );
        if ( stop == max ) {
            // this is the last field, and ther's no final separator
            finished |= true;
        }
        ++ stop;

        cur_sep = f_str.substr( start, aux-start-1ul );
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
        if ( const auto it{ f_map.find( cur_fld ) }; it != f_map.end() ) {
            // valid, append
            if ( start == 0ul ) {
                initial = parseNginxEscapes( cur_sep );
            } else {
                separators.push_back( parseNginxEscapes( cur_sep ) );
            }
            fields.push_back( it->second );
            if ( finished ) {
                // this was the last field
                break;
            }
        } else {
            // invalid, abort
            throw LogFormatException( "Invalid format code found: '$"+cur_fld+"'" );
        }
    }

    return LogsFormat(
        f_str, std::move(initial), std::move(final),
        std::move(separators), std::move(fields),
        countNewLines( initial, final, separators ) );
}
// sample
QString FormatOps::getNginxLogSample( const LogsFormat& log_format ) const noexcept
{
    QString sample;
    const auto& map{ this->NGINX_ALF_SAMPLES };

    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( size_t i{0}; i<log_format.separators.size(); ++i ) {
        // append fields and separators
        sample += map.at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0ul ) {
        sample += map.at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
}



LogsFormat FormatOps::processIisFormatString( const std::string& f_str, const IISLogsModule l_mod ) const
{
    checkIisString( f_str );
    std::string initial, final;
    std::vector<std::string> separators;
    std::vector<LogsFormatField> fields;
    switch ( l_mod ) {
        case IISLogsModule::IIS:
            // IIS logging module
            final = ",";
            separators = {", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", "};
            fields = {client,_DISCARDED,date_time_mdyyyy,date_time_utc_t,_DISCARDED,_DISCARDED,_DISCARDED,time_taken_ms,bytes_received,bytes_sent,response_code,_DISCARDED,request_method,request_uri,request_query};
            break;
        case IISLogsModule::NCSA:
            // NCSA logging module
            separators = {" "," "," [","] \"","\" "," "};
            fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,bytes_sent};
            break;
        case IISLogsModule::W3C:
            // W3C logging module
            if ( f_str.size() > 0ul ) {
                bool finished{ false };
                size_t start, stop{0ul};
                const size_t max{ f_str.size()-1ul };
                std::string cur_fld;
                const std::string cur_sep{ " " };
                const auto& f_map{ this->IIS_ALF };
                // parse the string to convert keyargs in craplog's fields format
                while (true) {
                    // start after the last found separator
                    start = stop;
                    // find the next separator, which is always a single whitespace, in this case
                    stop = f_str.find( cur_sep, start );
                    if ( stop == std::string::npos ) {
                        // not found, this is the last field
                        stop = max+1ul;
                        finished |= true;
                    }

                    // set the current field
                    cur_fld = f_str.substr( start, stop-start );
                    // step over the separator
                    ++ stop;

                    // check if the module is valid
                    if ( const auto it{ f_map.find( cur_fld ) }; it != f_map.end() ) {
                        // valid, append
                        fields.push_back( it->second );
                        if ( ! finished ) {
                            separators.push_back( cur_sep );
                        } else {
                            // this was the last field
                            break;
                        }
                    } else {
                        // invalid, abort
                        throw LogFormatException( "Invalid format code found: '"+cur_fld+"'" );
                    }
                }
            }
            // outside search loop, killing the switch
            break;

        default:
            // shouldn't be here
            throw LogFormatException( "Unexpected LogModule for IIS: "+std::to_string( static_cast<unsigned char>(l_mod) ) );
    }

    return LogsFormat(
        f_str, std::move(initial), std::move(final),
        std::move(separators), std::move(fields), 0 );
}
// sample
QString FormatOps::getIisLogSample( const LogsFormat& log_format ) const noexcept
{
    QString sample;
    const auto& map{ this->IIS_ALF_SAMPLES };

    // append the initial characters
    sample += QString::fromStdString( log_format.initial );
    for ( size_t i{0}; i<log_format.separators.size(); ++i ) {
        // append fields and separators
        sample += map.at( log_format.fields.at( i ) );
        sample += QString::fromStdString( log_format.separators.at( i ) );
    }
    // add the last field
    if ( log_format.fields.size() > 0ul ) {
        sample += map.at( log_format.fields.back() );
    }
    // and the final characters
    sample += QString::fromStdString( log_format.final );
    return sample;
}

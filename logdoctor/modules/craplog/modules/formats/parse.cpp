
#include "parse.h"

#include "modules/craplog/modules/lib.h"

#include "modules/exceptions.h"
#include "utilities/strings.h"

#include <vector>
#include <unordered_map>


using simple_fields_umap_t = std::unordered_map<std::string, LogsFormatField>;
using composed_fields_umap_t = std::unordered_map<std::string, std::unordered_map<std::string, LogsFormatField>>;


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
std::size_t countNewLines( std::string_view initial, std::string_view final, const std::vector<std::string>& separators );


//! Parses the escapes (backslashes) and returns the resulting string
/*!
    Used to obtain the same result as on Apache2
    \param string The string to parse
    \param strftime Whether to apply the strftime special rule when parsing or not
    \return The resulting string
    \throw LogFormatException
    \see processApacheFormatString()
*/
std::string parseApacheEscapes( std::string_view string , const bool strftime=false );


//! Parses the escapes (backslashes) and returns the resulting string
/*!
    Used to obtain the same result as on Nginx
    \param string The string to parse
    \return The resulting string
    \throw LogFormatException
    \see processNginxFormatString()
*/
std::string parseNginxEscapes( std::string_view string );

//! Checks whether the given character is valid or not
/*!
    \param chr The target character
    \return The result of the check
    \see findNginxFieldEnd
*/
bool checkNginxFieldChar( const char& chr );

//! Finds the end of a Nginx log field
/*!
    \param string The format string
    \param start The starting point of the field in the string
    \return The ending poin of the field in the string
    \see processNginxFormatString()
*/
std::size_t findNginxFieldEnd( const std::string& string, const std::size_t start );


//! Checks whether the given character is valid or not
/*!
    \param chr The target character
    \return The result of the check
    \see checkIisString
*/
bool checkIisChar( const char& chr );

//! Checks whether the format string contains invalid characters or not
/*!
    \param string The format string
    \throw LogFormatException
    \see processIisFormatString
*/
void checkIisString( std::string_view string );


//! Returns the map of all Apache fields
/*!
    \return The map of Apache fields
*/
const simple_fields_umap_t& getApacheFields();

//! Returns the map of all Apache composed fields
/*!
    \return The map of Apache composed fields
*/
const composed_fields_umap_t& getApacheComposedFields();


//! Returns the map of all Nginx fields
/*!
    \return The map of Nginx fields
*/
const simple_fields_umap_t& getNginxFields();


//! Returns the map of all IIS fields
/*!
    \return The map of IIS fields
*/
const simple_fields_umap_t& getIisFields();

} // namespace (private)



namespace FormatOps::Private
{

LogsFormat parseApacheFormatString( const std::string& f_str )
{
    if ( f_str.empty() ) {
        return LogsFormat();
    }

    const auto& f_map   { getApacheFields()         };
    const auto& f_map_v { getApacheComposedFields() };

    std::string initial, final;
    std::vector<std::string> separators;
    std::vector<LogsFormatField> fields;
    // parse the string to convert keyargs in craplog's fields format
    bool is_strftime_sep{ false };
    int n_fld{ 0 };
    std::size_t start, stop{0ul}, aux, aux_start, aux_stop;
    const std::size_t max{ f_str.size()-1ul };
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
                std::size_t aux_aux{ aux+1ul };
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
                        std::size_t aux_aux{ aux_fld.find( '%' ) };
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
                            std::size_t aux_aux_start,
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
                                        const std::size_t aux_aux_{ aux_aux+1ul };
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



LogsFormat parseNginxFormatString( const std::string& f_str )
{
    if ( f_str.empty() ) {
        return LogsFormat();
    }

    const auto& f_map{ getNginxFields() };

    std::string initial, final;
    std::vector<std::string> separators;
    std::vector<LogsFormatField> fields;
    // parse the string to convert keyargs in craplog's fields format
    bool finished{ false };
    std::size_t start, aux, stop{0ul};
    const std::size_t max{ f_str.size()-1ul };
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



LogsFormat parseIisFormatString( const std::string& f_str, const IISLogsModule l_mod )
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
                std::size_t start, stop{0ul};
                const std::size_t max{ f_str.size()-1ul };
                std::string cur_fld;
                const std::string cur_sep{ " " };
                const auto& f_map{ getIisFields() };
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

} // namespace FormatOps::Private



namespace /*private*/
{

std::size_t countNewLines( std::string_view initial, std::string_view final, const std::vector<std::string>& separators )
{
    std::size_t nl{ 0ul };
    nl += StringOps::count( initial, '\n' );
    nl += StringOps::count( final, '\n' );
    for ( const std::string& sep : separators ) {
            nl += StringOps::count( sep, '\n' );
    }
    return nl;
}


std::string parseApacheEscapes( std::string_view string , const bool strftime )
{
    if ( string.empty() ) {
            return std::string{string};
    }
    std::size_t i{ 0ul },
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


std::string parseNginxEscapes( std::string_view string )
{
    if ( string.empty() ) {
            return std::string{string};
    }
    std::size_t i{ 0ul };
    const std::size_t max{ string.size()-1ul };
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

bool checkNginxFieldChar( const char& chr )
{
    return CharOps::isAlnum( chr ) || chr == '_';
}

std::size_t findNginxFieldEnd( const std::string& string, const std::size_t start )
{
    if ( string.empty() || start >= string.size()-1ul ) {
            return start;
    }
    return std::distance( std::cbegin(string), std::find_if_not( string.cbegin()+start, string.cend(), checkNginxFieldChar ) ) - 1ul;
}


bool checkIisChar( const char& chr )
{
    return CharOps::isAlnum( chr )
           || chr == ' ' || chr == '-' || chr == ',' || chr == ':'
           || chr == '(' || chr == ')' || chr == '[' || chr == ']';
}

void checkIisString( std::string_view string )
{
    if ( const auto it{ std::find_if_not( string.cbegin(), string.cend(), checkIisChar ) };
        it != string.cend() ) {
            // unwanted character
            throw LogFormatException( "Unexpected character found in string: '"+std::string{*it}+"'" );
    }
}


const simple_fields_umap_t& getApacheFields()
{
    static const simple_fields_umap_t apache_fields{
        {"a",  _DISCARDED},
        {"A",  _DISCARDED},
        {"b",  _DISCARDED},
        {"B",  _DISCARDED},
        {"C",  _DISCARDED},
        {"D",  time_taken_ms},
        {"e",  _DISCARDED},
        {"f",  _DISCARDED},
        {"h",  client},
        {"H",  request_protocol},
        {"i",  _DISCARDED},
        {"I",  bytes_received},
        {"k",  _DISCARDED},
        {"l",  _DISCARDED},
        {"L",  _DISCARDED},
        {"m",  request_method},
        {"n",  _DISCARDED},
        {"o",  _DISCARDED},
        {"O",  bytes_sent},
        {"p",  _DISCARDED},
        {"P",  _DISCARDED},
        {"q",  request_query},
        {"r",  request_full},
        {"R",  _DISCARDED},
        {"s",  response_code},
        {"<s", response_code},
        {">s", response_code},
        {"S",  _DISCARDED},
        {"t",  date_time_ncsa},
        {"T",  time_taken_s},
        {"u",  _DISCARDED},
        {"U",  request_uri},
        {"v",  _DISCARDED},
        {"V",  _DISCARDED},
        {"X",  _DISCARDED} };
    return apache_fields;
}

const composed_fields_umap_t& getApacheComposedFields()
{
    static const composed_fields_umap_t apache_composed_fields{
        {"a", { {"",           client}, // as %h (from single fields)
                {"c",          client}}},
        {"h", { {"",           client}, // as %h (from single fields)
                {"c",          client}}},
        {"i", { {"",           _DISCARDED},
                {"Cookie",     cookie},
                {"Referer",    referer},
                {"User-agent", user_agent}}},
        {"t", { {"",           date_time_ncsa}, // as %t (from single fields)
              //{"%%",         _DISCARDED}, // the percent sign
              //{"%n",         _DISCARDED}, // line feed
              //{"%t",         _DISCARDED}, // horizontal tab
                {"sec",        date_time_epoch_s},
                {"msec",       date_time_epoch_ms},
                {"usec",       date_time_epoch_us},
                {"msec_frac",  _DISCARDED}, // milliseconds fraction
                {"usec_frac",  _DISCARDED}, // microseconds fraction
                {"%a",         _DISCARDED}, // abbreviated weekday name
                {"%A",         _DISCARDED}, // weekday name
                {"%b",         date_time_month_str},
                {"%B",         date_time_month_str},
                {"%c",         date_time_mcs},
                {"%C",         _DISCARDED}, // year (first 2 digits, aka centuries)
                {"%d",         date_time_day},
                {"%D",         date_time_mmddyy},
                {"%e",         date_time_day},
                {"%F",         date_time_yyyymmdd},
                {"%g",         _DISCARDED}, // weel-based year (last 2 digits)
                {"%G",         _DISCARDED}, // week-based year, namely the year which contains the current week
                {"%h",         date_time_month_str},
                {"%H",         date_time_hour},
                {"%I",         _DISCARDED}, // hour (12h format)
                {"%j",         _DISCARDED}, // day of the year number
                {"%k",         date_time_hour}, // ?!? hour (24h format) !?! no documentation ?!?
                {"%l",         _DISCARDED}, // hour (12h format)
                {"%m",         date_time_month},
                {"%M",         date_time_minute},
                {"%p",         _DISCARDED}, // AM or PM
                {"%r",         date_time_clock_12},
                {"%R",         date_time_clock_short},
                {"%s",         date_time_epoch_s},
                {"%S",         date_time_second},
                {"%T",         date_time_clock_24},
                {"%u",         _DISCARDED}, // weekday number (1-7, Monday is 1)
                {"%U",         _DISCARDED}, // week of the year number, with the first Sunday as the first day of week one
                {"%V",         _DISCARDED}, // week of the year number
                {"%w",         _DISCARDED}, // weekday number (0-6, Sunday is 0)
                {"%W",         _DISCARDED}, // week of the year number, with the first Monday as the first day of week one
                {"%x",         date_time_mmddyy},
                {"%X",         date_time_clock_24},
                {"%y",         date_time_year_short},
                {"%Y",         date_time_year},
                {"%z",         _DISCARDED}, // timezone offset from UTC (1 minute=1, 1 hour=100)
                {"%Z",         _DISCARDED}}}, // timezone name abbreviation
        {"T", { {"",           time_taken_s}, // as s
                {"s",          time_taken_s},
                {"ms",         time_taken_ms},
                {"us",         time_taken_us}}},
        // composed not in use
        {"C",   {}},
        {"e",   {}},
        {"L",   {}},
        {"n",   {}},
        {"o",   {}},
        {"p",   {{"",          _DISCARDED}, // as canonical
                 {"canonical", _DISCARDED},
                 {"local",     _DISCARDED},
                 {"remote",    _DISCARDED}}},
        {"P",   {{"",          _DISCARDED}, // as pid
                 {"pid",       _DISCARDED},
                 {"tid",       _DISCARDED},
                 {"hextid",    _DISCARDED}}},
        {"^ti", {}},
        {"^to", {}} };
    return apache_composed_fields;
}


const simple_fields_umap_t& getNginxFields()
{
    static const simple_fields_umap_t nginx_fields{
        {"remote_addr",        client},
        {"realip_remote_addr", client},
        {"time_local",         date_time_ncsa},
        {"time_iso8601",       date_time_iso},
        {"date_gmt",           date_time_gmt},
        {"msec",               date_time_epoch_s_ms},
        {"request",            request_full},
        {"server_protocol",    request_protocol},
        {"request_method",     request_method},
        {"request_uri",        request_uri_query},
        {"uri",                request_uri},
        {"query_string",       request_query},
        {"status",             response_code},
        {"bytes_sent",         bytes_sent},
        {"request_length",     bytes_received},
        {"request_time",       time_taken_s_ms},
        {"http_referer",       referer},
        {"cookie_",            cookie},
        {"http_user_agent",    user_agent},
        // not in use, will be discarded
        {"ancient_browser",           _DISCARDED},
        {"arg_",                      _DISCARDED},
        {"args",                      _DISCARDED},
        {"binary_remote_addr",        _DISCARDED},
        {"body_bytes_sent",           _DISCARDED},
        {"connection",                _DISCARDED},
        {"connection_requests",       _DISCARDED},
        {"connections_active",        _DISCARDED},
        {"connections_reading",       _DISCARDED},
        {"connections_waiting",       _DISCARDED},
        {"connections_writing",       _DISCARDED},
        {"content_length",            _DISCARDED},
        {"content_type",              _DISCARDED},
        {"date_local",                _DISCARDED},
        {"document_root",             _DISCARDED},
        {"document_uri",              _DISCARDED},
        {"fastcgi_path_info",         _DISCARDED},
        {"fastcgi_script_name",       _DISCARDED},
        {"geoip_area_code",           _DISCARDED},
        {"geoip_city",                _DISCARDED},
        {"geoip_city_continent_code", _DISCARDED},
        {"geoip_city_country_code",   _DISCARDED},
        {"geoip_city_country_code3",  _DISCARDED},
        {"geoip_city_country_name",   _DISCARDED},
        {"geoip_country_code",        _DISCARDED},
        {"geoip_country_code3",       _DISCARDED},
        {"geoip_country_name",        _DISCARDED},
        {"geoip_dma_code",            _DISCARDED},
        {"geoip_latitude",            _DISCARDED},
        {"geoip_longitude",           _DISCARDED},
        {"geoip_org",                 _DISCARDED},
        {"geoip_postal_code",         _DISCARDED},
        {"geoip_region",              _DISCARDED},
        {"geoip_region_name",         _DISCARDED},
        {"gzip_ratio",                _DISCARDED},
        {"host",                      _DISCARDED},
        {"hostname",                  _DISCARDED},
        {"http2",                     _DISCARDED},
        {"http_",                     _DISCARDED},
        {"https",                     _DISCARDED},
        {"invalid_referer",           _DISCARDED},
        {"is_args",                   _DISCARDED},
        {"limit_rate",                _DISCARDED},
        {"memcached_key",             _DISCARDED},
        {"modern_browser",            _DISCARDED},
        {"msie",                      _DISCARDED},
        {"nginx_version",             _DISCARDED},
        {"pid",                       _DISCARDED},
        {"pipe",                      _DISCARDED},
        {"proxy_add_x_forwarded_for", _DISCARDED},
        {"proxy_host",                _DISCARDED},
        {"proxy_port",                _DISCARDED},
        {"proxy_protocol_addr",       _DISCARDED},
        {"proxy_protocol_port",       _DISCARDED},
        {"realip_remote_port",        _DISCARDED},
        {"realpath_root",             _DISCARDED},
        {"remote_port",               _DISCARDED},
        {"remote_user",               _DISCARDED},
        {"request_body",              _DISCARDED},
        {"request_body_file",         _DISCARDED},
        {"request_completion",        _DISCARDED},
        {"request_filename",          _DISCARDED},
        {"request_id",                _DISCARDED},
        {"scheme",                    _DISCARDED},
        {"secure_link",               _DISCARDED},
        {"secure_link_expires",       _DISCARDED},
        {"sent_http_",                _DISCARDED},
        {"server_addr",               _DISCARDED},
        {"server_name",               _DISCARDED},
        {"server_port",               _DISCARDED},
        {"session_log_binary_id",     _DISCARDED},
        {"session_log_id",            _DISCARDED},
        {"slice_range",               _DISCARDED},
        {"spdy",                      _DISCARDED},
        {"spdy_request_priority",     _DISCARDED},
        {"ssl_cipher",                _DISCARDED},
        {"ssl_client_cert",           _DISCARDED},
        {"ssl_client_fingerprint",    _DISCARDED},
        {"ssl_client_i_dn",           _DISCARDED},
        {"ssl_client_raw_cert",       _DISCARDED},
        {"ssl_client_s_dn",           _DISCARDED},
        {"ssl_client_serial",         _DISCARDED},
        {"ssl_client_verify",         _DISCARDED},
        {"ssl_protocol",              _DISCARDED},
        {"ssl_server_name",           _DISCARDED},
        {"ssl_session_id",            _DISCARDED},
        {"ssl_session_reused",        _DISCARDED},
        {"tcpinfo_rtt",               _DISCARDED},
        {"tcpinfo_rttvar",            _DISCARDED},
        {"tcpinfo_snd_cwnd",          _DISCARDED},
        {"tcpinfo_rcv_space",         _DISCARDED},
        {"uid_got",                   _DISCARDED},
        {"uid_reset",                 _DISCARDED},
        {"uid_set",                   _DISCARDED},
        {"upstream_addr",             _DISCARDED},
        {"upstream_cache_status",     _DISCARDED},
        {"upstream_connect_time",     _DISCARDED},
        {"upstream_cookie_",          _DISCARDED},
        {"upstream_header_time",      _DISCARDED},
        {"upstream_http_",            _DISCARDED},
        {"upstream_response_length",  _DISCARDED},
        {"upstream_response_time",    _DISCARDED},
        {"upstream_status",           _DISCARDED} };
    return nginx_fields;
}


const simple_fields_umap_t& getIisFields()
{
    // W3C module
    static const simple_fields_umap_t iis_fields{
        {"date",             date_time_utc_d},
        {"time",             date_time_utc_t},
        {"cs-version",       request_protocol},
        {"cs-method",        request_method},
        {"cs-uri-stem",      request_uri},
        {"cs-uri-query",     request_query},
        {"sc-status",        response_code},
        {"sc-bytes",         bytes_sent},
        {"cs-bytes",         bytes_received},
        {"time-taken",       time_taken_ms},
        {"cs(Referer)",      referer},
        {"cs(Cookie)",       cookie},
        {"cs(User-Agent)",   user_agent},
        {"c-ip",             client},
        // not in use, will be discarded
        {"s-sitename",        _DISCARDED},
        {"s-computername",    _DISCARDED},
        {"s-ip",              _DISCARDED},
        {"s-port",            _DISCARDED},
        {"cs-username",       _DISCARDED},
        {"cs-host",           _DISCARDED},
        {"sc-substatus",      _DISCARDED},
        {"sc-win32-status",   _DISCARDED},
        {"streamid",          _DISCARDED} };
    return iis_fields;
}

} // namespace (private)

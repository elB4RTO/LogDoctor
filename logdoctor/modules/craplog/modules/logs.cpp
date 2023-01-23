
#include "logs.h"

#include "modules/exceptions.h"
#include "modules/craplog/modules/datetime.h"

#include "utilities/strings.h"


LogOps::LogOps()
{

}


const LogOps::LogType LogOps::defineFileType( const std::vector<std::string>& lines, const FormatOps::LogsFormat& format ) const
{
    if ( lines.size() == 0 ) {
        // empty content
        return LogOps::LogType::Failed;
    }

    int n_access=0, n_other=0;
    LogOps::LogType log_type;

    // real type assignment
    log_type = LogOps::LogType::Failed;
    for ( const std::string& line : lines ) {
        // scan
        if ( this->deepTypeCheck( line, format ) ) {
            n_access++;
        } else {
            n_other++;
        }
    }

    // final decision
    if ( n_access > 0 && n_other == 0 ) {
        // access logs
        log_type = LogOps::LogType::Access;
    } else if ( n_other > 0 && n_access == 0 ) {
        // other format, maybe error logs
        log_type = LogOps::LogType::Discarded;
    } else {
        // something is wrong with this file, keep the Failed type
    }
    return log_type;
}


const bool LogOps::deepTypeCheck( const std::string& line, const FormatOps::LogsFormat& format ) const
{
    int n_sep_found=0, n_blank_sep=0,
        n_sep = format.separators.size();
    size_t found_at, aux_found_at1=0, aux_found_at2;
    std::string sep, aux_sep1, aux_sep2;
    // check the initial part
    if ( format.initial.size() > 0 ) {
        if ( StringOps::startsWith( line, format.initial ) ) {
            n_sep_found ++;
        }
    } else {
        n_sep_found ++;
        n_blank_sep ++;
    }
    // check the middle part
    for ( int i=0; i<n_sep; i++ ) {
        sep = format.separators.at( i );
        if ( sep == "" ) {
            n_sep_found ++;
            n_blank_sep ++;
            continue;
        }
        aux_found_at2 = aux_found_at1;
        found_at = line.find( sep, aux_found_at2 );
        if ( found_at == std::string::npos ) {
            // not found
            continue;
        }

        if ( i+1 <= n_sep ) {
            // not the last separator, check the possibility of missing
            aux_sep1 = sep;
            aux_found_at1 = aux_sep1.find(' ');
            if ( aux_found_at1 != std::string::npos ) {
                aux_sep1 = StringOps::lstripUntil( aux_sep1, " " );
            }
            // iterate over following separators
            for ( int j=i+1; j<n_sep; j++ ) {
                aux_sep2 = format.separators.at( j );
                aux_found_at2 = aux_sep2.find(' ');
                if ( aux_found_at2 == std::string::npos ) {
                    aux_found_at2 = found_at;
                } else {
                    aux_found_at2 = found_at + aux_found_at2 + 1;
                    aux_sep2 = StringOps::lstripUntil( aux_sep2, " " );
                }
                // if the 2 seps are identical, skip (for uncertainty)
                if ( aux_sep1 == aux_sep2 || aux_sep2 == "" ) {
                    continue;
                }
                // check if the next sep is found in the same position of the current one
                if ( line.find( aux_sep2, aux_found_at2 ) == aux_found_at2 ) {
                    // probably the current field is missing, skip to this one
                    i = j;
                    sep = format.separators.at( j );
                }
                break;
            }
        }
        aux_found_at1 = found_at + sep.size();
        n_sep_found ++;

    }

    // check the final part
    if ( format.final.size() > 0 ) {
        if ( StringOps::endsWith( line, format.final ) ) {
            n_sep_found ++;
        }
    } else {
        n_sep_found ++;
        n_blank_sep ++;
    }

    // add the initial and final seps now
    n_sep += 2;

    // the result is considered ture if more then a half of the seps was found
    // and more than a half of the found separators was not blank
    bool result = false;
    if ( n_sep_found >= n_sep-1
      && n_blank_sep <= n_sep_found/2  ) {
        result = true;
    }

    return result;
}


void LogOps::cleanLines( std::vector<std::string>& lines ) const
{
    std::vector<std::string> aux;
    for ( const std::string& line : lines ) {
        if ( !StringOps::startsWith( line, "#" ) ) {
            // not a commented line
            aux.push_back( line );
        }
    }
    lines = std::move( aux );
}



const std::unordered_map<int, std::string> LogOps::parseLine( const std::string& line, const FormatOps::LogsFormat& format )
{
    std::unordered_map<int, std::string> data;
    std::string sep, fld, fld_str;
    bool add_pm=false;
    size_t start, stop=0, aux_start, aux_stop,
           line_size = line.size()-1;
    int i=0, n_sep=format.separators.size()-1;

    // add the initial chars
    stop = format.initial.size();

    while (true) {
        // split fields
        start = stop; // stop updated at the end of the loop
        if ( i <= n_sep ) {
            sep = format.separators.at( i );
            stop = line.find( sep, start );
        } else if ( i == n_sep+1 ) {
            // final separator
            sep = format.final;
            if ( sep == "" ) {
                stop = line_size+1;
            } else {
                stop = line.find( sep, start );
                if ( stop == std::string::npos ) {
                    stop = line_size +1;
                }
            }
        } else {
            // no more separators
            break;
        }
        if ( stop == std::string::npos ) {
            // separator not found, abort
            throw LogParserException( "Separator not found", sep );
        }

        // get the field
        fld = format.fields.at( i );
        if ( fld != "NONE" ) {
            // only parse the considered fields
            fld_str = StringOps::strip( line.substr(start, stop-start), " " );

            if ( i+1 <= n_sep ) {
                // not the last separator, check for mistakes
                bool ok = true;
                aux_stop = stop;

                if ( sep == " " ) {
                    // whitespace-separated-values fields
                    int c = StringOps::count( fld_str, sep ),
                        n = 0;
                    if ( fld == "request_full" ) {
                        n = 2;
                    } else if ( fld == "date_time_mcs" ) {
                        n = 4;
                    } else if ( fld == "date_time_ncsa" ) {
                        n = 1;
                    } else if ( fld == "date_time_gmt" ) {
                        n = 3;
                    }
                    if ( n > 0 && c < n ) {
                        // loop until the correct number of whitespaces is reached
                        aux_start = stop + 1;
                        while ( c < n ) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                ok = false;
                                break;
                            }
                            aux_start = aux_stop + 1;
                            c++;
                        }
                    }

                } else if ( fld == "user_agent" && StringOps::startsWith( sep, "\"" ) ) {
                    // atm the only support is for escaped quotes
                    if ( fld_str.back() == '\\' ) {
                        aux_start = stop + sep.size();
                        while (true) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                break;
                            } else if ( line.at( aux_stop-1 ) != '\\' ) {
                                // non-backslashed quotes
                                break;
                            }
                            aux_start = aux_stop + sep.size();
                        }
                    }
                }

                // finally update if needed
                if ( ok && aux_stop >= stop ) {
                    stop = aux_stop;
                    fld_str = StringOps::strip( line.substr(start, stop-start), " " );
                }
            }

            // process the field
            this->parsed_size += fld_str.size();

            if ( fld_str != "" ) {
                int fld_id = this->field2id.at(fld);
                if ( fld_id > 0 ) {
                    // no need to process, append directly if non-empty
                    data.emplace( fld_id, fld_str );

                } else {
                    // process the field

                    // process the date to get year, month, day, hour and minute
                    if ( StringOps::startsWith( fld, "date_time" ) ) {
                        const std::vector<std::string> dt = DateTimeOps::processDateTime( fld_str, fld.substr( 10 ) ); // cut away the "date_time_" part which is useless from now on
                        if ( dt.at( 0 ) != "" ) {
                            // year
                            data.emplace( this->field2id.at("date_time_year"), dt.at( 0 ) );
                        }
                        if ( dt.at( 1 ) != "" ) {
                            // month
                            data.emplace( this->field2id.at("date_time_month"), dt.at( 1 ) );
                        }
                        if ( dt.at( 2 ) != "" ) {
                            // day
                            data.emplace( this->field2id.at("date_time_day"), dt.at( 2 ) );
                        }
                        if ( dt.at( 3 ) != "" ) {
                            // hour
                            if ( dt.at( 3 ) == "PM" ) {
                                add_pm = true;
                            } else {
                                data.emplace( this->field2id.at("date_time_hour"), dt.at( 3 ) );
                            }
                        }
                        if ( dt.at( 4 ) != "" ) {
                            // minute
                            data.emplace( this->field2id.at("date_time_minute"), dt.at( 4 ) );
                        }
                        if ( dt.at( 5 ) != "" ) {
                            // second
                            data.emplace( this->field2id.at("date_time_second"), dt.at( 5 ) );
                        }


                    // process the request to get the protocol, method, resource and query
                    } else if ( fld == "request_full" ) {
                        size_t aux;
                        std::string aux_fld, protocol="", method="", page="", query="";
                        aux_fld = fld_str;
                        // method
                        aux = aux_fld.find( ' ' );
                        if ( aux != std::string::npos ) {
                            method  = aux_fld.substr( 0, aux );
                            aux_fld = StringOps::lstrip( aux_fld.substr( aux ) );

                            // page & query
                            aux = aux_fld.find( ' ' );
                            if ( aux != std::string::npos ) {
                                std::string aux_str = aux_fld.substr( 0, aux );
                                // search for the query
                                int aux_ = aux_str.find( '?' );
                                if ( aux_ != std::string::npos ) {
                                    page  = aux_str.substr( 0, aux_ );
                                    query = aux_str.substr( aux_+1 );
                                } else {
                                    // query not found
                                    page = aux_str;
                                }
                                aux_fld = StringOps::lstrip( aux_fld.substr( aux ) );

                                // protocol
                                protocol = aux_fld;
                            }
                        }
                        // append non-empty data
                        if ( protocol != "" ) {
                            data.emplace( this->field2id.at("request_protocol"), protocol );
                        }
                        if ( method != "" ) {
                            data.emplace( this->field2id.at("request_method"), method );
                        }
                        if ( page != "" ) {
                            data.emplace( this->field2id.at("request_uri"), page );
                        }
                        if ( query != "" ) {
                            data.emplace( this->field2id.at("request_query"), query );
                        }



                    // process the request to get uri and query
                    } else if ( fld == "request_uri_query" ) {
                        // search for the query
                        std::string page, query;
                        size_t aux_ = fld_str.find( '?' );
                        if ( aux_ != std::string::npos ) {
                            page  = fld_str.substr( 0, aux_ );
                            query = fld_str.substr( aux_+1 );
                        } else {
                            // query not found
                            page = fld_str;
                        }
                        if ( page != "" ) {
                            data.emplace( this->field2id.at("request_uri"), page );
                        }
                        if ( query != "" ) {
                            data.emplace( this->field2id.at("request_query"), query );
                        }



                    // process the time taken to convert to milliseconds
                    } else if ( StringOps::startsWith( fld, "time_taken_" ) ) {
                        float t = std::stof( fld_str );
                        fld = fld.substr( 11 );
                        if ( fld == "us" ) {
                            // from microseconds
                            t /= 1000;
                        } else if ( fld == "s" || fld == "s.ms" ) {
                            // from seconds
                            t *= 1000;
                        }
                        data.emplace( this->field2id.at("time_taken"), std::to_string( (int)t ) );


                    // something went wrong
                    } else {
                        // hmmm.. no...
                        throw LogParserException( "Unexpected LogField", fld );
                    }
                }
            }
        }


        // update the stop for the next start
        stop += sep.size();
        i++;
        if ( stop > line_size ) {
            // this was the final separator
            break;
        }
    }

    if ( add_pm ) {
        try {
            // add +12 hours for PM
            data.at( 4 ) = std::to_string( 12 + std::stoi(data.at( 4 )) );
        } catch (...) {
            // no hour data
        }
    }

    // set the default warning mark ( 0=false ) to default status
    data.emplace( 99, "0" );

    this->total_size += line_size;
    this->parsed_lines ++;

    return data;
}

void LogOps::parseLines( std::vector<std::unordered_map<int, std::string>>& data, const std::vector<std::string>& lines, const FormatOps::LogsFormat& format )
{
    data.clear();
    data.shrink_to_fit();
    int nl = format.new_lines;
    if ( nl == 0 ) {
        data.reserve( lines.size() );
        for ( const std::string& line : lines ) {
            data.push_back( this->parseLine( line, format ) );
        }
    } else {
        data.reserve( parsed_size / (nl+1) );
        parsed_size --;
        for ( int i=0; i<parsed_size; i++ ) {
            std::string line = lines.at( i );
            for ( int n=0; n<nl; n++ ) {
                i++;
                line += "\n" + lines.at( i );
            }
            data.push_back( this->parseLine( line, format ) );
        }
    }
    if ( data.size() < data.capacity() ) {
        data.shrink_to_fit();
    }
}


void LogOps::resetPerfData()
{
    this->total_size   = 0;
    this->parsed_size  = 0;
    this->parsed_lines = 0;
}
const unsigned LogOps::getTotalSize() const
{
    return this->total_size;
}
const unsigned LogOps::getParsedSize() const
{
    return this->parsed_size;
}
const unsigned LogOps::getParsedLines() const
{
    return this->parsed_lines;
}

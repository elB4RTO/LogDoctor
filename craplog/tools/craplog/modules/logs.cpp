
#include "logs.h"

#include "datetime.h"


LogOps::LogOps()
{
    // from fields to ids
    this->field2id["date_time_year"]    = 1;
    this->field2id["date_time_month"]   = 2;
    this->field2id["date_time_day"]     = 3;
    this->field2id["date_time_hour"]    = 4;
    this->field2id["date_time_minute"]  = 5;
    this->field2id["date_time_second"]  = 6;
        this->field2id["date_time_ncsa"]    = 0;
        this->field2id["date_time_utc"]     = 0;
        this->field2id["date_time_iso"]     = 0;
        this->field2id["date_time_mcs"]     = 0;
        this->field2id["date_time_iso_mcs"] = 0;

    this->field2id["request_protocol"]  = 10;
    this->field2id["request_method"]    = 11;
    this->field2id["request_page"]      = 12;
    this->field2id["request_query"]     = 13;
    this->field2id["response_code"]     = 14;
        this->field2id["request_full"]      = 0;

    this->field2id["time_taken_ms"]     = 15;
        this->field2id["time_taken_s"]      = 0;
        this->field2id["time_taken_s.ms"]   = 0;
    this->field2id["bytes_sent"]        = 16;
    this->field2id["bytes_received"]    = 17;

    this->field2id["referer"]           = 18;

    this->field2id["client"]            = 20;
        this->field2id["client:port"]       = 0;
    this->field2id["user_agent"]        = 21;
    this->field2id["cookie"]            = 22;

    this->field2id["port"]          = 30;
    this->field2id["error_level"]   = 31;
    this->field2id["error_message"] = 32;
    this->field2id["source_file"]   = 33;
}


LogOps::LogType LogOps::defineFileType( const std::string& name, const std::vector<std::string>& lines, std::unordered_map<int, FormatOps::LogsFormat>& formats )
{
    if ( lines.size() == 0 ) {
        // empty content
        return this->LogType::Failed;
    }

    int n_acc=0, n_err=0;
    bool maybe_acc, maybe_err;
    LogOps::LogType aux_type, real_type;
    const FormatOps::LogsFormat& current_ALF = formats.at( 1 ),
                                 current_ELF = formats.at( 2 );

    // real type assignment
    real_type = this->LogType::Failed;
    for ( const std::string& line : lines ) {
        // scan deeper
        maybe_acc = this->deepTypeCheck( line, current_ALF );
        maybe_err = this->deepTypeCheck( line, current_ELF );
        if ( maybe_acc == true && maybe_err == false ) {
            n_acc++; continue; }
        else if ( maybe_err == true && maybe_acc == false ) {
            n_err++; continue; }
        // if here, both was found, or none
        aux_type = this->comparativeTypeCheck( line, current_ALF, current_ELF );
        if ( aux_type == this->LogType::Access ) {
            n_acc ++;
        } else if ( aux_type == this->LogType::Error ) {
            n_err ++;
        } else {

        }
        // if here, it's probably not a valid file
    }

    // final decision
    if ( n_acc > 0 && n_err == 0 ) {
        // access logs
        real_type = LogOps::LogType::Access;
    } else if ( n_err > 0 && n_acc == 0 ) {
        // error logs
        real_type = LogOps::LogType::Error;
    } else {
        // something is wrong with these logs
    }
    return real_type;
}


bool LogOps::deepTypeCheck( const std::string& line, const FormatOps::LogsFormat& format )
{
    int n_sep_found = 0, n_blank_sep=0,
        found_at = 0, aux_found_at1 = 0, aux_found_at2 = 0,
        n_sep = format.separators.size(),
        line_size = line.size()-1;
    std::string sep, aux_sep1, aux_sep2;
    // check the initial part
    if ( format.initial.size() > 0 ) {
        if ( StringOps::startsWith( line, format.initial ) == true ) {
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
        while (true) {
            found_at = line.find( sep, aux_found_at2 );
            if ( found_at < 0 || found_at > line_size ) {
                // not found
                break;
            }
            if (found_at > 0 ) {
                if ( line.at( found_at-1 ) == '\\' ) {
                    // backslashed sep, probably not a sep
                    aux_found_at2 = found_at + sep.size();
                    continue;
                }
            }
            break;
        }
        if ( found_at < 0 || found_at > line_size ) {
            // not found
            continue;
        }

        if ( i+1 <= n_sep ) {
            // not the last separator, check the possibility of missing
            aux_sep1 = sep;
            aux_found_at1 = aux_sep1.find(' ');
            if ( aux_found_at1 >= 0 && aux_found_at1 < aux_sep1.size() ) {
                aux_sep1 = StringOps::lstripUntil( aux_sep1, " " );
            }
            // iterate over following separators
            for ( int j=i+1; j<n_sep; j++ ) {
                aux_sep2 = format.separators.at( j );
                aux_found_at2 = aux_sep2.find(' ');
                if ( aux_found_at2 > aux_sep2.size() || aux_found_at2 < 0 ) {
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
        if ( StringOps::endsWith( line, format.final ) == true ) {
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


LogOps::LogType LogOps::comparativeTypeCheck( const std::string& line, const FormatOps::LogsFormat& acc_f, const FormatOps::LogsFormat& err_f )
{
    LogOps::LogType type = this->LogType::Failed;

    int line_size = line.size(),
        n_acc_sep = acc_f.separators.size()-1,
        n_err_sep = err_f.separators.size()-1;

    std::string acc_sep, err_sep;

    struct Score {
        int score = 0;
        int n_sep_found = 0;
        int n_sep_blank = 0;
        bool sep_found = false;
        int start = 0;
        int stop = 0;
        void calc ( const std::string& line, const std::string& sep, const int posix )
        {
            this->start = this->stop;

            if ( sep == "" ) {
                this->sep_found = true;
                this->n_sep_blank ++;
                this->n_sep_found ++;
                this->score += 20;
            } else {
                int aux;
                switch (posix) {
                    // initial
                    case 0:
                        if ( StringOps::startsWith( line, sep ) == true ) {
                            this->sep_found = true;
                            this->n_sep_found ++;
                            this->score += 50;
                        }
                        break;
                    // separator
                    case 1:
                        aux = line.find( sep, this->start );
                        if ( aux >= 0 && aux <= line.size() ) {
                            // found
                            this->sep_found = true;
                            this->n_sep_found ++;
                            this->start = aux;
                            this->stop  = aux + sep.size();
                            this->score += 50;
                        }
                        break;
                    // final
                    case 2:
                        if ( StringOps::endsWith( line, sep ) == true ) {
                            this->sep_found = true;
                            this->n_sep_found ++;
                            this->score += 50;
                        }
                        break;
                }
            }
        }
    } acc, err;


    acc_sep = acc_f.initial;
    err_sep = err_f.initial;

    acc.sep_found = false;
    err.sep_found = false;

    acc.calc( line, acc_sep, 0 );
    err.calc( line, err_sep, 0 );

    if ( acc_sep == err_sep ) {
        // separators are identical
        if ( acc.sep_found == true && err.sep_found == false ) {
            acc.score += 80;
        } else if ( err.sep_found == true && acc.sep_found == false ) {
            err.score += 80;
        }
    }


    int i = 0;
    bool skip_acc, skip_err;
    while (true) {
        if ( i > n_acc_sep && i > n_err_sep ) {
            // no more separators to try
            break;
        }
        acc.sep_found = false;
        err.sep_found = false;

        if ( i <= n_acc_sep
          && acc.start < line_size ) {
            skip_acc = false;
            acc_sep = acc_f.separators.at( i );
            acc.calc( line, acc_sep, 1 );
        } else {
            skip_acc = true;
            acc_sep.clear();
        }

        if ( i <= n_err_sep
          && err.start < line_size ) {
            skip_err = false;
            err_sep = err_f.separators.at( i );
            err.calc( line, err_sep, 1 );
        } else {
            skip_err = true;
            err_sep.clear();
        }

        if ( skip_acc == false && skip_err == false && acc_sep == err_sep ) {
            // separators are identical
            if ( acc.sep_found == true && err.sep_found == false ) {
                acc.score += 80;
            } else if ( err.sep_found == true && acc.sep_found == false ) {
                err.score += 80;
            }
        }
        i++;
    }


    acc_sep = acc_f.final;
    err_sep = err_f.final;

    acc.sep_found = false;
    err.sep_found = false;

    acc.calc( line, acc_sep, 2 );
    err.calc( line, err_sep, 2 );

    if ( acc_sep == err_sep ) {
        // separators are identical
        if ( acc.sep_found == true && err.sep_found == false ) {
            acc.score += 80;
        } else if ( err.sep_found == true && acc.sep_found == false ) {
            err.score += 80;
        }
    }


    // final points, remove points for every missing sep
    acc.score -= 35 * ( n_acc_sep + 2 - acc.n_sep_found );
    acc.score -= 5 * ( acc.n_sep_found - acc.n_sep_blank );
    err.score -= 35 * ( n_err_sep + 2 - err.n_sep_found );
    err.score -= 5 * ( err.n_sep_found - err.n_sep_blank );


    if ( acc.score > err.score ) {
        type = this->LogType::Access;

    } else if ( err.score > acc.score ) {
        type = this->LogType::Error;

    } else {
        // something is wrong with these logs
    }

    return type;
}




std::unordered_map<int, std::string> LogOps::parseLine( const std::string& line, const FormatOps::LogsFormat& format )
{
    std::unordered_map<int, std::string> data;
    std::string sep, fld, fld_str, aux_fld_str, aux_sep1, aux_sep2;
    bool missing=false, add_pm=false;
    int start, stop=0, i=0, aux_start1, aux_start2, aux_stop;
    int line_size;
    int n_sep = format.separators.size()-1;
    line_size = line.size()-1;

    // add the initial chars
    stop = format.initial.size();

    while (true) {
        // split fields
        start = stop; // stop updated at the end of this loop
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
                if ( stop > line_size+1 || stop < 0 ) {
                    stop = line_size +1;
                }
            }
        } else {
            // no more separators
            break;
        }
        if ( stop > line_size+1 || stop < 0 ) {
            // separator not found, skip to the next one
            i++;
            stop = start;
            continue;
        }

        if ( i+1 <= n_sep ) {

            // tricky error messages from apache
            aux_fld_str = StringOps::strip( line.substr(start, stop-start), " " );
            if ( StringOps::startsWith( aux_fld_str, "AH0"  )
              || StringOps::startsWith( aux_fld_str, "PHP " ) ) {
                if ( format.fields.at( i ) != "error_message" ) {
                    // this field is missing, step to the right field
                    for ( int j=i+1; j<format.fields.size(); j++ ) {
                        if ( format.fields.at( j ) == "error_message" ) {
                            i = j;
                            break;
                        }
                    }
                }
                // correct field, check if the next separators has columns. in case, update the stop position
                int c_count = StringOps::count( aux_fld_str, ": " ),
                    c_max = 2;
                if ( StringOps::startsWith( aux_fld_str, "AH00171" )
                  || StringOps::startsWith( aux_fld_str, "AH00163" ) ) {
                    c_max = 1;
                }
                if ( c_count < c_max ) {
                    // supposed to contain at least 2 columns
                    aux_stop = stop;
                    int j;
                    for ( j=0; j<c_count; j++ ) {
                        aux_stop = line.find( ": ", aux_stop );
                        if ( aux_stop != 0 || aux_stop >= line_size ) {
                            // column not not found
                            break;
                        } else {
                            // one_setp_further
                            aux_stop ++;
                        }
                    }
                    // finally update the stop to the next sep
                    if ( i <= n_sep ) {
                        sep = format.separators.at( i );
                        stop = line.find( sep, aux_stop );
                    } else if ( i == n_sep+1 ) {
                        // final separator
                        sep = format.final;
                        if ( sep == "" ) {
                            stop = line_size+1;
                        } else {
                            stop = line.find( sep, aux_stop );
                            if ( stop > line_size+1 || stop < 0 ) {
                                stop = line_size +1;
                            }
                        }
                    }
                }

            } else {

                // not the last separator, check the possibility of missing
                aux_sep1 = sep;
                aux_start1 = aux_sep1.find(' ');
                if ( aux_start1 >= 0 && aux_start1 < aux_sep1.size() ) {
                    aux_sep1 = StringOps::lstripUntil( aux_sep1, " " );
                }
                // iterate over following separators
                for ( int j=i+1; j<n_sep; j++ ) {
                    aux_sep2 = format.separators.at( j );
                    aux_start2 = aux_sep2.find(' ');
                    if ( aux_start2 > aux_sep2.size() || aux_start2 < 0 ) {
                        aux_start2 = stop;
                    } else {
                        aux_start2 = stop + aux_start2 + 1;
                        aux_sep2 = StringOps::lstripUntil( aux_sep2, " " );
                    }
                    // if the 2 seps are identical, skip (for uncertainty)
                    if ( aux_sep1 == aux_sep2 || aux_sep2 == "" ) {
                        continue;
                    }
                    // check if the next sep is found in the same position of the current one
                    if ( line.find( aux_sep2, stop ) == aux_start2 ) {
                        // probably the current field is missing, skip to this one
                        i = j;
                        aux_stop = aux_start2 + aux_sep2.size();
                        aux_sep2 = format.separators.at( j );
                        missing = true;
                    }
                    break;
                }
            }
        }

        // get the field
        fld = format.fields.at( i );
        if ( fld != "NONE" ) {
            // only parse the considered fields
            fld_str = StringOps::strip( line.substr(start, stop-start), " " );
            this->size += fld_str.size();

            if ( fld_str != "" ) {
                int fld_id = this->field2id.at(fld);
                if ( fld_id > 0 ) {
                    // no need to process, append directly if non-empty
                    data.emplace( fld_id, fld_str );

                } else {
                    // process the field

                    // process the date to get year, month, day, hour and minute
                    if ( StringOps::startsWith( fld, "date_time" ) ) {
                        std::vector<std::string> dt = DateTimeOps::processDateTime( fld_str, fld.substr( 10 ) ); // cut away the "date_time_" part which is useless from now on
                        if ( dt[0] != "" ) {
                            // year
                            data.emplace( this->field2id.at("date_time_year"), dt[0] );
                        }
                        if ( dt[1] != "" ) {
                            // month
                            data.emplace( this->field2id.at("date_time_month"), dt[1] );
                        }
                        if ( dt[2] != "" ) {
                            // day
                            data.emplace( this->field2id.at("date_time_day"), dt[2] );
                        }
                        if ( dt[3] != "" ) {
                            // hour
                            if ( dt[3] == "PM" ) {
                                add_pm = true;
                            } else {
                                data.emplace( this->field2id.at("date_time_hour"), dt[3] );
                            }
                        }
                        if ( dt[4] != "" ) {
                            // minute
                            data.emplace( this->field2id.at("date_time_minute"), dt[4] );
                        }
                        if ( dt[5] != "" ) {
                            // second
                            data.emplace( this->field2id.at("date_time_second"), dt[5] );
                        }


                    // process the request to get the protocol, method, resource and query
                    } else if ( fld == "request_full" ) {
                        int aux, fld_size=fld_str.size()-1;
                        std::string aux_fld, protocol="", method="", page="", query="";
                        aux_fld = fld_str;
                        // method
                        aux = aux_fld.find( ' ' );
                        if ( aux >= 0 && aux <= fld_size ) {
                            method = aux_fld.substr( 0, aux );
                            aux_fld = StringOps::lstrip( aux_fld.substr( aux ) );

                            // page & query
                            aux = aux_fld.find( ' ' );
                            if ( aux >= 0 && aux <= fld_size ) {
                                std::string aux_str = aux_fld.substr( 0, aux );
                                // search for the query
                                int aux_ = aux_str.find( '?' );
                                if ( aux_ >= 0 && aux_ <= aux_str.size() ) {
                                    page = aux_str.substr( 0, aux_ );
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
                            data.emplace( this->field2id.at("request_page"), page );
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


                    // split client:port
                    } else if ( fld == "client:port" ) {
                        int aux = fld_str.find( ':' );
                        if ( aux >= 0 && aux < fld_str.size() ) {
                            data.emplace( this->field2id.at("client"), fld_str.substr( 0, aux ) );
                            data.emplace( this->field2id.at("port"), fld_str.substr( aux+1 ) );
                        }


                    // something went wrong
                    } else {
                        // hmmm.. no...
                    }
                }
            }
        }


        // update the stop for the next start
        if ( missing == true ) {
            missing = false;
            stop = aux_stop;
        } else {
            stop = stop + sep.size();
            i++;
        }
        if ( stop > line_size ) {
            // this was the final separator
            break;
        }
    }

    if ( add_pm == true ) {
        try {
            // add +12 hours for PM
            data[4] = std::to_string( 12 + std::stoi(data[4]) );
        } catch (...) {
            // no hour data
        }
    }

    // set the default warning mark ( 0=false )
    data.emplace( 99, "0" );

    this->lines ++;

    return data;
}

std::vector<std::unordered_map<int, std::string>> LogOps::parseLines( const std::vector<std::string>& lines, const FormatOps::LogsFormat& format )
{
    std::vector<std::unordered_map<int, std::string>> collection;
    collection.reserve( lines.size() );
    for ( const std::string& line : lines ) {
        collection.push_back( this->parseLine( line, format ) );
    }

    return collection;
}


void LogOps::resetPerfData()
{
    this->size  = 0;
    this->lines = 0;
}
int LogOps::getSize()
{
    return this->size;
}
int LogOps::getLines()
{
    return this->lines;
}


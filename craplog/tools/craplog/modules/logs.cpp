
#include "logs.h"


LogOps::LogOps()
{
    // from fields to ids
    this->field2id["date_time"] = 0;
    this->field2id["date_time_utc"] = 0;
    this->field2id["date_time_ct"] = 0;
    this->field2id["date_time_mcs"] = 0;
    this->field2id["date_time_iso_mcs"] = 0;

    this->field2id["request_full"] = 0;
    this->field2id["request_method"] = 10;
    this->field2id["request_page"] = 11;
    this->field2id["request_query"] = 12;
    this->field2id["response_code"] = 13;
    this->field2id["time_taken"] = 0;
    this->field2id["time_taken+ms"] = 0;
    this->field2id["time_taken_ms"] = 14;
    this->field2id["bytes_sent"] = 15;
    this->field2id["bytes_received"] = 16;
    this->field2id["referer"] = 17;

    this->field2id["client:port"] = 0;
    this->field2id["client"] = 20;
    this->field2id["user_agent"] = 21;
    this->field2id["cookie"] = 22;

    this->field2id["port"] = 30;
    this->field2id["error_level"] = 31;
    this->field2id["error_message"] = 32;
    this->field2id["source_file"] = 33;
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




std::unordered_map<int, std::string> LogOps::parseLine( const std::string& line, const FormatOps::LogsFormat& format ) const
{
    std::unordered_map<int, std::string> data;

    std::string sep, fld, fld_str, aux_sep1, aux_sep2;
    bool missing=false;
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
            stop = line_size;
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
                if ( line.find( aux_sep2, aux_start2 ) == aux_start2 ) {
                    // probably the current field is missing, skip to this one
                    i = j;
                    aux_stop = aux_start2 + aux_sep2.size();
                    aux_sep2 = format.separators.at( j );
                    missing = true;
                }
                break;
            }
        }

        // get the field
        fld = format.fields.at( i );
        fld_str = line.substr(start, stop-start);

        // process the field if needed
        if ( StringOps::startsWith( fld, "date_time" ) ) {
            // special case for date in NCSA format being enclosed between square brackets
            if ( StringOps::startsWith( fld_str, "[" ) ) {
                fld_str = fld_str.substr( 1, fld_str.size()-1 );
                if ( StringOps::endsWith( fld_str, "]" ) ) {
                    fld_str = fld_str.substr( 0, fld_str.size()-1 );
                    sep = "]" + sep;
                }
            }
            // process the date to get year, month, day, hour and minute
            fld = fld.substr( 10 ); // cut away the "date_time_" part which is useless from now on
            if ( fld == "" ) {

            } else if ( fld == "" ) {

            } else if ( fld == "" ) {

            } else if ( fld == "" ) {

            } else if ( fld == "" ) {

            } else if ( fld == "" ) {

            }

        } else if ( fld == "user_agent" || fld == "source_file" ) {
            //class_name += colors["ua_src"];

        } else if ( StringOps::startsWith( fld, "request" ) || fld == "error_message" ) {
            //class_name += colors["req_err"];

        } else if ( fld == "response_code" || fld == "error_level" ) {
            //class_name += colors["res_lev"];

        } else {
            //class_name += colors["x"];
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

    return data;
}

std::vector<std::unordered_map<int, std::string>> LogOps::parseLines( const std::vector<std::string>& lines, const FormatOps::LogsFormat& format ) const
{
    std::vector<std::unordered_map<int, std::string>> collection;
    collection.reserve( lines.size() );
    for ( const std::string& line : lines ) {
        collection.push_back( this->parseLine( line, format ) );
    }

    return collection;
}


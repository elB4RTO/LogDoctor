
#include "logs.h"

using std::string, std::vector, std::unordered_map;


LogOps::LogOps()
{

}


LogOps::LogType LogOps::defineFileType( const string& name, const vector<string>& lines, unordered_map<int, FormatOps::LogsFormat>& formats )
{
    if ( lines.size() == 0 ) {
        // empty content
        return this->LogType::Failed;
    }

    int n_acc=0, n_err=0;
    bool maybe_acc, maybe_err;
    LogOps::LogType supposed_type, real_type;
    const FormatOps::LogsFormat& current_ALF = formats[1],
                                 current_ELF = formats[2];

    // preliminary type assignment
    if ( StringOps::startsWith( name, "access" ) ) {
        supposed_type = this->LogType::Access;
    } else if ( StringOps::startsWith( name, "error" ) ) {
        supposed_type = this->LogType::Error;
    } else {
        // hopefully not entering here
        supposed_type = this->LogType::Failed;
    }

    // real type assignment
    real_type = this->LogType::Failed;
    for ( const string& line : lines ) {

        if ( supposed_type == this->LogType::Access ) {
            // check access logs initial/final chars first
            if ( current_ALF.initial.size() > 0 ) {
                if ( StringOps::startsWith( line, current_ALF.initial ) == true ) {
                    n_acc++; continue; }}
            if ( current_ALF.final.size() > 0 ) {
                if ( StringOps::endsWith( line, current_ALF.final ) == true ) {
                    n_acc++; continue; }}
            // none found, check error logs ones
            if ( current_ELF.initial.size() > 0 ) {
                if ( StringOps::startsWith( line, current_ELF.initial ) == true ) {
                    n_err++; continue; }}
            if ( current_ELF.final.size() > 0 ) {
                if ( StringOps::endsWith( line, current_ELF.final ) == true ) {
                    n_err++; continue; }}
            // yet none found, scan deeper
            maybe_acc = this->deepTypeCheck( line, current_ALF );
            maybe_err = this->deepTypeCheck( line, current_ELF );
            if ( maybe_acc == true && maybe_err == false ) {
                n_acc++; continue; }
            else if ( maybe_err == true && maybe_acc == false ) {
                n_err++; continue; }
            // probably not a valid file

        } else if ( supposed_type == this->LogType::Error ) {
            // check error logs initial/final chars first
            if ( current_ELF.initial.size() > 0 ) {
                if ( StringOps::startsWith( line, current_ELF.initial ) == true ) {
                    n_err++; continue; }}
            if ( current_ELF.final.size() > 0 ) {
                if ( StringOps::endsWith( line, current_ELF.final ) == true ) {
                    n_err++; continue; }}
            // none found, check access logs ones
            if ( current_ALF.initial.size() > 0 ) {
                if ( StringOps::startsWith( line, current_ALF.initial ) == true ) {
                    n_acc++; continue; }}
            if ( current_ALF.final.size() > 0 ) {
                if ( StringOps::endsWith( line, current_ALF.final ) == true ) {
                    n_acc++; continue; }}
            // yet none found, scan deeper
            maybe_err = this->deepTypeCheck( line, current_ELF );
            maybe_acc = this->deepTypeCheck( line, current_ALF );
            if ( maybe_err == true && maybe_acc == false ) {
                n_acc++; continue; }
            else if ( maybe_acc == true && maybe_err == false ) {
                n_err++; continue; }
            // probably not a valid file

        } else {
            // scan deeper
            maybe_acc = this->deepTypeCheck( line, current_ALF );
            maybe_err = this->deepTypeCheck( line, current_ELF );
            if ( maybe_acc == true && maybe_err == false ) {
                n_acc++; continue; }
            else if ( maybe_err == true && maybe_acc == false ) {
                n_err++; continue; }
            // if here, it's probably not a valid file
        }

    }

    // final decision
    if ( n_acc > 0 && n_err == 0 ) {
        // access logs
        real_type = LogOps::LogType::Access;
    } else if ( n_err > 0 && n_acc == 0 ) {
        // error logs
        real_type = LogOps::LogType::Error;
    }/* else {
        // something is wrong with these logs
    }*/
    return real_type;
}


bool LogOps::deepTypeCheck( const string& line, const FormatOps::LogsFormat& format )
{
    int n_sep_found = 0, n_blank_sep=0,
        found_at = 0, aux_found_at1 = 0, aux_found_at2 = 0,
        n_sep = format.separators.size(),
        line_size = line.size();
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
        sep = format.separators[i];
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
                if ( line[ found_at-1 ] == '\\' ) {
                    // backslashed sep, probably not a sep
                    aux_found_at2 = found_at + sep.size();
                }
            } else {
                break;
            }
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
                aux_sep2 = format.separators[j];
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
                    sep = format.separators[j];
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
    if ( n_sep_found >= n_sep/2
      && n_blank_sep <= n_sep_found/2  ) {
        result = true;
    }

    return result;
}




vector<string> LogOps::splitLine( const string& line, const FormatOps::LogsFormat& format )
{

}

vector<string> LogOps::splitLines( const string& line, const FormatOps::LogsFormat& format )
{

}


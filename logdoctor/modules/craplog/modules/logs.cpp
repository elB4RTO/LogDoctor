
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

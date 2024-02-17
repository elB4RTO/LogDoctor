
#include "logs.h"

#include "modules/exceptions.h"
#include "modules/craplog/modules/datetime.h"

#include "utilities/strings.h"


namespace LogOps
{

namespace /*private*/
{

//! Parse the given line using the given format
/*!
    \param line The log line to check
    \param format The logs format to use
    \return Whether the line respects the format or not
    \see defineFileType(), FormatOps::LogsFormat
*/
bool deepTypeCheck( const std::string& line, const LogsFormat& format ) noexcept
{
    size_t n_sep{ format.separators.size() },
           n_sep_found{0}, n_blank_sep{0},
           found_at, aux_found_at1{0}, aux_found_at2;
    std::string sep, aux_sep1, aux_sep2;
    // check the initial part
    if ( ! format.initial.empty() ) {
        if ( StringOps::startsWith( line, format.initial ) ) {
            ++ n_sep_found;
        }
    } else {
        ++ n_sep_found;
        ++ n_blank_sep;
    }
    // check the middle part
    for ( size_t i{0}; i<n_sep; ++i ) {
        sep = format.separators.at( i );
        if ( sep.empty() ) {
            ++ n_sep_found;
            ++ n_blank_sep;
            continue;
        }
        aux_found_at2 = aux_found_at1;
        found_at = line.find( sep, aux_found_at2 );
        if ( found_at == std::string::npos ) {
            // not found
            continue;
        }

        if ( i+1ul <= n_sep ) {
            // not the last separator, check the possibility of missing
            aux_sep1 = sep;
            aux_found_at1 = aux_sep1.find(' ');
            if ( aux_found_at1 != std::string::npos ) {
                aux_sep1 = StringOps::lstripUntil( aux_sep1, ' ' );
            }
            // iterate over following separators
            for ( size_t j{i+1ul}; j<n_sep; ++j ) {
                aux_sep2 = format.separators.at( j );
                aux_found_at2 = aux_sep2.find(' ');
                if ( aux_found_at2 == std::string::npos ) {
                    aux_found_at2 = found_at;
                } else {
                    aux_found_at2 = found_at + aux_found_at2 + 1;
                    aux_sep2 = StringOps::lstripUntil( aux_sep2, ' ' );
                }
                // if the 2 seps are identical, skip (for uncertainty)
                if ( aux_sep1 == aux_sep2 || aux_sep2.empty() ) {
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
        ++ n_sep_found;

    }

    // check the final part
    if ( ! format.final.empty() ) {
        if ( StringOps::endsWith( line, format.final ) ) {
            ++ n_sep_found;
        }
    } else {
        ++ n_sep_found;
        ++ n_blank_sep;
    }

    // add the initial and final seps now
    n_sep += 2;

    // the line is considered valid if all the seps have been found
    // and more than a half of them were not blank
    return n_sep_found >= n_sep-1
        && n_blank_sep <= n_sep_found/2;
}

} // namespace (private)


LogType defineFileType( const std::vector<std::string>& lines, const LogsFormat& format ) noexcept
{
    if ( lines.empty() ) {
        // empty file, already handled by craplog, should be unreachable
        return LogType::Failed;
    }

    // real type assignment
    int n_access{0}, n_other{0};
    for ( const std::string& line : lines ) {
        // scan the given lines
        if ( deepTypeCheck( line, format ) ) {
            ++ n_access;
        } else {
            ++ n_other;
        }
    }

    // final decision
    if ( n_access > 0 && n_other == 0 ) {
        // Access: valid logs (for the currently set LogsFormat)
        return LogType::Access;
    } else if ( n_other > 0 && n_access == 0 ) {
        // Discarded: other format, or maybe error logs
        return LogType::Discarded;
    } else {
        // Failed: something is wrong with this file
        return LogType::Failed;
    }
}

} // namespace LogOps


#include "datetime.h"

#include "modules/exceptions.h"
#include "utilities/strings.h"

#include <QDateTime>

#include <ctime>


namespace DateTimeOps
{

namespace /*private*/
{

//! Converts a month from the short-name to the number
/*!
    \param month The short-name of the month
    \return The month number in the calendar
    \throw DateTimeException
    \see processDateTime
*/
const std::string convertMonth( std::string_view month )
{
    if ( month == "Jan" ) {
        return "01";
    } else if ( month == "Feb" ) {
        return "02";
    } else if ( month == "Mar" ) {
        return "03";
    } else if ( month == "Apr" ) {
        return "04";
    } else if ( month == "May" ) {
        return "05";
    } else if ( month == "Jun" ) {
        return "06";
    } else if ( month == "Jul" ) {
        return "07";
    } else if ( month == "Aug" ) {
        return "08";
    } else if ( month == "Sep" ) {
        return "09";
    } else if ( month == "Oct" ) {
        return "10";
    } else if ( month == "Nov" ) {
        return "11";
    } else if ( month == "Dec" ) {
        return "12";
    } else {
        // nope
        throw DateTimeException("Unexpected month format: "+std::string{month});
    }
}

} // namespace (private)


const std::vector<std::string> processDateTime( std::string_view datetime_, std::string_view format )
{
    std::string aux, datetime{datetime_};
    std::string year{""}, month{""}, day{""}, hour{""}, minute{""}, second{""};

    if ( format == "ncsa" ) {
        datetime = StringOps::strip( datetime, "[ ]" );
        day    = datetime.substr( 0, 2 );
        month  = convertMonth( datetime.substr( 3, 3 ) );
        year   = datetime.substr( 7, 4 );
        hour   = datetime.substr( 12, 2 );
        minute = datetime.substr( 15, 2 );
        second = datetime.substr( 18, 2 );

    } else if ( format == "mcs" ) {
        month  = convertMonth( datetime.substr( 4, 3 ) );
        day    = datetime.substr( 8, 2 );
        hour   = datetime.substr( 11, 2 );
        minute = datetime.substr( 14, 2 );
        second = datetime.substr( 17, 2 );
        year   = datetime.substr( datetime.size()-4 );

    } else if ( format == "gmt" ) {
        int start = datetime.find( ", " ) + 2;
        day    = datetime.substr( start, 2 );
        start += 3;
        month  = convertMonth( datetime.substr( start, 3 ) );
        start += 4;
        year   = datetime.substr( start, 4 );
        start += 5;
        hour   = datetime.substr( start, 2 );
        start += 3;
        minute = datetime.substr( start, 2 );
        start += 3;
        second = datetime.substr( start, 2 );

    } else if ( StringOps::startsWith( format, "iso" ) ) {
        year   = datetime.substr( 0, 4 );
        month  = datetime.substr( 5, 2 );
        day    = datetime.substr( 8, 2 );
        hour   = datetime.substr( 11, 2 );
        minute = datetime.substr( 14, 2 );
        second = datetime.substr( 17, 2 );

    } else if ( StringOps::startsWith( format, "utc" ) ) {
        if ( format == "utc_d" ) {
            // date
            year   = datetime.substr( 0, 4 );
            month  = datetime.substr( 5, 2 );
            day    = datetime.substr( 8, 2 );
        } else {
            // time
            hour   = datetime.substr( 0, 2 );
            minute = datetime.substr( 3, 2 );
            second = datetime.substr( 6, 2 );
        }

    } else if ( StringOps::startsWith( format, "epoch_" ) ) {
        aux = format.substr( 6 );
        // convert to seconds
        if ( aux == "us" ) {
            // from microseconds
            datetime.resize( datetime.size()-6 );
        } else if ( aux == "ms" ) {
            // from milliseconds
            datetime.resize( datetime.size()-3 );
        } else if ( aux == "s.ms" ) {
            // from seconds.milliseconds
            datetime = std::to_string( std::stoi( datetime ) );
        }
        // convert to iso date format
        const QDateTime e{ QDateTime::fromSecsSinceEpoch( std::stoi( datetime ) ) };
        datetime = e.toString( "yyyy-MM-dd HH:mm:ss" ).toStdString();

        // parse
        year   = datetime.substr( 0, 4 );
        month  = datetime.substr( 5, 2 );
        day    = datetime.substr( 8, 2 );
        hour   = datetime.substr( 11, 2 );
        minute = datetime.substr( 14, 2 );
        second = datetime.substr( 17, 2 );

    } else {
        if ( format == "YYYYMMDD" ) {
            year   = datetime.substr( 0, 4 );
            month  = datetime.substr( 5, 2 );
            day    = datetime.substr( 8, 2 );

        } else if ( format == "MMDDYY" ) {
            const int y = std::stoi( datetime.substr( 6, 2 ) );
            month  = datetime.substr( 0, 2 );
            day    = datetime.substr( 3, 2 );
            year   = (y<70) ? "20" : "19";
            year  += (y<10) ? "0"+std::to_string( y ) : std::to_string( y );

        } else if ( format == "MDYYYY" ) {
            size_t aux_;
            if ( datetime.at(2) == '/' ) {
                month = datetime.substr( 0, 2 );
                aux_ = 3;
            } else {
                month = "0" + datetime.substr( 0, 1 );
                aux_ = 2;
            }
            if ( datetime.at(aux_+2) == '/' ) {
                day = datetime.substr( aux_, 2 );
                aux_ += 3;
            } else {
                day = "0" + datetime.substr( aux_, 1 );
                aux_ += 2;
            }
            year = datetime.substr( aux_ );

        } else if ( StringOps::startsWith( format, "year" ) ) {
            year = datetime;
            if ( format == "year_short" ) {
                int y = std::stoi( year );
                year  = (y<70) ? "20" : "19";
                year += year;
            }

        } else if ( StringOps::startsWith( format, "month" ) ) {
            if ( format.size() <= 5 ) {
                month = datetime;
            } else {
                datetime.resize( 3 ); // may be the full name
                month = convertMonth( datetime );
            }

        } else if ( format == "day" ) {
            day = datetime;

        } else if ( StringOps::startsWith( format, "clock_" ) ) {
            aux = format.substr( 6 );
            if ( aux == "24" ) {
                hour   = datetime.substr( 0, 2 );
                minute = datetime.substr( 3, 2 );
                second = datetime.substr( 6, 2 );

            } else if ( aux == "12" ) {
                hour   = datetime.substr( 0, 2 );
                minute = datetime.substr( 3, 2 );
                second = datetime.substr( 6, 2 );
                if ( datetime.substr( 9, 2 ) == "pm" ) {
                    hour = std::to_string( 12 + std::stoi(hour) );
                }

            } else if ( aux == "short" ) {
                hour   = datetime.substr( 0, 2 );
                minute = datetime.substr( 3, 2 );

            } else if ( aux == "meridian" ) {
                if ( datetime == "pm" ) {
                    hour = "PM"; // to mark for final update
                }
            }

        } else if ( format == "hour" ) {
            hour = datetime;

        } else if ( format == "minute" ) {
            minute = datetime;

        } else if ( format == "second" ) {
            second = datetime;

        } else {
            // wronthing went some ...
            throw DateTimeException("Unexpected DateTime format: "+std::string{datetime_});
        }
    }

    return std::vector<std::string>{ year, month, day, hour, minute, second };
}

} // namespace DateTimeOps

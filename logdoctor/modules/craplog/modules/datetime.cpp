
#include "datetime.h"

#include "lib.h"

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
        throw DateTimeException("Unexpected month format", std::string{month});
    }
}

} // namespace (private)


std::vector<std::string> processDateTime( std::string_view datetime_, const LogsFormatField format )
{
    std::string datetime{datetime_};
    std::string year, month, day, hour, minute, second;

    if ( format == date_time_ncsa ) {
        datetime = StringOps::strip( datetime, "[ ]" );
        day    = datetime.substr( 0ul, 2ul );
        month  = convertMonth( datetime.substr( 3ul, 3ul ) );
        year   = datetime.substr( 7ul,  4ul );
        hour   = datetime.substr( 12ul, 2ul );
        minute = datetime.substr( 15ul, 2ul );
        second = datetime.substr( 18ul, 2ul );

    } else if ( format == date_time_mcs ) {
        month  = convertMonth( datetime.substr( 4ul, 3ul ) );
        day    = datetime.substr( 8ul,  2ul );
        hour   = datetime.substr( 11ul, 2ul );
        minute = datetime.substr( 14ul, 2ul );
        second = datetime.substr( 17ul, 2ul );
        year   = datetime.substr( datetime.size()-4 );

    } else if ( format == date_time_gmt ) {
        size_t start{ datetime.find( ", " ) + 2ul };
        day    = datetime.substr( start, 2ul );
        start += 3ul;
        month  = convertMonth( datetime.substr( start, 3ul ) );
        start += 4ul;
        year   = datetime.substr( start, 4ul );
        start += 5ul;
        hour   = datetime.substr( start, 2ul );
        start += 3ul;
        minute = datetime.substr( start, 2ul );
        start += 3ul;
        second = datetime.substr( start, 2ul );

    } else if ( format == date_time_iso ) {
        year   = datetime.substr( 0ul,  4ul );
        month  = datetime.substr( 5ul,  2ul );
        day    = datetime.substr( 8ul,  2ul );
        hour   = datetime.substr( 11ul, 2ul );
        minute = datetime.substr( 14ul, 2ul );
        second = datetime.substr( 17ul, 2ul );

    } else if ( _DATE_TIME_UTC & format ) {
        if ( format == date_time_utc_d ) {
            // date
            year   = datetime.substr( 0ul, 4ul );
            month  = datetime.substr( 5ul, 2ul );
            day    = datetime.substr( 8ul, 2ul );
        } else if ( format == date_time_utc_t ) {
            // time
            hour   = datetime.substr( 0ul, 2ul );
            minute = datetime.substr( 3ul, 2ul );
            second = datetime.substr( 6ul, 2ul );
        } else [[unlikely]] {
            // wronthing went some ...
            throw DateTimeException("Unexpected DateTime UTC", std::string{datetime_}+" - format: "+std::to_string(format));
        }

    } else if ( _DATE_TIME_EPOCH & format ) {
        // convert to seconds
        if ( format == date_time_epoch_us ) {
            // from microseconds
            datetime.resize( datetime.size()-6ul );
        } else if ( format == date_time_epoch_ms ) {
            // from milliseconds
            datetime.resize( datetime.size()-3ul );
        } else if ( format == date_time_epoch_s_ms ) {
            // from seconds.milliseconds
            datetime = std::to_string( std::stoi( datetime ) );
        } else if ( format != date_time_epoch_s ) [[unlikely]] {
            // wronthing went some ...
            throw DateTimeException("Unexpected DateTime EPOCH", std::string{datetime_}+" - format: "+std::to_string(format));
        }
        // convert to iso date format
        const QDateTime e{ QDateTime::fromSecsSinceEpoch( std::stoi( datetime ) ) };
        datetime = e.toString( "yyyy-MM-dd HH:mm:ss" ).toStdString();

        // parse
        year   = datetime.substr( 0ul,  4ul );
        month  = datetime.substr( 5ul,  2ul );
        day    = datetime.substr( 8ul,  2ul );
        hour   = datetime.substr( 11ul, 2ul );
        minute = datetime.substr( 14ul, 2ul );
        second = datetime.substr( 17ul, 2ul );

    } else if ( _DATE_TIME_DATE & format ) {
        if ( format == date_time_yyyymmdd ) {
            year   = datetime.substr( 0ul, 4ul );
            month  = datetime.substr( 5ul, 2ul );
            day    = datetime.substr( 8ul, 2ul );

        } else if ( format == date_time_mmddyy ) {
            const int y{ std::stoi( datetime.substr( 6ul, 2ul ) ) };
            month  = datetime.substr( 0ul, 2ul );
            day    = datetime.substr( 3ul, 2ul );
            year   = (y<70) ? "20" : "19";
            year  += (y<10) ? "0"+std::to_string( y ) : std::to_string( y );

        } else if ( format == date_time_mdyyyy ) {
            size_t aux_;
            if ( datetime.at(2) == '/' ) {
                month = datetime.substr( 0ul, 2ul );
                aux_ = 3ul;
            } else {
                month = "0" + datetime.substr( 0ul, 1ul );
                aux_ = 2ul;
            }
            if ( datetime.at(aux_+2ul) == '/' ) {
                day = datetime.substr( aux_, 2ul );
                aux_ += 3ul;
            } else {
                day = "0" + datetime.substr( aux_, 1ul );
                aux_ += 2ul;
            }
            year = datetime.substr( aux_ );

        } else [[unlikely]] {
            // wronthing went some ...
            throw DateTimeException("Unexpected DateTime DATE", std::string{datetime_}+" - format: "+std::to_string(format));
        }

    } else if ( _DATE_TIME_CLOCK & format ) {
        if ( format == date_time_clock_24 ) {
            hour   = datetime.substr( 0ul, 2ul );
            minute = datetime.substr( 3ul, 2ul );
            second = datetime.substr( 6ul, 2ul );

        } else if ( format == date_time_clock_12 ) {
            hour   = datetime.substr( 0ul, 2ul );
            minute = datetime.substr( 3ul, 2ul );
            second = datetime.substr( 6ul, 2ul );
            if ( datetime.substr( 9ul, 2ul ) == "pm" ) {
                hour = std::to_string( 12 + std::stoi(hour) );
            }

        } else if ( format == date_time_clock_short ) {
            hour   = datetime.substr( 0ul, 2ul );
            minute = datetime.substr( 3ul, 2ul );

        } else [[unlikely]] {
            // wronthing went some ...
            throw DateTimeException("Unexpected DateTime CLOCK", std::string{datetime_}+" - format: "+std::to_string(format));
        }

    } else if ( _DATE_TIME_YEAR & format ) {
        year = datetime;
        if ( format == date_time_year_short ) {
            const int y{ std::stoi( year ) };
            year  = (y<70) ? "20" : "19";
            year += year;
        }

    } else if ( _DATE_TIME_MONTH & format ) {
        if ( format == date_time_month ) {
            month = datetime;
        } else {
            datetime.resize( 3ul ); // may be the full name
            month = convertMonth( datetime );
        }

    } else if ( format == date_time_day ) {
        day = datetime;

    } else if ( format == date_time_hour ) {
        hour = datetime;

    } else if ( format == date_time_minute ) {
        minute = datetime;

    } else if ( format == date_time_second ) {
        second = datetime;

    } else [[unlikely]] {
        // wronthing went some ...
        throw DateTimeException("Unexpected DateTime", std::string{datetime_}+" - format: "+std::to_string(format));
    }

    return { year, month, day, hour, minute, second };
}

} // namespace DateTimeOps

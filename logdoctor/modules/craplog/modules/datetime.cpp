
#include "datetime.h"

#include "modules/exceptions.h"
#include "utilities/strings.h"

#include <ctime>


DateTimeOps::DateTimeOps()
{

}


const std::string DateTimeOps::convertMonth( const std::string& month )
{
    std::string m;
    if ( month == "Jan" ) {
        m = "1";
    } else if ( month == "Feb" ) {
        m = "2";
    } else if ( month == "Mar" ) {
        m = "3";
    } else if ( month == "Apr" ) {
        m = "4";
    } else if ( month == "May" ) {
        m = "5";
    } else if ( month == "Jun" ) {
        m = "6";
    } else if ( month == "Jul" ) {
        m = "7";
    } else if ( month == "Aug" ) {
        m = "8";
    } else if ( month == "Sep" ) {
        m = "9";
    } else if ( month == "Oct" ) {
        m = "10";
    } else if ( month == "Nov" ) {
        m = "11";
    } else if ( month == "Dec" ) {
        m = "12";
    } else {
        // nope
        throw DateTimeException("Unexpected month format: "+month);
    }
    return m;
}



const std::vector<std::string> DateTimeOps::processDateTime( const std::string& datetime_, const std::string& format )
{
    std::string aux, datetime=datetime_;
    std::string year="", month="", day="", hour="", minute="", second="";

    if ( format == "ncsa" ) {
        datetime = StringOps::strip( datetime, "[ ]" );
        day    = datetime.substr( 0, 2 );
        month  = DateTimeOps::convertMonth( datetime.substr( 3, 3 ) );
        year   = datetime.substr( 7, 4 );
        hour   = datetime.substr( 12, 2 );
        minute = datetime.substr( 15, 2 );
        second = datetime.substr( 18, 2 );

    } else if ( format == "mcs" ) {
        month  = DateTimeOps::convertMonth( datetime.substr( 4, 3 ) );
        day    = datetime.substr( 8, 2 );
        hour   = datetime.substr( 11, 2 );
        minute = datetime.substr( 14, 2 );
        second = datetime.substr( 17, 2 );
        year   = datetime.substr( datetime.size()-5 );

    } else if ( format == "gmt" ) {
        int start = datetime.find( ", " ) + 2;
        day    = datetime.substr( start, 2 );
        start += 3;
        month  = DateTimeOps::convertMonth( datetime.substr( start, 3 ) );
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
            datetime = datetime.substr( 0, datetime.size()-6 );
        } else if ( aux == "ms" ) {
            // from milliseconds
            datetime = datetime.substr( 0, datetime.size()-3 );
        } else if ( aux == "s.ms" ) {
            // from seconds.milliseconds
            datetime = std::to_string( std::stoi( datetime ) );
        }
        // convert to iso date format
        const char* c = datetime.c_str();
        struct tm t;
        char d[32];
        strptime( c, "%s", &t );
        strftime( d, sizeof(d), "%Y-%m-%d %H:%M:%S", &t);
        datetime = std::string( d );
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
            month  = datetime.substr( 0, 2 );
            day    = datetime.substr( 3, 2 );
            year   = "20" + datetime.substr( 6, 2 );

        } else if ( format == "MDYY" ) {
            int aux;
            if ( datetime.at(2) == '/' ) {
                month = datetime.substr( 0, 2 );
                aux = 3;
            } else {
                month = "0" + datetime.substr( 0, 1 );
                aux = 2;
            }
            if ( datetime.at(aux+2) == '/' ) {
                day = datetime.substr( aux, 2 );
                aux += 3;
            } else {
                day = "0" + datetime.substr( aux, 1 );
                aux = +2;
            }
            year = "20" + datetime.substr( aux );

        } else if ( StringOps::startsWith( format, "year" ) ) {
            year = datetime;
            if ( format == "year_short" ) {
                year = "20" + year;
            }

        } else if ( StringOps::startsWith( format, "month" ) ) {
            if ( format.size() <= 5 ) {
                month = datetime;
            } else {
                datetime = datetime.substr( 0, 3 ); // may be the full name
                month = DateTimeOps::convertMonth( datetime );
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
            throw DateTimeException("Unexpected DateTime format: "+datetime_);
        }
    }

    return std::vector<std::string>({ year, month, day, hour, minute, second });
}

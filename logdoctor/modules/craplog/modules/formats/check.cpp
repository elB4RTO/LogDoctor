
#include "check.h"

#include "modules/dialogs.h"
#include "modules/shared.h"


namespace /*private*/
{

struct Checker final
{
    Checker( const LogsFormat& format );

    bool keepFormat() const;

private:
    void checkDateTime( const LogsFormatField& field );
    void checkRequest( const LogsFormatField& field );
    void checkControlCharacters( std::string_view format_string );

    bool year{ false };
    bool month{ false };
    bool day{ false };
    bool hour{ false };
    bool minute{ false };
    bool second{ false };
    bool method{ false };
    bool uri{ false };
    bool carriage_return{ false };
};

} // namespace (private)


namespace FormatOps::Private
{

LogsFormat checkFormat( const LogsFormat format )
{
    if ( !format.string.empty() && Checker( format ).keepFormat() ) {
        return format;
    }
    return LogsFormat();
}

} // namespace FormatOps::Private



namespace /*private*/
{

Checker::Checker( const LogsFormat& format )
{
    for ( const LogsFormatField& field : format.fields ) {
        if ( _DISCARDED | field ) {
            if ( _DATE_TIME & field ) {
                checkDateTime( field );
            } else if ( _REQUEST & field ) {
                checkRequest( field );
            }
        }
    }
    checkControlCharacters( format.string );
}
void Checker::checkDateTime( const LogsFormatField& field )
{
    if ( _DATE_TIME_FULL & field ) {
        year |= true; month  |= true; day    |= true;
        hour |= true; minute |= true; second |= true;
    } else if ( _DATE_TIME_DATE & field ) {
        year |= true; month |= true; day |= true;
    } else if ( _DATE_TIME_YEAR & field ) {
        year |= true;
    } else if ( _DATE_TIME_MONTH & field ) {
        month |= true;
    } else if ( date_time_day == field ) {
        day |= true;
    } else if ( _DATE_TIME_CLOCK & field ) {
        hour |= true; minute |= true;
        if ( date_time_clock_short != field ) {
            second |= true;
        }
    } else if ( date_time_utc_t == field ) {
        hour |= true; minute |= true;second |= true;
    } else if ( date_time_hour == field ) {
        hour |= true;
    } else if ( date_time_minute == field ) {
        minute |= true;
    } else if ( date_time_second == field ) {
        second |= true;
    }
}
void Checker::checkRequest( const LogsFormatField& field )
{
    if ( request_full == field ) {
        method |= true; uri |= true;
    } else if ( request_method == field ) {
        method |= true;
    } else if ( (request_uri & field ^ _NO_PARSE_NEEDED) == (request_uri ^ _NO_PARSE_NEEDED) ) {
        uri |= true;
    }
}
void Checker::checkControlCharacters( std::string_view format_string )
{
    carriage_return = format_string.find( '\r' ) != std::string::npos;
}

bool Checker::keepFormat() const
{
    if ( !(year || month || day) ) {
        if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__DATE.c_str() ) ) ) {
            return false;
        }
    } else {
        if ( !year ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__YEAR.c_str() ) ) ) {
                return false;
            }
        }
        if ( !month ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__MONTH.c_str() ) ) ) {
                return false;
            }
        }
        if ( !day ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__DAY.c_str() ) ) ) {
                return false;
            }
        }
    }
    if ( !(hour || minute || second) ) {
        if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__TIME.c_str() ) ) ) {
            return false;
        }
    } else {
        if ( !hour ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__HOUR.c_str() ) ) ) {
                return false;
            }
        }
        if ( !minute ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__MINUTE.c_str() ) ) ) {
                return false;
            }
        }
        if ( !second ) {
            if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( WORDS__SECOND.c_str() ) ) ) {
                return false;
            }
        }
    }
    if ( !method ) {
        if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( FIELDS__METHOD.c_str() ) ) ) {
            return false;
        }
    }
    if ( !uri ) {
        if ( ! DialogSec::choiceLogFormatMissingField( TR::tr( FIELDS__URI.c_str() ) ) ) {
            return false;
        }
    }
    if ( carriage_return ) {
        if ( ! DialogSec::choiceLogFormatWithCarriageReturn() ) {
            return false;
        }
    }
    return true;
}

} // namespace (private)

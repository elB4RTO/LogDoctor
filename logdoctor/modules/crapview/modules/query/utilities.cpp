
#include "modules/exceptions.h"

#include "modules/dialogs.h"


namespace QueryPrivate
{

int toInt( QStringView str )
{
    bool ok;
    const int result{ str.toInt( &ok ) };
    if ( ! ok ) {
        DialogSec::errConvertingData(
            QStringLiteral("QStringView"),
            QStringLiteral("int"),
            str.toString() );
        throw VoidException{};
    }
    return result;
}

int toInt( const QVariant& var )
{
    if ( ! var.canConvert( QMetaType(QMetaType::Int) ) ) {
        DialogSec::errConvertingData(
            QStringLiteral("QVariant"),
            QStringLiteral("int"),
            var.toString() );
        throw VoidException{};
    }
    return var.toInt();
}


QString toString( const QVariant& var )
{
    if ( ! var.canConvert( QMetaType(QMetaType::QString) ) ) {
        DialogSec::errConvertingData(
            QStringLiteral("QVariant"),
            QStringLiteral("QString"),
            QStringLiteral("???") );
        throw VoidException{};
    }
    return var.toString();
}


int getMinuteGap( const int minute, const int gap=10 )
{
    int m{ -1 };
    if ( minute < 0 || minute >= 60 ) {
        // unexpected value
        throw DateTimeException( "Unexpected Minute", std::to_string( minute ) );
    }
    int n{ 0 };
    for ( int g{0}; g<60; g+=gap ) {
        if ( minute >= g && minute < g+gap ) {
            m = gap * n;
            break;
        }
        ++n;
    }
    return m;
}


int getMonthDays( const int year, const int month )
{
    int n_days;
    switch (month) {
    case 1:  n_days = 31; break;
    case 2:  n_days = year%4 == 0 ? 29 : 28 ; break;
    case 3:  n_days = 31; break;
    case 4:  n_days = 30; break;
    case 5:  n_days = 31; break;
    case 6:  n_days = 30; break;
    case 7:  n_days = 31; break;
    case 8:  n_days = 31; break;
    case 9:  n_days = 30; break;
    case 10: n_days = 31; break;
    case 11: n_days = 30; break;
    case 12: n_days = 31; break;
    default:
        throw DateTimeException( "Unexpected Month number", std::to_string( month ) );
    }
    return n_days;
}


int countDays( const int from_year, const int from_month, const int from_day, const int to_year, const int to_month, const int to_day )
{
    int n_days{ 1 };
    if ( from_year == to_year ) {
        if ( from_month == to_month ) {
            n_days += to_day - from_day + 1;
        } else {
            n_days += getMonthDays( from_year, from_month ) - from_day; // first month's days
            for ( int month{from_month+1}; month<to_month; ++month ) {
                n_days += getMonthDays( from_year, month );
            }
            n_days += to_day; // last month's days
        }
    } else {
        n_days += getMonthDays( from_year, from_month ) - from_day; // first month's days
        if ( from_month < 12 ) {
            for ( int month{from_month+1}; month<=12; ++month ) {
                n_days += getMonthDays( from_year, month );
            }
        }
        for ( int year{from_year+1}; year<=to_year; ++year ) {
            int last_month{ 12 };
            if ( year == to_year ) {
                last_month = to_month-1;
                n_days += to_day; // last month's days
            }
            for ( int month{1}; month<=last_month; ++month ) {
                n_days += getMonthDays( year, month );
            }
        }
    }
    return n_days;
}


int countMonths( const int from_year, const int from_month, const int to_year, const int to_month ) noexcept
{
    int n_months{ 0 };
    if ( from_year == to_year ) {
        if ( from_month == to_month ) {
            n_months = 1;
        } else {
            n_months = to_month - from_month + 1;
        }
    } else {
        n_months += 13 - from_month; // months to the end of the first year
        n_months += to_month; // months from the beginning of the last year
        n_months += 12 * ( to_year - from_year - 1 ); // 12 months for every year in the middle
    }
    return n_months;
}

} // namespace QueryPrivate

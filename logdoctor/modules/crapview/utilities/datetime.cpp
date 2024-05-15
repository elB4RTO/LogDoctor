
#include "datetime.h"

#include "modules/crapview/modules/fetcher/utilities.h"

#include "modules/exceptions.h"



DateTime::Value::Value() noexcept
    : value{-1}
    , valid{false}
{
}

DateTime::Value::Value( const int v ) noexcept
    : value{v}
    , valid{true}
{
}

bool DateTime::Value::operator !=( const DateTime::Value& other ) const noexcept
{
    return this->value != other.value;
}



DateTime::DateTime( QStringView year, QStringView month, QStringView day )
{
    using namespace FetcherPrivate;

    if ( ! year.isEmpty() ) {
        this->year = Value( toInt(year) );
    }
    if ( ! month.isEmpty() ) {
        this->month = Value( getMonthNumber(month) );
    }
    if ( ! day.isEmpty() ) {
        this->day = Value( toInt(day) );
    }
}

DateTime::DateTime( QStringView year, QStringView month, QStringView day, QStringView hour )
{
    using namespace FetcherPrivate;

    if ( ! year.isEmpty() ) {
        this->year = Value( toInt(year) );
    }
    if ( ! month.isEmpty() ) {
        this->month = Value( getMonthNumber(month) );
    }
    if ( ! day.isEmpty() ) {
        this->day = Value( toInt(day) );
    }
    if ( ! hour.isEmpty() ) {
        this->hour = Value( toInt(hour) );
    }
}

bool DateTime::DateTime::operator !=( const DateTime& other ) const noexcept
{
    return this->year  != other.year
        || this->month != other.month
        || this->day   != other.day;
}

bool DateTime::isValid() const noexcept
{
    return year.valid && month.valid && day.valid;
}

bool DateTime::isHourValid() const noexcept
{
    return hour.valid;
}

int DateTime::getYear() const
{
    if ( year.valid ) {
        return year.value;
    }
    throw DoNotCatchException("Invalid DateTime Value", "year");
}

int DateTime::getMonth() const
{
    if ( month.valid ) {
        return month.value;
    }
    throw DoNotCatchException("Invalid DateTime Value", "month");
}

int DateTime::getDay() const
{
    if ( day.valid ) {
        return day.value;
    }
    throw DoNotCatchException("Invalid DateTime Value", "day");
}

int DateTime::getHour() const
{
    if ( hour.valid ) {
        return hour.value;
    }
    throw DoNotCatchException("Invalid DateTime Value", "hour");
}

QDate DateTime::toQDate() const
{
    return QDate( getYear(), getMonth(), getDay() );
}

FetcherPrivate::DateManager DateTime::toDateManager() const
{
    using namespace FetcherPrivate;

    return DateManager( getYear(), getMonth(), getDay() );
}

FetcherPrivate::TimeManager DateTime::toTimeManager( const qint64 interval_step ) const
{
    using namespace FetcherPrivate;

    return TimeManager( getYear(), getMonth(), getDay(), interval_step );
}

int DateTime::countMonths( const DateTime& other ) const
{
    if ( other.isValid() ) {
        return FetcherPrivate::countMonths( this->getYear(), this->getMonth(), other.getYear(), other.getMonth() );
    } else {
        return FetcherPrivate::countMonths( this->getYear(), this->getMonth(), this->getYear(), this->getMonth() );
    }
}

int DateTime::countDays( const DateTime& other ) const
{
    if ( other.isValid() ) {
        return FetcherPrivate::countDays( this->getYear(), this->getMonth(), this->getDay(), other.getYear(), other.getMonth(), other.getDay() );
    } else {
        return FetcherPrivate::countDays( this->getYear(), this->getMonth(), this->getDay(), this->getYear(), this->getMonth(), this->getDay() );
    }
}

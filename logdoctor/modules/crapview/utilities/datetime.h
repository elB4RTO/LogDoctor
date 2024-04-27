#ifndef LOGDOCTOR__CRAPVIEW__UTILITIES__DATETIME_H
#define LOGDOCTOR__CRAPVIEW__UTILITIES__DATETIME_H


#include "modules/crapview/modules/fetcher/date_time.h"


class DateTime final
{
    struct Value
    {
        Value() noexcept;
        Value( const int v ) noexcept;

        bool operator !=( const Value& other ) const noexcept;

        int  value;
        bool valid;
    };

public:
    DateTime( QStringView year, QStringView month, QStringView day );
    DateTime( QStringView year, QStringView month, QStringView day, QStringView hour );

    bool operator !=( const DateTime& other ) const noexcept;

    bool isValid() const noexcept;
    bool isHourValid() const noexcept;

    //! \throw DoNotCatchException
    int getYear() const;
    //! \throw DoNotCatchException
    int getMonth() const;
    //! \throw DoNotCatchException
    int getDay() const;
    //! \throw DoNotCatchException
    int getHour() const;

    QDate toQDate() const;
    FetcherPrivate::DateManager toDateManager() const;
    FetcherPrivate::TimeManager toTimeManager( const qint64 interval_step ) const;

    int countMonths( const DateTime& other ) const;
    int countDays( const DateTime& other ) const;

private:
    Value year;
    Value month;
    Value day;

    Value hour;
};


#endif // LOGDOCTOR__CRAPVIEW__UTILITIES__DATETIME_H

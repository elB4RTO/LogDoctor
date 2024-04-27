#ifndef LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__DATE_TIME_H
#define LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__DATE_TIME_H


#include "utilities.h"

#include "modules/exceptions.h"

#include "modules/dialogs.h"

#include "modules/database/database.h"

#include <QDateTime>


namespace FetcherPrivate
{

//! IManager
/*!
    Interface for the TimeManager and DateManager classes
*/
struct IManager
{
    virtual ~IManager() = default;
    virtual qint64 intervalTimestamp() const = 0;
};


//! TimeManager
/*!
    Manages the time and the time interval used
    to group the query data for the stats
*/
class TimeManager final : public IManager
{
    QDateTime time;
    QDateTime time_interval_beg;
    QDateTime time_interval_end;
    const qint64 time_interval_step;

public:
    explicit TimeManager( const int year, const int month, const int day, const qint64 step )
        : time{QDate(year,month,day),QTime(0,0,0)}
        , time_interval_beg{time}
        , time_interval_end{time.addSecs(step)}
        , time_interval_step{step}
    {}
    Q_DISABLE_COPY(TimeManager)

    //! Returns the time interval step, in seconds
    inline qint64 intervalStep() const noexcept
    {
        return time_interval_step;
    }

    //! Returns the current time
    inline QTime currentTime() const
    {
        return time.time();
    }

    //! Returns the timestamp of the current interval, in milliseconds since epoch
    inline qint64 intervalTimestamp() const override
    {
        return time_interval_beg.toMSecsSinceEpoch();
    }

    //! Sets the hour and the minute from the query data
    inline void setHourMinute( const QueryWrapper& query )
    {
        time.setTime( QTime( toInt(query[0]), toInt(query[1]), 0 ) );
    }
    //! Sets the hour, the minute and the second from the query data
    inline void setHourMinuteSecond( const QueryWrapper& query )
    {
        time.setTime( QTime( toInt(query[0]), toInt(query[1]), toInt(query[2]) ) );
    }

    //! Sets the time to the given time
    inline void setTime( const QTime&& new_time )
    {
        time.setTime( new_time );
    }

    //! Sets the interval beginning time and date to the next midnight
    inline void setIntervalToNextMidnight()
    {
        time_interval_beg.setTime( QTime(23,59,59) );
        time_interval_beg.setSecsSinceEpoch( time_interval_beg.toSecsSinceEpoch() + 1 );
        time_interval_end = time_interval_beg.addSecs( time_interval_step );
    }

    //! Returns whether the current time is after the beginning of the time interval
    inline bool isAfterIntervalBegin() const
    {
        return time > time_interval_beg;
    }

    //! Returns whether the time is between the current time interval's begin and eng
    inline bool inCurrentInterval() const
    {
        return time_interval_beg <= time && time < time_interval_end;
    }

    //! Increases the time interval by one step
    inline void increaseInterval()
    {
        time_interval_beg.setSecsSinceEpoch( time_interval_beg.toSecsSinceEpoch() + time_interval_step );
        time_interval_end.setSecsSinceEpoch( time_interval_end.toSecsSinceEpoch() + time_interval_step );
    }

    //! Increases the time interval until it's one step below the current time
    /*!
        \throw DoNotCatchException
    */
    inline void increaseIntervalUntil()
    {
        do    {  increaseInterval(); }
        while ( !inCurrentInterval() );
        assertValid();
        decreaseInterval();
    }

private:

    //! Decreases the time interval by one step
    inline void decreaseInterval()
    {
        time_interval_beg.setSecsSinceEpoch( time_interval_beg.toSecsSinceEpoch() - time_interval_step );
        time_interval_end.setSecsSinceEpoch( time_interval_end.toSecsSinceEpoch() - time_interval_step );
    }

    //! Asserts that the current time is after the beginning of the current interval
    /*!
        \throw DoNotCatchException
    */
    inline void assertValid() const
    {
        if ( time < time_interval_beg ) {
            throw DoNotCatchException{"Unexpected TimeManager state", "time interval exceeds current time"};
        }
    }
};


//! DateManager
/*!
    Manages the date used
    to group the query data for the stats
*/
class DateManager final : public IManager
{
    QDateTime date;
    QDateTime date_interval;

public:
    explicit DateManager( const int year, const int month, const int day )
        : date{QDate(year,month,day),QTime(0,0,0)}
        , date_interval{date}
    {}
    Q_DISABLE_COPY(DateManager)

    //! Returns the interval's timestamp in milliseconds since epoch
    inline qint64 intervalTimestamp() const override
    {
        return date_interval.toMSecsSinceEpoch();
    }

    //! Sets the day from the query data
    inline void setDay( const QueryWrapper& query )
    {
        date.setDate( QDate{date.date().year(), date.date().month(), toInt(query[0])} );
    }

    //! Sets the date to the given date
    inline void setDate( const QDate&& new_date )
    {
        date.setDate( new_date );
    }

    //! Sets the date to the given date
    inline void setIntervalDate( const QDate&& new_date )
    {
        date_interval.setDate( new_date );
    }

    //! Returns whether the date fits in the current time interval
    inline bool inCurrentInterval() const
    {
        return date == date_interval;
    }

    //! Returns whether the time interval is before the given day
    inline bool isIntervalBeforeDay( const int day ) const
    {
        return date_interval.date().day() < day;
    }

    //! Decreases the time interval by one step
    inline void decreaseInterval()
    {
        date_interval = date_interval.addDays( -1 );
    }

    //! Increases the time interval by one step
    inline void increaseInterval()
    {
        date_interval = date_interval.addDays( 1 );
    }

    //! Increases the time interval until it's one step below the current time
    /*!
        \throw DoNotCatchException
    */
    inline void increaseIntervalUntil()
    {
        do    {  increaseInterval(); }
        while ( !inCurrentInterval() );
        assertValid();
        decreaseInterval();
    }

private:
    //! Asserts that the current date is after the beginning of the current interval
    /*!
        \throw DoNotCatchException
    */
    inline void assertValid() const
    {
        if ( date < date_interval ) {
            throw DoNotCatchException{"Unexpected DateManager state", "date interval exceeds current date"};
        }
    }
};

} // namespace FetcherPrivate


#endif // LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__DATE_TIME_H

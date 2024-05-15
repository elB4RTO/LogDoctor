
#include "relational_stats.h"

#include "modules/database/database.h"
#include "modules/crapview/modules/fetcher/date_time.h"

#include <QLineSeries>
#include <QTableWidget>


using namespace FetcherPrivate;


RelationalDatum::RelationalDatum( const qint64 timestamp )
    : timestamp{ timestamp }
    , count{ 0 }
{
}

RelationalDatum::RelationalDatum( const qint64 timestamp, const int count )
    : timestamp{ timestamp }
    , count{ count }
{
}



void RelationalData::pushEmpty( const IManager& date_time )
{
    data.emplace_back( date_time.intervalTimestamp() );
}

void RelationalData::pushDatum( const IManager& date_time, const int count )
{
    data.emplace_back( date_time.intervalTimestamp(), count );
}

bool RelationalData::buildFromQuery( QueryWrapper& query, TimeManager&& time )
{
    if ( query.size() == 0ul ) {
        return false;
    }

    data.reserve( static_cast<size_t>( 24*(60/(time.intervalStep()/60)) ) );

    int count{ 0 };

    pushEmpty( time );

    while ( query->next() ) {
        time.setHourMinute( query );

        if ( time.inCurrentInterval() ) {
            ++ count;
        } else {
            pushDatum( time, count );
            time.increaseInterval();
            if ( ! time.inCurrentInterval() ) {
                pushEmpty( time );
                time.increaseIntervalUntil();
                pushEmpty( time );
                time.increaseInterval();
            }
            count = 1;
        }
    }
    pushDatum( time, count );

    time.increaseInterval();
    pushEmpty( time );

    time.setTime( QTime( 23, 59, 59 ) );
    if ( time.isAfterIntervalBegin() ) {
        time.setIntervalToNextMidnight();
        pushEmpty( time );
    }

    if ( data.capacity() > data.size() ) {
        data.shrink_to_fit();
    }

    return true;
}

bool RelationalData::buildFromQuery( QueryWrapper& query, DateManager&& date, const QDate last_date )
{
    if ( const size_t size{query.size()}; size == 0ul ) {
        return false;
    } else {
        data.reserve( size * 2 );
    }

    date.decreaseInterval();
    pushEmpty( date );
    date.increaseInterval();

    process( query, date, last_date.day()-1 );

    date.setIntervalDate( std::move(last_date) );
    pushEmpty( date );

    if ( data.capacity() > data.size() ) {
        data.shrink_to_fit();
    }

    return true;
}

bool RelationalData::appendFromQuery( QueryWrapper& query, DateManager& date, const QDate initial_date, const QDate final_date, const int last_day )
{
    if ( query.size() == 0ul ) {
        // no data found for this month
        date.setDate( std::move(initial_date) );
        pushEmpty( date );
        date.setDate( std::move(final_date) );
        pushEmpty( date );
        return false;

    } else {
        date.setDate( QDate(initial_date) );
        date.setIntervalDate( std::move(initial_date) );
        process( query, date, last_day );
        return true;
    }
}

void RelationalData::process( QueryWrapper& query, DateManager& date, const int last_day )
{
    int count{ 0 };

    while ( query->next() ) {
        date.setDay( query );

        if ( date.inCurrentInterval() ) {
            ++ count;
        } else {
            pushDatum( date, count );
            date.increaseInterval();
            if ( ! date.inCurrentInterval() ) {
                pushEmpty( date );
                date.increaseIntervalUntil();
                pushEmpty( date );
                date.increaseInterval();
            }
            count = 1;
        }
    }
    pushDatum( date, count );

    if ( date.isIntervalBeforeDay( last_day ) ) {
        date.increaseInterval();
        pushEmpty( date );
    }
}

void RelationalData::reserveSpace( const size_t size )
{
    data.reserve( size );
}

void RelationalData::appendFirstEmpty( DateManager& date )
{
    date.decreaseInterval();
    pushEmpty( date );
    date.increaseInterval();
}

void RelationalData::appendLastEmpty( DateManager& date, const QDate&& last_date )
{
    date.setIntervalDate( std::move(last_date) );
    pushEmpty( date );
}

int RelationalData::insertInChart( QLineSeries*const line ) const
{
    int max_count{ 0 };

    for ( const RelationalDatum& datum : data ) {
        const qint64 time{ datum.timestamp };
        const int count{ datum.count };
        line->append( static_cast<qreal>(time), static_cast<qreal>(count) );
        if ( count > max_count ) {
            max_count = count;
        }
    }

    return max_count;
}

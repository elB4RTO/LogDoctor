
#include "speed_stats.h"

#include "modules/database/database.h"
#include "modules/crapview/modules/fetcher/date_time.h"

#include <QLineSeries>
#include <QTableWidget>

#include <ranges>


using namespace FetcherPrivate;


SpeedDatum::SpeedDatum( const qint64 chart_timestamp )
    : chart_timestamp{ chart_timestamp }
{
}

SpeedDatum::SpeedDatum( const QueryWrapper& query, const TimeManager& time )
    : chart_timestamp{ time.intervalTimestamp() }
    , time{            time.currentTime()       }
    , time_taken{      toString( query[3] )     }
    , uri{             toString( query[4] )     }
    , query{           toString( query[5] )     }
    , method{          toString( query[6] )     }
    , protocol{        toString( query[7] )     }
    , response{        toString( query[8] )     }
{
}



void SpeedData::pushEmpty( const TimeManager& date_time )
{
    data.push_back( { date_time.intervalTimestamp() } );
}

void SpeedData::pushDatum( const SpeedDatum datum )
{
    data.push_back( std::move(datum) );
}

bool SpeedData::buildFromQuery( QueryWrapper& query, TimeManager&& time )
{
    if ( const size_t size{ query.size() }; size > 0ul ) {
        data.reserve( size * 3 );
    } else {
        return false;
    }

    pushEmpty( time );

    while ( query->next() ) {
        time.setHourMinuteSecond( query );
        const SpeedDatum datum{ query, time };

        if ( time.inCurrentInterval() ) {
            pushDatum( datum );
        } else {
            time.increaseInterval();
            if ( ! time.inCurrentInterval() ) {
                pushEmpty( time );
                time.increaseIntervalUntil();
                pushEmpty( time );
                time.increaseInterval();
            }
            pushDatum( datum );
        }
    }

    time.increaseInterval();
    pushEmpty( time );

    time.setTime( std::move(QTime( 23, 59, 59 )) );
    if ( time.isAfterIntervalBegin() ) {
        time.setIntervalToNextMidnight();
        pushEmpty( time );
    }

    if ( data.capacity() > data.size() ) {
        data.shrink_to_fit();
    }

    return true;
}

int SpeedData::insertInChart( QLineSeries*const line ) const
{
    size_t i{ 0 };
    const size_t max_i{ data.size() };
    int value{0}, count{0}, aux_value, max_value{0};
    qint64 time{ -1 };

    const auto finalize_value{
        [&value,&count]()
        {
            if ( value == -1 ) {
                value = 0;
            } else {
                value /= count+1;
            }
        }
    };
    const auto push_to_line{
        [&max_value]( QLineSeries*const line, const qint64 time, const int value )
        {
            line->append( time, value );
            if ( value > max_value ) {
                max_value = value;
            }
        }
    };
    const auto set_values{
        [&time,&value,&count]( const qint64 t, const int v, const int c )
        {
            time = t; value = v; count = c;
        }
    };

    for ( const SpeedDatum& datum : data ) {
        ++i;
        const qint64 aux_time{ datum.chart_timestamp };
        if ( datum.time_taken.isEmpty() ) {
            if ( time != -1 ) {
                finalize_value();
                push_to_line( line, time, value );
            }
            set_values( aux_time, -1, 0 );
            push_to_line( line, time, 0 );
            continue;
        } else {
            aux_value = datum.time_taken.toInt();
            if ( aux_time > time ) {
                finalize_value();
                push_to_line( line, time, value );
                set_values( aux_time, aux_value, 0 );
                if ( i == max_i ) {
                    push_to_line( line, time, value );
                }
            } else {
                if ( value == -1 ) {
                    value = 0;
                } else {
                    ++ count;
                }
                value += aux_value;
                if ( i == max_i ) {
                    value /= count+1;
                    push_to_line( line, aux_time, value );
                }
            }
        }
    }

    return max_value;
}

void SpeedData::insertInTable( QTableWidget*const table ) const
{
    int n_rows{0};

    auto filtered_data{ data | std::views::filter(
        [](const SpeedDatum& datum){ return static_cast<bool>(datum); })
    };

    for ( const SpeedDatum& datum : filtered_data ) {
        table->insertRow( n_rows );
        auto* tt{ new QTableWidgetItem() };
        tt->setData( Qt::DisplayRole, datum.time_taken );
        table->setItem( n_rows, 0, tt );
        table->setItem( n_rows, 1, new QTableWidgetItem( datum.uri ));
        table->setItem( n_rows, 2, new QTableWidgetItem( datum.query ));
        table->setItem( n_rows, 3, new QTableWidgetItem( datum.method ));
        table->setItem( n_rows, 4, new QTableWidgetItem( datum.protocol ));
        table->setItem( n_rows, 5, new QTableWidgetItem( datum.response ));
        table->setItem( n_rows, 6, new QTableWidgetItem( datum.time.toString("hh:mm:ss") ));
        ++ n_rows;
    }
}

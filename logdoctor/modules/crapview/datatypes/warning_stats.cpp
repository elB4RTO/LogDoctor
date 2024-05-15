
#include "warning_stats.h"

#include "modules/crapview/modules/fetcher/utilities.h"

#include "modules/database/database.h"

#include "modules/warnlists/modules/warnlist.h"

#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#if defined( Q_OS_MACOS )
    #include "workarounds/ranges_join.h"
#else
    #include <ranges>
#endif

#include <QBarSet>
#include <QTableWidget>


using namespace FetcherPrivate;


namespace /*private*/
{

QTableWidgetItem* textualTableItem( const QString& data )
{
    return new QTableWidgetItem( data );
}

QTableWidgetItem* numericTableItem( const QString& data )
{
    if ( data.isEmpty() ) {
        return textualTableItem( data );
    } else {
        QTableWidgetItem* item{ new QTableWidgetItem() };
        item->setData( Qt::DisplayRole, data.toInt() );
        return item;
    }
}

} // namespace (private)


WarningDatum::WarningDatum( const QueryWrapper& query )
    : year{           toInt(query[0])     }
    , month{          toInt(query[1])     }
    , day{            toInt(query[2])     }
    , hour{           toInt(query[3])     }
    , minute{         toInt(query[4])     }
    , second{         toInt(query[5])     }
    , protocol{       toString(query[6])  }
    , method{         toString(query[7])  }
    , uri{            toString(query[8])  }
    , query{          toString(query[9])  }
    , response{       toString(query[10]) }
    , user_agent{     toString(query[15]) }
    , client{         toString(query[14]) }
    , cookie{         toString(query[16]) }
    , referer{        toString(query[17]) }
    , bytes_received{ toString(query[13]) }
    , bytes_sent{     toString(query[12]) }
    , time_taken{     toString(query[11]) }
{
}

void WarningDatum::checkWarnings( const Warnlist& warnlist )
{
    std::vector<int> warning_cols;
    warning_cols.reserve(4ul);

    if ( warnlist.method.used ) {
        if ( VecOps::contains( warnlist.method.list, method.toStdString() ) ) {
            warning_cols.push_back( 4 );
        }
    }
    if ( warnlist.uri.used ) {
        const auto target{ uri.toStdString() };
        const auto& list{ warnlist.uri.list };
        if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
            warning_cols.push_back( 5 );
        }
    }
    if ( warnlist.user_agent.used ) {
        const auto target{ user_agent.toStdString() };
        const auto& list{ warnlist.user_agent.list };
        if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
            warning_cols.push_back( 8 );
        }
    }
    if ( warnlist.client.used ) {
        const auto target{ client.toStdString() };
        const auto& list{ warnlist.client.list };
        if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
            warning_cols.push_back( 9 );
        }
    }

    if ( ! warning_cols.empty() ) {
        warning_cols.shrink_to_fit();
        warning_columns = std::move(warning_cols);
    }
}

bool WarningDatum::hasWarnings() const noexcept
{
    return ! warning_columns.empty();
}


void WarningDatum::insertInTable( QTableWidget*const table, const QColor& warning_color ) const
{
    const bool has_warning{ hasWarnings() };

    const int n_rows{ table->rowCount() };
    table->insertRow( n_rows );
    table->setItem( n_rows,  0, new QTableWidgetItem( PrintSec::printableBool( has_warning )) );
    table->setItem( n_rows,  1, new QTableWidgetItem( PrintSec::printableDate( year, month, day )) );
    table->setItem( n_rows,  2, new QTableWidgetItem( PrintSec::printableTime( hour, minute, second )) );
    table->setItem( n_rows,  3, textualTableItem( protocol       ) );
    table->setItem( n_rows,  4, textualTableItem( method         ) );
    table->setItem( n_rows,  5, textualTableItem( uri            ) );
    table->setItem( n_rows,  6, textualTableItem( query          ) );
    table->setItem( n_rows,  7, numericTableItem( response       ) );
    table->setItem( n_rows,  8, textualTableItem( user_agent     ) );
    table->setItem( n_rows,  9, textualTableItem( client         ) );
    table->setItem( n_rows, 10, textualTableItem( cookie         ) );
    table->setItem( n_rows, 11, textualTableItem( referer        ) );
    table->setItem( n_rows, 12, numericTableItem( bytes_received ) );
    table->setItem( n_rows, 13, numericTableItem( bytes_sent     ) );
    table->setItem( n_rows, 14, numericTableItem( time_taken     ) );

    if ( has_warning ) {
        table->item( n_rows, 0 )->setForeground( warning_color );
        for ( const int col : warning_columns ) {
            table->item( n_rows, col )->setForeground( warning_color );
        }
    }
}



void WarningData::setTimelineAsDay()
{
    timeline = TimelineType::Day;

    data.reserve( 24ul );
    for ( size_t h{0ul}; h<24ul; ++h ) {
        data.push_back( std::vector<std::vector<WarningDatum>>{} );
        auto& aux{ data.back() };
        aux.reserve( 6ul );
        for ( int m{0}; m<60; m+=10 ) {
            aux.push_back( std::vector<WarningDatum>{} );
        }
    }
}
void WarningData::setTimelineAsHour()
{
    timeline = TimelineType::Hour;

    data.reserve( 6ul );
    for ( size_t g{0ul}; g<6ul; ++g ) {
        data.push_back( std::vector<std::vector<WarningDatum>>{} );
        auto& aux{ data.back() };
        aux.reserve( 10ul );
        for ( int m{0}; m<10; ++m ) {
            aux.push_back( std::vector<WarningDatum>{} );
        }
    }
}

bool WarningData::buildFromQuery( QueryWrapper& query )
{
    switch ( timeline ) {
        case TimelineType::Day:
            return buildForDay( query );
        case TimelineType::Hour:
            return buildForHour( query );
        default:
            throw DoNotCatchException("WarningData", "unexpected TimelineType");
    }
}
bool WarningData::buildForDay( QueryWrapper& query )
{
    if ( query.size() == 0ul ) {
        return false;
    }

    while ( query->next() ) {
        data.at( static_cast<size_t>( toInt( query[3] ) ) )
            .at( static_cast<size_t>( getMinuteGap( toInt( query[4] ) )/10 ) )
            .emplace_back( query );
    }

    return true;
}
bool WarningData::buildForHour( QueryWrapper& query )
{
    if ( query.size() == 0ul ) {
        return false;
    }

    while ( query->next() ) {
        const int min{ toInt( query[4] ) };
        data.at( static_cast<size_t>( getMinuteGap( min )/10 ) )
            .at( static_cast<size_t>( min % 10 ) )
            .emplace_back( query );
    }

    return true;
}

void WarningData::checkWarnings( const Warnlist& warnlist )
{
    for ( auto& layer1 : data ) {
        for ( auto& layer2 : layer1 ) {
            std::for_each( layer2.begin(), layer2.end(),
                [&warnlist](auto& datum){ datum.checkWarnings(warnlist); } );
        }
    }
}

int WarningData::insertInChart( std::vector<std::vector<QBarSet*>>& bars ) const
{
    switch ( timeline ) {
        case TimelineType::Day:
            return insertForDay( bars );
        case TimelineType::Hour:
            return insertForHour( bars );
        default:
            throw DoNotCatchException("WarningData", "unexpected TimelineType");
    }
}
int WarningData::insertForDay( std::vector<std::vector<QBarSet*>>& bars ) const
{
    int max_count{ 0 };

    for ( int i{0}; i<6; ++i ) {
        bars.push_back( std::vector<QBarSet*>() );
        bars.back().push_back( new QBarSet("") );
        bars.back().push_back( new QBarSet("") );
    }

    for ( size_t h{0}; h<24ul; ++h ) {
        for ( size_t m{0}; m<6ul; ++m ) {
            int norm_count{0}, warn_count{0};
            for ( const WarningDatum& datum : data.at( h ).at( m ) ) {
                datum.hasWarnings() ? ++warn_count : ++norm_count;
            }
            bars.at( m ).at( 0ul )->append( norm_count );
            bars.at( m ).at( 1ul )->append( warn_count );
            const int sum_count{ norm_count + warn_count };
            if ( sum_count > max_count ) {
                max_count = sum_count;
            }
        }
    }

    return max_count;
}
int WarningData::insertForHour( std::vector<std::vector<QBarSet*>>& bars ) const
{
    int max_count{ 0 };

    for ( int i{0}; i<10; ++i ) {
        bars.push_back( std::vector<QBarSet*>() );
        bars.back().push_back( new QBarSet("") );
        bars.back().push_back( new QBarSet("") );
    }

    for ( size_t g{0ul}; g<6ul; ++g ) {
        for ( size_t m{0ul}; m<10ul; ++m ) {
            int norm_count{0}, warn_count{0};
            for ( const WarningDatum& datum : data.at( g ).at( m ) ) {
                datum.hasWarnings() ? ++warn_count : ++norm_count;
            }
            bars.at( m ).at( 0ul )->append( norm_count );
            bars.at( m ).at( 1ul )->append( warn_count );
            const int sum_count{ norm_count + warn_count };
            if ( sum_count > max_count ) {
                max_count = sum_count;
            }
        }
    }

    return max_count;
}

void WarningData::insertInTable( QTableWidget*const table, const QColor& warning_color ) const
{
    #if defined( Q_OS_MACOS )
        const auto flattened_data{ Workarounds::join( Workarounds::join(data) ) };
        for ( const WarningDatum* datum : flattened_data ) {
            datum->insertInTable( table, warning_color );
        }
    #else
        const auto flattened_data{ std::views::join( std::views::join(data) ) };
        for ( const WarningDatum& datum : flattened_data ) {
            datum.insertInTable( table, warning_color );
        }
    #endif
}

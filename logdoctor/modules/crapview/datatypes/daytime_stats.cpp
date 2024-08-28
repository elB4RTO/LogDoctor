
#include "daytime_stats.h"

#include "modules/database/database.h"
#include "modules/crapview/modules/fetcher/utilities.h"

#include <QBarSet>
#include <QTableWidget>

#include <ranges>


using namespace FetcherPrivate;


DaytimeData::DaytimeData()
    : data{ { 0, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 1, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 2, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 3, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 4, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 5, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 6, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 7, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 8, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            { 9, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {10, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {11, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {12, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {13, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {14, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {15, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {16, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {17, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {18, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {19, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {20, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {21, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {22, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
            {23, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}} }
{
}

int DaytimeData::buildFromQuery( QueryWrapper& query )
{
    if ( query.size() == 0ul ) {
        return 0;
    }

    std::unordered_map<int,int> days_l;
    days_l.reserve( 31ul );

    while ( query->next() ) {
        const int day{    toInt( query[0] ) };
        const int hour{   toInt( query[1] ) };
        const int minute{ toInt( query[2] ) };

        ++ data.at( hour ).at( getMinuteGap( minute ) );
        ++ days_l[ day ];
    }

    return static_cast<int>(days_l.size());
}

void DaytimeData::adjustCounts( const int n_days )
{
    for ( auto& [h,data_] : data ) {
        for ( auto& [m,count] : data_ ) {
            count /= n_days;
        }
    }
}

qreal DaytimeData::insertInChart( QList<QBarSet*>& sets ) const
{
    qreal count, max_count{0};
    const auto check_max_count{ [&count,&max_count](){
        if ( count > max_count ) max_count = count;
    }};
    for ( std::size_t h{0ul}; h<24ul; ++h ) {
        const auto& data_{ data.at( h ) };
        count = data_.at( 0ul );
        *sets.at( 0ul ) << count;
        check_max_count();
        count = data_.at( 10ul );
        *sets.at( 1ul ) << count;
        check_max_count();
        count = data_.at( 20ul );
        *sets.at( 2ul ) << count;
        check_max_count();
        count = data_.at( 30ul );
        *sets.at( 3ul ) << count;
        check_max_count();
        count = data_.at( 40ul );
        *sets.at( 4ul ) << count;
        check_max_count();
        count = data_.at( 50ul );
        *sets.at( 5ul ) << count;
        check_max_count();
    }

    return max_count;
}

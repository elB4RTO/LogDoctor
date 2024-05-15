#ifndef LOGDOCTOR__CRAPVIEW__UTILITIES__GLOBALS_H
#define LOGDOCTOR__CRAPVIEW__UTILITIES__GLOBALS_H


#include "modules/crapview/datatypes/global_stats.h"

#include "workarounds/ranges_enumerate.h"

#include <QString>


namespace GlobalsOps
{

inline void appendMostRecurrent( auto& recur_list, const auto& kvr, const QString& __dash )
{
    size_t max{ 0 };
    QStringView max_str{ __dash };
    std::for_each( kvr.begin(), kvr.end(),
        [&max,&max_str](const auto sc)
        { if (auto& [s,c]{sc}; c>max){ max=c; max_str=s; } });
    recur_list.push_back(
        std::make_tuple( max_str.toString(), QString::number(max) ) );
}

inline void appendMostTrafficked( auto& traffic_list, const auto& traf, const size_t max_i, const double max_c, const QString&& value, const QString& __dash, const QString& __zero )
{
    if ( max_i == traf.size() ) {
        traffic_list.emplace_back( __dash, __zero );
    } else {
        const size_t f{ static_cast<size_t>(max_c) };
        const size_t d{ max_c<10.0 ? static_cast<size_t>(max_c*100.0)%100ul : static_cast<size_t>(max_c*10.0)%10ul };
        QString count{ QString::number( f ) };
        if ( d > 0 ) {
            count += QString::number( d ).prepend(QLatin1Char('.'));
        }
        traffic_list.emplace_back( value, count );
    }
}
inline auto getMaxIndexCount( const auto& traf )
{
    double max_c{ 0.0 };
    size_t max_i{ traf.size() };
    /*std::ranges::for_each( std::views::enumerate(traf),
        [&max_c,&max_i](const auto ic)
        { if (auto& [i,c]{ic}; c>max_c){ max_c=c; max_i=i; } });*/
    for( const auto [index,count] : Workarounds::enumerate(traf) ) {
        if ( count > max_c ) {
            max_c = count;
            max_i = index;
        }
    };
    return std::make_tuple( max_i, max_c );
}

inline void appendMeanMaxPerformances( auto& perfs_list, const Perfs& perfs, const QLatin1String suffix )
{
    perfs_list.push_back( std::make_tuple(
        QStringLiteral("%1 %2")
            .arg( perfs.count > 0ul ? perfs.total / perfs.count : perfs.total )
            .arg( suffix ),
        QStringLiteral("%1 %2")
            .arg( perfs.max )
            .arg( suffix )
    ));
}

inline void appendTotalWorkTime( auto& work_list, const Perfs& perfs, const QString& __zero )
{
    float t{ static_cast<float>( perfs.total ) };
    if ( t < 0.f ) {
        work_list.push_back( __zero );
    } else {
        int h{0}, m{0}, s{0}, ms{0};
        if ( t > 1000.f ) {
            ms = static_cast<int>( t ) % 1000;
            t /= 1000.f;
        }
        if ( t > 60.f ) {
            s = static_cast<int>( t );
            s /= 60;
            if ( s > 60 ) {
                m = s/60;
                s %= 60;
                if ( m > 60 ) {
                    h = m/60;
                    m %= 60;
                }
            }
        }
        work_list.push_back( QStringLiteral("%1 h %2 m %3 s %4 ms").arg(h).arg(m).arg(s).arg(ms) );
    }
}

inline void appendTotalDataTransfer( auto& work_list, const Perfs& perfs, const QString& __zero )
{
    float b{ static_cast<float>( perfs.total ) };
    if ( b < 0.f ) {
        work_list.push_back( __zero );
    } else {
        int f{0}, d{0};
        QString sfx{ "B" };
        if ( b > 1024.f ) {
            b /= 1024.f;
            sfx = "KiB";
            if ( b > 1024.f ) {
                b /= 1024.f;
                sfx = "MiB";
                if ( b > 1024.f ) {
                    b /= 1024.f;
                    sfx = "GiB";
                }
            }
        }
        f = static_cast<int>( b );
        d = static_cast<int>( b*1000.f ) %1000;
        work_list.push_back( QStringLiteral("%1.%2 %3").arg(f).arg(d).arg(sfx) );
    }
}

} // namespace GlobalsOps


#endif // LOGDOCTOR__CRAPVIEW__UTILITIES__GLOBALS_H

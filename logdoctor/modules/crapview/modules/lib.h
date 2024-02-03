#ifndef LOGDOCTOR__CRAPVIEW__LIB_H
#define LOGDOCTOR__CRAPVIEW__LIB_H


#include <array>
#include <unordered_map>

#include <QHash>


#define CRAPVIEW_DATA_TYPEDEFS\
    using stats_dates_t       = std::map<int, std::map<int, std::vector<int>>>;\
    using stats_warn_items_t  = std::vector<std::vector<std::vector<std::array<QString,18>>>>;\
    using stats_speed_items_t = std::vector<std::tuple<qint64, std::array<QString,6>>>;\
    using stats_day_items_t   = std::unordered_map<int, std::unordered_map<int, int>>;\
    using stats_relat_items_t = std::vector<std::tuple<qint64, int>>;\
    using stats_count_items_t = std::multimap<unsigned, QString>;\
    using database_dates_t    = std::map<int, stats_dates_t>;


struct RecurrenceData final
{
    QHash<QString, size_t> protocol;
    QHash<QString, size_t> method;
    QHash<QString, size_t> uri;
    QHash<QString, size_t> user_agent;
};

struct TrafficData final
{
    // ( date_str, count )
    std::tuple<QString, QString> date;
    // { day_name : total_count } // 0:monday, ..., 7:sunday
    std::array<double,7> day{ 0, 0, 0, 0, 0, 0, 0 };
    // { hour : total_count }
    std::array<double,24> hour{ .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0 };
};

struct Perfs final
{
    size_t max{   0ul };
    size_t total{ 0ul };
    size_t count{ 0ul };
};
struct PerformanceData final
{
    // [ max, total, count ]
    Perfs time_taken;
    // [ max, total, count ]
    Perfs bytes_sent;
    // [ max, total, count ]
    Perfs bytes_recv;
};

struct GlobalsData final
{
    RecurrenceData recurs;
    TrafficData traf;
    PerformanceData perf;
    size_t req_count{ 0 }; // total number of requests

    GlobalsData() noexcept = default;
    GlobalsData(GlobalsData&&) noexcept = default;
    GlobalsData& operator =(GlobalsData&&) noexcept = default;
    Q_DISABLE_COPY(GlobalsData)
};

#endif // LOGDOCTOR__CRAPVIEW__LIB_H

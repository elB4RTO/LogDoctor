#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__GLOBALS_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__GLOBALS_H


#include <tuple>
#include <array>

#include <QHash>


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


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__GLOBALS_H

#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__SPEED_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__SPEED_H


#include <QDateTime>


class QueryWrapper;

namespace FetcherPrivate {
class TimeManager;
}

class QLineSeries;
class QTableWidget;


class SpeedDatum final
{
    using TimeManager = FetcherPrivate::TimeManager;

public:
    SpeedDatum( const qint64 chart_timestamp );
    SpeedDatum( const QueryWrapper& query, const TimeManager& time );

    explicit operator bool() const { return time.isValid(); }

    const qint64  chart_timestamp;
    const QTime   time;
    const QString time_taken;
    const QString uri;
    const QString query;
    const QString method;
    const QString protocol;
    const QString response;
};


class SpeedData final
{
    using TimeManager = FetcherPrivate::TimeManager;

public:
    SpeedData() = default;

    bool buildFromQuery( QueryWrapper& query, TimeManager&& time );

    int insertInChart( QLineSeries*const line ) const;

    void insertInTable( QTableWidget*const table ) const;

private:
    void pushEmpty( const TimeManager& date_time );
    void pushDatum( const SpeedDatum datum );

    std::vector<SpeedDatum> data;
};


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__SPEED_H

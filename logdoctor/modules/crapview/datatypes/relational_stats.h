#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__RELATIONAL_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__RELATIONAL_H


#include <QDateTime>


class QueryWrapper;

namespace QueryPrivate {
class TimeManager;
class DateManager;
class IManager;
}

class QLineSeries;
class QTableWidget;


struct RelationalDatum final
{
    RelationalDatum() = default;
    RelationalDatum( const qint64 timestamp );
    RelationalDatum( const qint64 timestamp, const int count );

    const qint64 timestamp;
    const int    count;
};


class RelationalData final
{
    using IManager    = QueryPrivate::IManager;
    using TimeManager = QueryPrivate::TimeManager;
    using DateManager = QueryPrivate::DateManager;

public:
    RelationalData() = default;

    bool buildFromQuery( QueryWrapper& query, TimeManager&& time );

    bool buildFromQuery( QueryWrapper& query, DateManager&& date, const QDate last_date );
    bool appendFromQuery( QueryWrapper& query, DateManager& date, const QDate initial_date, const QDate final_date, const int last_day );
    void reserveSpace( const size_t size );
    void appendFirstEmpty( DateManager& date );
    void appendLastEmpty( DateManager& date, const QDate&& last_date );

    int insertInChart( QLineSeries*const line ) const;

private:
    void pushEmpty( const IManager& date_time );
    void pushDatum( const IManager& date_time, const int count );

    void process( QueryWrapper& query, DateManager& date, const int last_day );

    std::vector<RelationalDatum> data;
};


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__RELATIONAL_H

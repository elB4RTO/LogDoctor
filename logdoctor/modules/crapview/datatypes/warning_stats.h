#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__WARNING_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__WARNING_H


#include <QString>

#include <vector>


class QueryWrapper;
struct Warnlist;

class QColor;
class QBarSet;
class QTableWidget;
class QTableWidgetItem;


struct WarningDatum final
{
    WarningDatum( const QueryWrapper& query );

    void checkWarnings( const Warnlist& warnlist );
    bool hasWarnings() const noexcept;

    void insertInTable( QTableWidget*const table, const QColor& warning_color ) const;

    const int year;
    const int month;
    const int day;
    const int hour;
    const int minute;
    const int second;
    const QString protocol;
    const QString method;
    const QString uri;
    const QString query;
    const QString response;
    const QString user_agent;
    const QString client;
    const QString cookie;
    const QString referer;
    const QString bytes_received;
    const QString bytes_sent;
    const QString time_taken;

private:
    std::vector<int> warning_columns;
};


class WarningData final
{
    enum class TimelineType {
        Hour, Day
    };

public:
    WarningData() = default;

    void setTimelineAsDay();
    void setTimelineAsHour();

    //! \throw DoNotCatchException
    bool buildFromQuery( QueryWrapper& query );

    void checkWarnings( const Warnlist& warnlist );

    int insertInChart( std::vector<std::vector<QBarSet*>>& bars ) const;

    void insertInTable( QTableWidget*const table, const QColor& warning_color ) const;

private:
    bool buildForDay( QueryWrapper& query );
    bool buildForHour( QueryWrapper& query );

    int insertForDay( std::vector<std::vector<QBarSet*>>& bars ) const;
    int insertForHour( std::vector<std::vector<QBarSet*>>& bars ) const;

    TimelineType timeline;

    std::vector<std::vector<std::vector<WarningDatum>>> data;
};


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__WARNING_H

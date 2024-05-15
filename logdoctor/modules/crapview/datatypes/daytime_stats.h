#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__DAYTIME_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__DAYTIME_H


#include <QList>

#include <unordered_map>


class QueryWrapper;

class QString;
class QBarSet;
class QTableWidget;


struct DaytimeData final
{
    DaytimeData();

    int buildFromQuery( QueryWrapper& query );

    void adjustCounts( const int n_days );

    qreal insertInChart( QList<QBarSet*>& sets ) const;

private:
    std::unordered_map<int, std::unordered_map<int, qreal>> data;
};


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__DAYTIME_H

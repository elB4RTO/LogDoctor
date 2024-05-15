#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__COUNT_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__COUNT_H


#include <map>


class QueryWrapper;

class QString;
class QPieSeries;
class QTableWidget;


class CountData final
{
public:
    CountData() = default;

    bool buildFromQuery( QueryWrapper& query );

    void insertInChart( QPieSeries*const pie, const int max_slices, const QString others_label ) const;

    void insertInTable( QTableWidget*const table ) const;

private:
    std::multimap<unsigned, QString> data;
};


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__COUNT_H

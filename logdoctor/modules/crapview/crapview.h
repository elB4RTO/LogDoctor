#ifndef CRAPVIEW_H
#define CRAPVIEW_H

#include <QHash>
#include <QtCharts>
#include <QTableWidget>

#include "modules/crapview/modules/query.h"

#include <string>


class Crapview
{
public:
    Crapview();

    const int& getDialogsLevel();
    void setDialogsLevel( const int& new_level );

    void setChartsTheme( const int& new_theme_id );

    void setDbPath( const std::string& path );

    const QString parseBooleanFilter( const QString& filter_str );
    const QString parseNumericFilter( const QString& filter_str );
    const QString parseTextualFilter( const QString& filter_str );

    const QString getLogFieldString ( const int& field_id );
    const int getLogFieldID ( const QString& field_str ),
              getMonthNumber( const QString& month_str );

    void refreshDates();
    void clearDates();

    const QStringList getYears(  const QString& web_server );
    const QStringList getMonths( const QString& web_server, const QString& year );
    const QStringList getDays(   const QString& web_server, const QString& year, const QString& month );
    const QStringList getHours();

    const QStringList getFields( const std::string& tab );

    void updateWarn(
        QTableWidget* table,
        const QString& web_server );
    void drawWarn(
        QTableWidget* table, QtCharts::QChartView* chart,
        const QChart::ChartTheme& theme,
        const std::unordered_map<std::string, QFont>& fonts,
        const QString& web_server,
        const QString& year, const QString& month, const QString& day, const QString& hour );

    void drawSpeed(
        QTableWidget* table, QtCharts::QChartView* chart,
        const QChart::ChartTheme& theme,
        const std::unordered_map<std::string, QFont>& fonts,
        const QString& web_server,
        const QString& year, const QString& month, const QString& day,
        const QString& protocol, const QString& method, const QString& uri, const QString& query, const QString& response );

    void drawCount(
        QTableWidget* table, QtCharts::QChartView* chart,
        const QChart::ChartTheme& theme,
        const std::unordered_map<std::string, QFont>& fonts,
        const QString& web_server,
        const QString& year, const QString& month, const QString& day,
        const QString& field );

    void drawDay(
        QtCharts::QChartView* chart,
            const QChart::ChartTheme& theme,
        const std::unordered_map<std::string, QFont>& fonts,
        const QString& web_server,
        const QString& from_year, const QString& from_month, const QString& from_day,
        const QString& to_year, const QString& to_month, const QString& to_day,
        const QString& field, const QString& filter );

    void drawRelat(
        QtCharts::QChartView* chart,
        const QChart::ChartTheme& theme,
        const std::unordered_map<std::string, QFont>& fonts,
        const QString& web_server,
        const QString& from_year, const QString& from_month, const QString& from_day,
        const QString& to_year, const QString& to_month, const QString& to_day,
        const QString& field_1, const QString& filter_1,
        const QString& field_2, const QString& filter_2 );

    const bool calcGlobals(
        std::vector<std::tuple<QString,QString>>& recur_list,
        std::vector<std::tuple<QString,QString>>& traffic_list,
        std::vector<std::tuple<QString,QString>>& perf_list,
        std::vector<QString>& work_list,
        const QString& web_server );


private:
    // quantity of informational dialogs to display
    int dialogs_level = 2; // 0: essential, 1: usefull, 2: explanatory

    // charts theme
    int charts_theme = 0;

    DbQuery dbQuery;

    // collection of available dates
    // { web_server_id : { year : { month_str : [ days ] } } }
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>> dates;

    // collection of available fields for tabs which needs them
    // { tab : [ fields ] }
    const std::unordered_map<std::string, std::vector<std::string>> fields = {
        {"Daytime", {
            this->dbQuery.FIELDS.at(0),this->dbQuery.FIELDS.at(10),this->dbQuery.FIELDS.at(11),this->dbQuery.FIELDS.at(12),this->dbQuery.FIELDS.at(13),this->dbQuery.FIELDS.at(14),this->dbQuery.FIELDS.at(18),this->dbQuery.FIELDS.at(22),this->dbQuery.FIELDS.at(21),this->dbQuery.FIELDS.at(20)} },
        {"Relational", {
            this->dbQuery.FIELDS.at(0),this->dbQuery.FIELDS.at(10),this->dbQuery.FIELDS.at(11),this->dbQuery.FIELDS.at(12),this->dbQuery.FIELDS.at(13),this->dbQuery.FIELDS.at(14),this->dbQuery.FIELDS.at(15),this->dbQuery.FIELDS.at(16),this->dbQuery.FIELDS.at(17),this->dbQuery.FIELDS.at(18),this->dbQuery.FIELDS.at(22),this->dbQuery.FIELDS.at(21),this->dbQuery.FIELDS.at(20)} }
    };

    const QString printableDate( const QString& year, const int& month, const QString& day );
    const QString printableDate( const int& year, const int& month, const int& day );
    const QString printableTime( const int& hour, const int& minute, const int& second );
    const QString printableWarn( const int& value );

    // conversion between text and IDs
    const QHash<QString, int>
        WebServer_s2i = {
            {"apache",11}, {"nginx",12}, {"iis",13} },
        LogFields_s2i = {
            {QString::fromStdString(this->dbQuery.FIELDS.at( 0)), 0},
            {QString::fromStdString(this->dbQuery.FIELDS.at(10)), 10},
            {QString::fromStdString(this->dbQuery.FIELDS.at(11)), 11},
            {QString::fromStdString(this->dbQuery.FIELDS.at(12)), 12},
            {QString::fromStdString(this->dbQuery.FIELDS.at(13)), 13},
            {QString::fromStdString(this->dbQuery.FIELDS.at(14)), 14},
            {QString::fromStdString(this->dbQuery.FIELDS.at(15)), 15},
            {QString::fromStdString(this->dbQuery.FIELDS.at(16)), 16},
            {QString::fromStdString(this->dbQuery.FIELDS.at(17)), 17},
            {QString::fromStdString(this->dbQuery.FIELDS.at(18)), 18},
            {QString::fromStdString(this->dbQuery.FIELDS.at(20)), 20},
            {QString::fromStdString(this->dbQuery.FIELDS.at(21)), 21},
            {QString::fromStdString(this->dbQuery.FIELDS.at(22)), 22}},
        Months_s2i = {
            {QString::fromStdString(this->dbQuery.MONTHS.at(1)),   1},
            {QString::fromStdString(this->dbQuery.MONTHS.at(2)),   2},
            {QString::fromStdString(this->dbQuery.MONTHS.at(3)),   3},
            {QString::fromStdString(this->dbQuery.MONTHS.at(4)),   4},
            {QString::fromStdString(this->dbQuery.MONTHS.at(5)),   5},
            {QString::fromStdString(this->dbQuery.MONTHS.at(6)),   6},
            {QString::fromStdString(this->dbQuery.MONTHS.at(7)),   7},
            {QString::fromStdString(this->dbQuery.MONTHS.at(8)),   8},
            {QString::fromStdString(this->dbQuery.MONTHS.at(9)),   9},
            {QString::fromStdString(this->dbQuery.MONTHS.at(10)), 10},
            {QString::fromStdString(this->dbQuery.MONTHS.at(11)), 11},
            {QString::fromStdString(this->dbQuery.MONTHS.at(12)), 12}};
};

#endif // CRAPVIEW_H

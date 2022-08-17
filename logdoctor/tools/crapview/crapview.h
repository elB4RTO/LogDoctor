#ifndef CRAPVIEW_H
#define CRAPVIEW_H

#include <QHash>
#include <QtCharts>
#include <QTableWidget>

#include "tools/crapview/modules/query.h"

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

    const int getLogFieldID ( const QString& field_str ),
              getMonthNumber( const QString& month_str );

    void refreshDates();
    void clearDates();

    const QStringList getYears(  const QString& web_server );
    const QStringList getMonths( const QString& web_server, const QString& year );
    const QStringList getDays(   const QString& web_server, const QString& year, const QString& month );
    const QStringList getHours();

    const QStringList getFields( const QString& tab );

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

    QString TITLE_WARN = QMessageBox::tr("Log Lines Marked as Warning"),
            TITLE_SPEED = QMessageBox::tr("Time Taken to Serve Requests"),
            TEXT_COUNT_OTHERS = QMessageBox::tr("Others"),
            TITLE_DAY  = QMessageBox::tr("Time of Day Count"),
            TITLE_RELAT       = QMessageBox::tr("Relational Count"),
            LEGEND_FROM = QMessageBox::tr("from"),
            LEGEND_TO   = QMessageBox::tr("to"),

            DATE = QMessageBox::tr("Date"),
            TIME = QMessageBox::tr("Time");

    // collection of available dates
    // { web_server_id : { year : { month_str : [ days ] } } }
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>> dates;

    // collection of available fields for tabs which needs them
    // { tab : [ fields ] }
    const QHash<QString, QStringList> fields = {
        {"Daytime", {
            FIELDS.value(0),FIELDS.value(10),FIELDS.value(11),FIELDS.value(12),FIELDS.value(13),FIELDS.value(14),FIELDS.value(18),FIELDS.value(22),FIELDS.value(21),FIELDS.value(20)} },
        {"Relational", {
            FIELDS.value(0),FIELDS.value(10),FIELDS.value(11),FIELDS.value(12),FIELDS.value(13),FIELDS.value(14),FIELDS.value(15),FIELDS.value(16),FIELDS.value(17),FIELDS.value(18),FIELDS.value(22),FIELDS.value(21),FIELDS.value(20)} }
    };

    const QString printableDate( const QString& year, const int& month, const QString& day );
    const QString printableDate( const int& year, const int& month, const int& day );
    const QString printableTime( const int& hour, const int& minute, const int& second );
    const QString printableWarn( const int& value );

    const QStringList getWarnHeader();

    // conversion between text and IDs
    const QHash<QString, int>
        WebServer_s2i = {
            {"Apache2",11}, {"Nginx",12}, {"IIS",13} },
        LogFields_s2i = {
            {FIELDS.value(0), 0},
            {FIELDS.value(10),10},
            {FIELDS.value(11),11},
            {FIELDS.value(12),12},
            {FIELDS.value(13),13},
            {FIELDS.value(14),14},
            {FIELDS.value(15),15},
            {FIELDS.value(16),16},
            {FIELDS.value(17),17},
            {FIELDS.value(18),18},
            {FIELDS.value(20),20},
            {FIELDS.value(21),21},
            {FIELDS.value(22),22}
        },
        Months_s2i = {
            {MONTHS.value(1),1},   {MONTHS.value(2),2},   {MONTHS.value(3),3},
            {MONTHS.value(4),4},   {MONTHS.value(5),5},   {MONTHS.value(6),6},
            {MONTHS.value(7),7},   {MONTHS.value(8),8},   {MONTHS.value(9),9},
            {MONTHS.value(10),10}, {MONTHS.value(11),11}, {MONTHS.value(12),12} };
};

#endif // CRAPVIEW_H

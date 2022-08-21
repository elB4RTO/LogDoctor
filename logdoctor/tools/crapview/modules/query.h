#ifndef QUERY_H
#define QUERY_H

#include <QMessageBox>
#include <QString>
#include <QHash>

#include <vector>
#include <unordered_map>


class DbQuery
{
public:
    DbQuery();

    const QHash<int, QString>
        FIELDS = {
            {0, QMessageBox::tr("Warning")},
            {10,QMessageBox::tr("Protocol")},   {11,QMessageBox::tr("Method")},
            {12,QMessageBox::tr("URI")},        {13,QMessageBox::tr("Query")},      {14,QMessageBox::tr("Response code")},
            {15,QMessageBox::tr("Time taken")}, {16,QMessageBox::tr("Bytes sent")}, {17,QMessageBox::tr("Bytes received")},
            {18,QMessageBox::tr("Referrer")},   {22,QMessageBox::tr("Cookie")},
            {20,QMessageBox::tr("Client")},     {21,QMessageBox::tr("User-agent")} },
        MONTHS = {
            {1, QMessageBox::tr("January")},  {2, QMessageBox::tr("February")},  {3, QMessageBox::tr("March")},
            {4, QMessageBox::tr("April")},    {5, QMessageBox::tr("May")},       {6, QMessageBox::tr("June")},
            {7, QMessageBox::tr("July")},     {8, QMessageBox::tr("August")},    {9, QMessageBox::tr("September")},
            {10,QMessageBox::tr("October")},  {11,QMessageBox::tr("November")},  {12,QMessageBox::tr("December")} },
        DAYS = {
            {1, QMessageBox::tr("Sunday")},    {2, QMessageBox::tr("Monday")},   {3, QMessageBox::tr("Tuesday")},
            {4, QMessageBox::tr("Wednesday")}, {5, QMessageBox::tr("Thursday")}, {6, QMessageBox::tr("Friday")},
            {7, QMessageBox::tr("Saturday")} };

    void setDialogLevel( const int& new_level );

    void setDbPath( const std::string& path );
    //const std::string getDbPath( const int web_server );

    const int getMonthsCount( const QString& from_year, const QString& from_month, const QString& to_year, const QString& to_month );

    void refreshDates( std::tuple<bool, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>& result );

    void updateWarnings(
        const QString& web_server,
        const std::vector<std::tuple<int, int>>& updates );

    void getWarnCounts(
        std::tuple<bool, std::vector<std::vector<std::vector<std::vector<QString>>>>>& result,
        const QString& web_server,
        const QString& year_,      const QString& month_,   const QString& day_, const QString& hour_ );

    void getSpeedData(
            std::tuple<bool, std::vector<std::tuple<long long, std::vector<QString>>>>& result,
            const QString& web_server,
            const QString& year_, const QString& month_, const QString& day_,
            const QString& protocol_f, const QString& method_f, const QString& uri_f, const QString& query_f, const QString& response_f );

    void getItemsCount(
        std::tuple<bool, std::vector<std::tuple<QString, int>>>& result,
        const QString& web_server,
        const QString& year,       const QString& month,    const QString& day,
        const QString& log_field );

    void getDaytimeCounts(
        std::tuple<bool, std::unordered_map<int, std::unordered_map<int, int>>>& result,
        const QString& web_server,
        const QString& from_year_, const QString& from_month_, const QString& from_day_,
        const QString& to_year_,   const QString& to_month_,   const QString& to_day_,
        const QString& log_field_, const QString& field_filter );

    void getRelationalCountsDay(
        std::tuple<bool, std::vector<std::tuple<long long, int>>>& result,
        const QString& web_server,
        const QString& year_,        const QString& month_,         const QString& day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2 );

    void getRelationalCountsPeriod(
        std::tuple<bool, std::vector<std::tuple<long long, int>>>& result,
        const QString& web_server,
        const QString& from_year_,   const QString& from_month_,    const QString& from_day_,
        const QString& to_year_,     const QString& to_month_,      const QString& to_day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2 );

    const bool getGlobalCounts(
        const QString& web_server,
        const std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& dates,
        std::vector<std::unordered_map<QString, int>>& recurs,
        std::tuple<QString, int>& traf_date,
        std::unordered_map<int, double>& traf_day,
        std::unordered_map<int, double>& traf_hour,
        std::vector<long long>& perf_time,
        std::vector<long long>& perf_sent,
        std::vector<long long>& perf_receiv,
        long& req_count );

private:
    const QString MSG_ERR_UNX_WS = QMessageBox::tr("Unexpected WebServer");
    const QString MSG_ERR_PROCESSING = QMessageBox::tr("An error occured while processing");
    const QString MSG_ERR_PROCESSING_DATES = QMessageBox::tr("An error occured while processing dates");
    const QString MSG_ERR_PARSING_YMD = QMessageBox::tr("An error occured while parsing %1 from the database");
    const QString WORD_YEARS  = QMessageBox::tr("Years");
    const QString WORD_MONTHS = QMessageBox::tr("Months");
    const QString WORD_DAYS   = QMessageBox::tr("Days");
    const QString MSG_RESPONSIBLE_VALUE = QMessageBox::tr("Value responsible for the error");
    const QString MSG_TABLE_NAME = QMessageBox::tr("Database table name");

    // quantity of informational dialogs to display
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    std::string db_path;
    QString db_name;

    const QHash<QString, QString>
        LogFields_to_DbFields = {
            {this->FIELDS.value(0),  "warning"},
            {this->FIELDS.value(10), "protocol"},
            {this->FIELDS.value(11), "method"},
            {this->FIELDS.value(12), "uri"},
            {this->FIELDS.value(13), "query"},
            {this->FIELDS.value(14), "response"},
            {this->FIELDS.value(15), "time_taken"},
            {this->FIELDS.value(16), "bytes_sent"},
            {this->FIELDS.value(17), "bytes_received"},
            {this->FIELDS.value(18), "referrer"},
            {this->FIELDS.value(20), "client"},
            {this->FIELDS.value(21), "user_agent"},
            {this->FIELDS.value(22), "cookie"}
        };

    const QHash<QString, int>
        Months_s2i = {
            {this->MONTHS.value(1),1},   {this->MONTHS.value(2),2},   {this->MONTHS.value(3),3},
            {this->MONTHS.value(4),4},   {this->MONTHS.value(5),5},   {this->MONTHS.value(6),6},
            {this->MONTHS.value(7),7},   {this->MONTHS.value(8),8},   {this->MONTHS.value(9),9},
            {this->MONTHS.value(10),10}, {this->MONTHS.value(11),11}, {this->MONTHS.value(12),12} };

    const int getMinuteGap( const int& minute, const int& gap=10 ),
              getMonthDays( const int& year, const int& month ),
              getMonthsCount( const int& from_year, const int& from_month, const int& to_year, const int& to_month );
};

#endif // QUERY_H

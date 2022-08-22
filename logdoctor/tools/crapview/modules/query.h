#ifndef QUERY_H
#define QUERY_H

#include "tools/shared.h"

#include <QString>
#include <QHash>

#include <vector>
#include <unordered_map>


class DbQuery
{
public:
    DbQuery();

    const std::unordered_map<int, std::string>
        FIELDS = {
            {0, FIELDS__WARNING},
            {10,FIELDS__PROTOCOL},   {11,FIELDS__METHOD},
            {12,FIELDS__URI},        {13,FIELDS__QUERY},      {14,FIELDS__RESPONSE_CODE},
            {15,FIELDS__TIME_TAKEN}, {16,FIELDS__BYTES_SENT}, {17,FIELDS__BYTES_RECEIVED},
            {18,FIELDS__REFERRER},   {22,FIELDS__COOKIE},
            {20,FIELDS__CLIENT},     {21,FIELDS__USER_AGENT} },
        MONTHS = {
            {1, MONTHS__JANUARY},  {2, MONTHS__FEBRUARY},  {3, MONTHS__MARCH},
            {4, MONTHS__APRIL},    {5, MONTHS__MAY},       {6, MONTHS__JUNE},
            {7, MONTHS__JULY},     {8, MONTHS__AUGUST},    {9, MONTHS__SEPTEMBER},
            {10,MONTHS__OCTOBER},  {11,MONTHS__NOVEMBER},  {12,MONTHS__DECEMBER} },
        DAYS = {
            {1, DAYS__SUNDAY},    {2, DAYS__MONDAY},   {3, DAYS__TUESDAY},
            {4, DAYS__WEDNESDAY}, {5, DAYS__THURSDAY}, {6, DAYS__FRIDAY},
            {7, DAYS__SATURDAY} };

    const QString getDbField( const QString& tr_fld );
    const int getLogFieldID ( const QString& field_str ),
              getMonthNumber( const QString& month_str );

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
    const std::string MSG_ERR_UNX_WS = TR::tr("Unexpected WebServer").toStdString();
    const std::string MSG_ERR_PROCESSING = TR::tr("An error occured while processing").toStdString();
    const std::string MSG_ERR_PROCESSING_DATES = TR::tr("An error occured while processing dates").toStdString();
    const std::string MSG_ERR_PARSING_YMD = TR::tr("An error occured while parsing %1 from the database").toStdString();
    const std::string WORD_YEARS  = TR::tr("Years").toStdString();
    const std::string WORD_MONTHS = TR::tr("Months").toStdString();
    const std::string WORD_DAYS   = TR::tr("Days").toStdString();
    const std::string MSG_RESPONSIBLE_VALUE = TR::tr("Value responsible for the error").toStdString();
    const std::string MSG_TABLE_NAME = TR::tr("Database table name").toStdString();

    // quantity of informational dialogs to display
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    std::string db_path;
    QString db_name;

    const std::unordered_map<std::string, QString>
        LogFields_to_DbFields = {
            {this->FIELDS.at( 0), "warning"},
            {this->FIELDS.at(10), "protocol"},
            {this->FIELDS.at(11), "method"},
            {this->FIELDS.at(12), "uri"},
            {this->FIELDS.at(13), "query"},
            {this->FIELDS.at(14), "response"},
            {this->FIELDS.at(15), "time_taken"},
            {this->FIELDS.at(16), "bytes_sent"},
            {this->FIELDS.at(17), "bytes_received"},
            {this->FIELDS.at(18), "referrer"},
            {this->FIELDS.at(20), "client"},
            {this->FIELDS.at(21), "user_agent"},
            {this->FIELDS.at(22), "cookie"}
        };

    const int getMinuteGap( const int& minute, const int& gap=10 ),
              getMonthDays( const int& year, const int& month ),
              getMonthsCount( const int& from_year, const int& from_month, const int& to_year, const int& to_month );
};

#endif // QUERY_H

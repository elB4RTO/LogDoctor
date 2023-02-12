#ifndef QUERY_H
#define QUERY_H

#include "modules/shared.h"
#include "utilities/result.h"

#include <QString>

#include <map>
#include <vector>
#include <unordered_map>


class DbQuery
{
public:
    explicit DbQuery();

    // convert log fields IDs to log fields
    const std::unordered_map<int, std::string> FIELDS = {
            {0, FIELDS__WARNING},
            {10,FIELDS__PROTOCOL},   {11,FIELDS__METHOD},
            {12,FIELDS__URI},        {13,FIELDS__QUERY},      {14,FIELDS__RESPONSE_CODE},
            {15,FIELDS__TIME_TAKEN}, {16,FIELDS__BYTES_SENT}, {17,FIELDS__BYTES_RECEIVED},
            {18,FIELDS__REFERRER},   {22,FIELDS__COOKIE},
            {20,FIELDS__CLIENT},     {21,FIELDS__USER_AGENT} };

    // convert month numbers to month names
    const std::unordered_map<int, std::string> MONTHS = {
            {1, MONTHS__JANUARY},  {2, MONTHS__FEBRUARY},  {3, MONTHS__MARCH},
            {4, MONTHS__APRIL},    {5, MONTHS__MAY},       {6, MONTHS__JUNE},
            {7, MONTHS__JULY},     {8, MONTHS__AUGUST},    {9, MONTHS__SEPTEMBER},
            {10,MONTHS__OCTOBER},  {11,MONTHS__NOVEMBER},  {12,MONTHS__DECEMBER} };

    // convert week-day numbers to day names
    const std::unordered_map<int, std::string> DAYS = {
            {1, DAYS__SUNDAY},    {2, DAYS__MONDAY},   {3, DAYS__TUESDAY},
            {4, DAYS__WEDNESDAY}, {5, DAYS__THURSDAY}, {6, DAYS__FRIDAY},
            {7, DAYS__SATURDAY} };


    //! Returns the Dialogs level
    void setDialogLevel( const int new_level );


    //! Sets the path for the logs Collection database
    /*!
        \see Crapview::setDbPath()
    */
    void setDbPath( const std::string& path );

    /*const std::string getDbPath( const int web_server );*/

    //! Returns the number of months in a given period
    /*!
        \overload const int getMonthsCount(const int& from_year, const int& from_month, const int& to_year, const int& to_month)
        \param from_year The initial year
        \param from_month The initial month
        \param to_year The final Year
        \param to_month The final month
        \return The number of months in the period
        \throw DateTimeException
    */
    const int countMonths(
        const QString& from_year,
        const QString& from_month,
        const QString& to_year,
        const QString& to_month
    ) const;


    //! Refreshes the dates which are available in the database
    /*!
        \param result Tuple which will hold the result of the operation and the data
    */
    void refreshDates( Result<stats_dates_t>& result );


    //! Updates the database applying the changes made in the Warnings statistics table
    /*!
        \param web_server The ID of the Web Server to use
        \param updates The list of rowids to update and the corresponding warning values
    */
    void updateWarnings(
        const QString& web_server,
        const std::vector<std::tuple<int, int>>& updates
    ) const;

    //! Retrieves the data needed for the Warnings statistics
    /*!
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param hour_ The hour
    */
    void getWarnCounts(
        Result<stats_warn_items_t>& result,
        const QString& web_server,
        const QString& year_,
        const QString& month_,
        const QString& day_,
        const QString& hour_
    ) const;


    //! Retrieves the data needed for the Speed statistics
    /*!
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param protocol_f The filter for the Protocol field
        \param method_f The filter for the Method field
        \param uri_f The filter for the URI field
        \param query_f The filter for the Query field
        \param response_f The filter for the Response field
    */
    void getSpeedData(
        Result<stats_speed_items_t>& result,
        const QString& web_server,
        const QString& year_,
        const QString& month_,
        const QString& day_,
        const QString& protocol_f,
        const QString& method_f,
        const QString& uri_f,
        const QString& query_f,
        const QString& response_f
    ) const;


    //! Retrieves the data needed for the Counts statistics
    /*!
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param year The year
        \param month The month
        \param day The day
        \param log_field The log field
    */
    void getItemsCount(
        Result<stats_count_items_t>& result,
        const QString& web_server,
        const QString& year,
        const QString& month,
        const QString& day,
        const QString& log_field
    ) const;


    //! Retrieves the data needed for the Daytime statistics
    /*!
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param from_year_ The initial year
        \param from_month_ The initial month
        \param from_day_ The initial day
        \param to_year_ The final year
        \param to_month_ The final month
        \param to_day_ The final day
        \param log_field_ The log field to filter
        \param field_filter The filter to apply
    */
    void getDaytimeCounts(
        Result<stats_day_items_t>& result,
        const QString& web_server,
        const QString& from_year_, const QString& from_month_, const QString& from_day_,
        const QString& to_year_,   const QString& to_month_,   const QString& to_day_,
        const QString& log_field_, const QString& field_filter
    ) const;


    //! Retrieves the data needed for the Relationsl statistics
    /*!
        Used when querying a single day
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param log_field_1_ The first log field to filter
        \param field_filter_1 The filter to apply to the first field
        \param log_field_2_ The second log field to filter
        \param field_filter_2 The filter to apply to the second fiend
        \see getRelationalCountsPeriod()
    */
    void getRelationalCountsDay(
        Result<stats_relat_items_t>& result,
        const QString& web_server,
        const QString& year_,        const QString& month_,         const QString& day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2
    ) const;

    //! Retrieves the data needed for the Relational statistics
    /*!
        Used when querying a period of time
        \param result Tuple which will hold the result of the operation and the data
        \param web_server The ID of the Web Server to use
        \param from_year_ The initial year
        \param from_month_ The initial month
        \param from_day_ The initial day
        \param to_year_ The final year
        \param to_month_ The final month
        \param to_day_ The final day
        \param log_field_1_ The first log field to filter
        \param field_filter_1 The filter to apply to the first field
        \param log_field_2_ The second log field to filter
        \param field_filter_2 The filter to apply to the second fiend
        \see getRelationalCountsDay()
    */
    void getRelationalCountsPeriod(
        Result<stats_relat_items_t>& result,
        const QString& web_server,
        const QString& from_year_,   const QString& from_month_,    const QString& from_day_,
        const QString& to_year_,     const QString& to_month_,      const QString& to_day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2
    ) const;


    //! Retrieves the data needed for the Global statistics
    /*!
        \param web_server The ID of the Web Server to use
        \param dates The dates to query
        \param recurs Will hold the recurrencies of the items
        \param traf_date Will hold the most trafficked date and the count
        \param traf_day Will hold the most trafficked day of the week and the count
        \param traf_hour Will hold the most trafficked hour of the day and the count
        \param perf_time Will hold the overallo time taken
        \param perf_sent Will hold the overall Bytes sent
        \param perf_receiv Will hold the overall Bytes received
        \param req_count Will hold the number of requests examined by the query
        \return Whether the operation has been successful or not
    */
    const bool getGlobalCounts(
        const QString& web_server,
        const std::map<int, std::map<int, std::vector<int>>>& dates,
        std::vector<std::unordered_map<QString, unsigned>>& recurs,
        std::tuple<QString, int>& traf_date,
        std::unordered_map<int, double>& traf_day,
        std::unordered_map<int, double>& traf_hour,
        std::vector<long long>& perf_time,
        std::vector<long long>& perf_sent,
        std::vector<long long>& perf_receiv,
        long& req_count
    ) const;

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

    // quantity of information to display throught dialogs
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    std::string db_path;
    QString db_name;

    // convert log fields to database fields
    const std::unordered_map<std::string, QString> LogFields_to_DbFields = {
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

    //! Returns the database field corresponding to the relative log field
    /*!
        \param tr_fld The log field, hendles translated text
        \return The database field
    */
    const QString getDbField( const QString& tr_fld ) const;

    /*const int getLogFieldID ( const QString& field_str );*/


    //! Returns the minute gap for the given minute with the given gap
    /*!
        \param minute The minute of the hour
        \param gap The gap used to split the hour
        \return The gap index
        \throw DateTimeException
    */
    static int getMinuteGap( const int minute, const int gap=10 );


    //! Returns the number of days for a given month
    /*!
        \param year The year, used to handle leap years
        \param month The month
        \return The number of days
        \throw DateTimeException
    */
    static int getMonthDays( const int year, const int month );

    //! Returns the month number in the year
    /*!
        \param month_str The month
        \return The month number
    */
    const int getMonthNumber( const QString& month_str ) const;


    //! Returns the number of days in a given period
    /*!
        \param from_year The initial year
        \param from_month The initial month
        \param from_day The initial day
        \param to_year The final Year
        \param to_month The final month
        \param to_day The final day
        \return The number of days
        \throw DateTimeException
    */
    static int countDays(
        const int from_year, const int from_month, const int from_day,
        const int to_year, const int to_month, const int to_day );


    //! Returns the number of months in a given period
    /*!
        \param from_year The initial year
        \param from_month The initial month
        \param to_year The final Year
        \param to_month The final month
        \return The number of months in the period
    */
    static int countMonths(
        const int& from_year, const int& from_month,
        const int& to_year,   const int& to_month );
};

#endif // QUERY_H

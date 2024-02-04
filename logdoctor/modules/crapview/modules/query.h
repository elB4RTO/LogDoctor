#ifndef LOGDOCTOR__CRAPVIEW__MODULES__QUERY_H
#define LOGDOCTOR__CRAPVIEW__MODULES__QUERY_H

#include "lib.h"

#include "main_lib.h"

#include "modules/shared.h"

#include "modules/crapview/lib.h"

#include <unordered_map>
#include <optional>


class DbQuery final
{
    CRAPVIEW_DATA_TYPEDEFS

public:

    // log fields enums to log fields strings
    const std::unordered_map<LogField, std::string> FIELDS{
        {LogField::Warning,       FIELDS__WARNING},
        {LogField::Protocol,      FIELDS__PROTOCOL},
        {LogField::Method,        FIELDS__METHOD},
        {LogField::Uri,           FIELDS__URI},
        {LogField::Query,         FIELDS__QUERY},
        {LogField::ResponseCode,  FIELDS__RESPONSE_CODE},
        {LogField::TimeTaken,     FIELDS__TIME_TAKEN},
        {LogField::BytesSent,     FIELDS__BYTES_SENT},
        {LogField::BytesReceived, FIELDS__BYTES_RECEIVED},
        {LogField::Referrer,      FIELDS__REFERRER},
        {LogField::Cookie,        FIELDS__COOKIE},
        {LogField::Client,        FIELDS__CLIENT},
        {LogField::UserAgent,     FIELDS__USER_AGENT} };

    // convert month numbers to month names
    const std::unordered_map<int, std::string> MONTHS{
        {1, MONTHS__JANUARY},  {2, MONTHS__FEBRUARY},  {3, MONTHS__MARCH},
        {4, MONTHS__APRIL},    {5, MONTHS__MAY},       {6, MONTHS__JUNE},
        {7, MONTHS__JULY},     {8, MONTHS__AUGUST},    {9, MONTHS__SEPTEMBER},
        {10,MONTHS__OCTOBER},  {11,MONTHS__NOVEMBER},  {12,MONTHS__DECEMBER} };

    // convert week-day numbers to day names
    const std::unordered_map<int, std::string> DAYS{
        {1, DAYS__MONDAY},   {2, DAYS__TUESDAY}, {3, DAYS__WEDNESDAY},
        {4, DAYS__THURSDAY}, {5, DAYS__FRIDAY},  {6, DAYS__SATURDAY},
        {7, DAYS__SUNDAY} };


    //! Returns the Dialogs level
    void setDialogLevel( const DialogsLevel new_level ) noexcept;


    //! Sets the path for the logs Collection database
    /*!
        \see Crapview::setDbPath()
    */
    void setDbPath( std::string&& path ) noexcept;

    /*const std::string getDbPath( const int web_server );*/

    //! Returns the number of months in a given period
    /*!
        \overload const int getMonthsCount(const int& from_year, const int& from_month, const int& to_year, const int& to_month)
        \param from_year The initial year
        \param from_month The initial month
        \param to_year The final Year
        \param to_month The final month
        \return The number of months in the period
        \throw ConversionException
        \throw DateTimeException
    */
    int countMonths(
        QStringView from_year,
        QStringView from_month,
        QStringView to_year,
        QStringView to_month
    ) const;


    //! Refreshes the dates which are available in the database
    /*!
        \param result Holds the data only if the operation completed succssfully
        \throw LogDoctorException
        \throw ConversionException
    */
    void refreshDates( std::optional<database_dates_t>& result ) noexcept;


    //! Retrieves the data needed for the Warnings statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param hour_ The hour
        \throw LogDoctorException
        \throw ConversionException
        \throw DateTimeException
    */
    void getWarningsData(
        std::optional<stats_warn_items_t>& result,
        QStringView web_server,
        QStringView year_,
        QStringView month_,
        QStringView day_,
        QStringView hour_
    ) const;


    //! Retrieves the data needed for the Speed statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param protocol_f The filter for the Protocol field
        \param method_f The filter for the Method field
        \param uri_f The filter for the URI field
        \param query_f The filter for the Query field
        \param response_f The filter for the Response field
        \throw LogDoctorException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
    */
    void getSpeedData(
        std::optional<stats_speed_items_t>& result,
        QStringView web_server,
        QStringView year_,
        QStringView month_,
        QStringView day_,
        QStringView protocol_f,
        QStringView method_f,
        QStringView uri_f,
        QStringView query_f,
        QStringView response_f
    ) const;


    //! Retrieves the data needed for the Counts statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param year The year
        \param month The month
        \param day The day
        \param log_field The log field
        \throw LogDoctorException
        \throw CrapviewException
        \throw DateTimeException
    */
    void getItemsCount(
        std::optional<stats_count_items_t>& result,
        QStringView web_server,
        QStringView year,
        QStringView month,
        QStringView day,
        QStringView log_field
    ) const;


    //! Retrieves the data needed for the Daytime statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param from_year_ The initial year
        \param from_month_ The initial month
        \param from_day_ The initial day
        \param to_year_ The final year
        \param to_month_ The final month
        \param to_day_ The final day
        \param log_field_ The log field to filter
        \param field_filter The filter to apply
        \throw LogDoctorException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
    */
    void getDaytimeCounts(
        std::optional<stats_day_items_t>& result,
        QStringView web_server,
        QStringView from_year_, QStringView from_month_, QStringView from_day_,
        QStringView to_year_,   QStringView to_month_,   QStringView to_day_,
        const LogField log_field_, QStringView field_filter
    ) const;


    //! Retrieves the data needed for the Relationsl statistics
    /*!
        Used when querying a single day
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param year_ The year
        \param month_ The month
        \param day_ The day
        \param log_field_1_ The first log field to filter
        \param field_filter_1 The filter to apply to the first field
        \param log_field_2_ The second log field to filter
        \param field_filter_2 The filter to apply to the second fiend
        \throw LogDoctorException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
        \see getRelationalCountsPeriod()
    */
    void getRelationalCountsDay(
        std::optional<stats_relat_items_t>& result,
        QStringView web_server,
        QStringView year_,        QStringView month_,         QStringView day_,
        const LogField log_field_1_, QStringView field_filter_1,
        const LogField log_field_2_, QStringView field_filter_2
    ) const;

    //! Retrieves the data needed for the Relational statistics
    /*!
        Used when querying a period of time
        \param result Holds the data only if the operation completed succssfully
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
        \throw LogDoctorException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
        \see getRelationalCountsDay()
    */
    void getRelationalCountsPeriod(
        std::optional<stats_relat_items_t>& result,
        QStringView web_server,
        QStringView from_year_,   QStringView from_month_,    QStringView from_day_,
        QStringView to_year_,     QStringView to_month_,      QStringView to_day_,
        const LogField log_field_1_, QStringView field_filter_1,
        const LogField log_field_2_, QStringView field_filter_2
    ) const;


    //! Retrieves the data needed for the Global statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param dates The dates to query
        \throw LogDoctorException
        \throw CrapviewException
        \throw ConversionException
    */
    void getGlobalCounts(
        std::optional<GlobalsData>& result,
        QStringView web_server,
        const stats_dates_t& dates
    ) const;


private:

    // leave TR::tr here to make them appear in the translatable phrases
    const std::string MSG_ERR_UNX_WS{ TR::tr("Unexpected WebServer").toStdString() };
    const std::string MSG_ERR_PROCESSING{ TR::tr("An error occured while processing").toStdString() };
    const std::string MSG_ERR_PROCESSING_DATES{ TR::tr("An error occured while processing dates").toStdString() };
    const std::string MSG_ERR_PARSING_YMD{ TR::tr("An error occured while parsing %1 from the database").toStdString() };
    const std::string WORD_YEARS{  TR::tr("Years").toStdString() };
    const std::string WORD_MONTHS{ TR::tr("Months").toStdString() };
    const std::string WORD_DAYS{   TR::tr("Days").toStdString() };
    const std::string MSG_RESPONSIBLE_VALUE{ TR::tr("Value responsible for the error").toStdString() };
    const std::string MSG_TABLE_NAME{ TR::tr("Database table name").toStdString() };

    // quantity of information to display throught dialogs
    DialogsLevel dialog_level{ DL_NORMAL };

    std::string db_path;
    QString db_name;

    // convert log fields to database fields
    const std::unordered_map<LogField, QString> LogFields_to_DbFields{
        {LogField::Warning,       "warning"},
        {LogField::Protocol,      "protocol"},
        {LogField::Method,        "method"},
        {LogField::Uri,           "uri"},
        {LogField::Query,         "query"},
        {LogField::ResponseCode,  "response"},
        {LogField::TimeTaken,     "time_taken"},
        {LogField::BytesSent,     "bytes_sent"},
        {LogField::BytesReceived, "bytes_received"},
        {LogField::Referrer,      "referrer"},
        {LogField::Client,        "client"},
        {LogField::UserAgent,     "user_agent"},
        {LogField::Cookie,        "cookie"}
    };

    //! Returns the database field corresponding to the relative log field
    /*!
        \param fld The log field
        \return The database field
        \throw CrapviewException
    */
    const QString& getDbField( const LogField fld ) const;

    //! Returns the database field corresponding to the relative log field
    /*!
        \param tr_fld The log field, hendles translated text
        \return The database field
        \throw CrapviewException
    */
    const QString& getDbField( QStringView tr_fld ) const;


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
        \throw DateTimeException
    */
    int getMonthNumber( QStringView month_str ) const;


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
        const int to_year, const int to_month, const int to_day
    );


    //! Returns the number of months in a given period
    /*!
        \param from_year The initial year
        \param from_month The initial month
        \param to_year The final Year
        \param to_month The final month
        \return The number of months in the period
    */
    static int countMonths(
        const int from_year, const int from_month,
        const int to_year,   const int to_month
    ) noexcept;
};


#endif // LOGDOCTOR__CRAPVIEW__MODULES__QUERY_H

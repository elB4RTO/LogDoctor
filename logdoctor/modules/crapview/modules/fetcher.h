#ifndef LOGDOCTOR__CRAPVIEW__MODULES__FETCHER_H
#define LOGDOCTOR__CRAPVIEW__MODULES__FETCHER_H

#include "main_lib.h"

#include "modules/shared.h"
#include "modules/security/path.h"

#include "modules/crapview/lib.h"
#include "modules/crapview/datatypes/fwd.h"

#include <unordered_map>
#include <optional>


class DateTime;


//! Fetcher
/*!
    Fetches data for the statistics from the database
*/
class Fetcher final
{
    CRAPVIEW_DATA_TYPEDEFS

public:

    explicit Fetcher() noexcept {}
    Q_DISABLE_COPY_MOVE(Fetcher)

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
    void setDbPath( PathHandler&& path ) noexcept;


    //! Refreshes the dates which are available in the database
    /*!
        \param result Holds the data only if the operation completed succssfully
        \throw VoidException
        \throw ConversionException
    */
    void fetchAllDates( std::optional<database_dates_t>& result ) noexcept;


    //! Retrieves the data needed for the Warnings statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param date The date to use for the wuery (may contain the hour)
        \throw VoidException
        \throw ConversionException
        \throw DateTimeException
    */
    void fetchWarningsData(
        std::optional<WarningData>& result,
        QStringView web_server,
        const DateTime& date
    ) const;


    //! Retrieves the data needed for the Speed statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param date The date to use for the wuery
        \param protocol_f The filter for the Protocol field
        \param method_f The filter for the Method field
        \param uri_f The filter for the URI field
        \param query_f The filter for the Query field
        \param response_f The filter for the Response field
        \param time_interval The time interval used to group the values
        \throw VoidException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
    */
    void fetchSpeedData(
        std::optional<SpeedData>& result,
        QStringView web_server,
        const DateTime& date,
        QStringView protocol_f,
        QStringView method_f,
        QStringView uri_f,
        QStringView query_f,
        QStringView response_f,
        const qint64 time_interval
    ) const;


    //! Retrieves the data needed for the Counts statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param date The date to use for the wuery
        \param log_field The log field
        \throw VoidException
        \throw CrapviewException
        \throw DateTimeException
    */
    void fetchCountsData(
        std::optional<CountData>& result,
        QStringView web_server,
        const DateTime& date,
        QStringView log_field
    ) const;


    //! Retrieves the data needed for the Daytime statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param from_date The initial date to use for the wuery
        \param date The final date to use for the wuery (may be invalid)
        \param log_field_ The log field to filter
        \param field_filter The filter to apply
        \throw VoidException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
    */
    void fetchDaytimeData(
        std::optional<DaytimeData>& result,
        QStringView web_server,
        const DateTime& from_date,
        DateTime to_date,
        const LogField log_field_, QStringView field_filter
    ) const;


    //! Retrieves the data needed for the Relationsl statistics
    /*!
        Used when querying a single day
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param date The date to use for the wuery
        \param log_field_1_ The first log field to filter
        \param field_filter_1 The filter to apply to the first field
        \param log_field_2_ The second log field to filter
        \param field_filter_2 The filter to apply to the second fiend
        \throw VoidException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
        \see getRelationalCountsPeriod()
    */
    void fetchRelationalDataDay(
        std::optional<RelationalData>& result,
        QStringView web_server,
        const DateTime& date,
        const LogField log_field_1_, QStringView field_filter_1,
        const LogField log_field_2_, QStringView field_filter_2
    ) const;

    //! Retrieves the data needed for the Relational statistics
    /*!
        Used when querying a period of time
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param from_date The date to use for the wuery
        \param to_date The date to use for the wuery
        \param log_field_1_ The first log field to filter
        \param field_filter_1 The filter to apply to the first field
        \param log_field_2_ The second log field to filter
        \param field_filter_2 The filter to apply to the second fiend
        \throw VoidException
        \throw CrapviewException
        \throw ConversionException
        \throw DateTimeException
        \see getRelationalCountsDay()
    */
    void fetchRelationalDataPeriod(
        std::optional<RelationalData>& result,
        QStringView web_server,
        const DateTime& from_date,
        DateTime to_date,
        const LogField log_field_1_, QStringView field_filter_1,
        const LogField log_field_2_, QStringView field_filter_2
    ) const;


    //! Retrieves the data needed for the Global statistics
    /*!
        \param result Holds the data only if the operation completed succssfully
        \param web_server The ID of the Web Server to use
        \param dates The dates to query
        \throw VoidException
        \throw CrapviewException
        \throw ConversionException
    */
    void fetchGlobalsData(
        std::optional<GlobalsData>& result,
        QStringView web_server,
        const stats_dates_t& dates
    ) const;


private:

    // quantity of information to display throught dialogs
    DialogsLevel dialog_level{ DL_NORMAL };

    PathHandler db_path;
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
};


#endif // LOGDOCTOR__CRAPVIEW__MODULES__FETCHER_H

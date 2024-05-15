#ifndef LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__QUERIES_H
#define LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__QUERIES_H

#include "modules/database/database.h"

#include "modules/crapview/utilities/datetime.h"


namespace QueryFactory
{

void allDates( QueryWrapper& query, QStringView table )
{
    query << QStringLiteral(R"(SELECT DISTINCT "year", "month", "day" FROM "%1" ORDER BY "year", "month", "day" ASC;)")
                .arg( table );
}

void warningsData( QueryWrapper& query, QStringView web_server, const DateTime& date )
{
    query << QStringLiteral(R"(SELECT * FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4)")
                .arg( web_server      )
                .arg( date.getYear()  )
                .arg( date.getMonth() )
                .arg( date.getDay()   );

    if ( date.isHourValid() ) {
        query << QStringLiteral(R"( AND "hour"=%1 ORDER BY "minute","second" ASC;)")
                    .arg( date.getHour() );
    } else {
        query << R"( ORDER BY "hour","minute","second" ASC;)";
    }
}

void speedData( QueryWrapper& query, QStringView web_server, const DateTime& date, QStringView protocol_f, QStringView method_f, QStringView uri_f, QStringView query_f, QStringView response_f )
{
    query << QStringLiteral(R"(SELECT "hour","minute","second","time_taken","uri","query","method","protocol","response" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4 AND "time_taken" IS NOT NULL)")
                .arg( web_server      )
                .arg( date.getYear()  )
                .arg( date.getMonth() )
                .arg( date.getDay()   );

    if ( ! protocol_f.isEmpty() ) {
        query << QStringLiteral(R"( AND "protocol")").append( protocol_f );
    }

    if ( ! method_f.isEmpty() ) {
        query << QStringLiteral(R"( AND "method")").append( method_f );
    }

    if ( ! uri_f.isEmpty() ) {
        query << QStringLiteral(R"( AND "uri")").append( uri_f );
    }

    if ( ! query_f.isEmpty() ) {
        query << QStringLiteral(R"( AND "query")").append( query_f );
    }

    if ( ! response_f.isEmpty() ) {
        query << QStringLiteral(R"( AND "response")").append( response_f );
    }

    query << R"( ORDER BY "hour","minute","second" ASC;)";
}

void countData( QueryWrapper& query, QStringView web_server, QStringView field, const DateTime& date )
{
    query << QStringLiteral(R"(SELECT "%1" FROM "%2" WHERE "%3" IS NOT NULL AND "year"=%4 AND "month"=%5 AND "day"=%6;)")
                .arg( field, web_server, field )
                .arg( date.getYear()  )
                .arg( date.getMonth() )
                .arg( date.getDay()   )
                .replace(QChar('\''),QLatin1String("''"));
}

void daytimeDataPartialMonth( QueryWrapper& query, QStringView web_server, const DateTime& from_date, const DateTime& to_date, QStringView field, QStringView field_filter )
{
    query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day">=%4 AND "day"<=%5)")
                .arg( web_server           )
                .arg( from_date.getYear()  )
                .arg( from_date.getMonth() )
                .arg( from_date.getDay()   )
                .arg( to_date.getDay()     );

    if ( ! field_filter.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( field, field_filter );
    }

    query << ";";
}

void daytimeDataFirstMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, const int day, QStringView field_filter )
{
    query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    query << QStringLiteral(R"( AND "day"<=%1)").arg( day );

    if ( ! field_filter.isEmpty() ) {
        query << field_filter;
    }

    query << ";";
}

void daytimeDataLastMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, const int day, QStringView field_filter )
{
    query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    query << QStringLiteral(R"( AND "day">=%1)").arg( day );

    if ( ! field_filter.isEmpty() ) {
        query << field_filter;
    }

    query << ";";
}

void daytimeDataEntireMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, QStringView field_filter )
{
    query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    if ( ! field_filter.isEmpty() ) {
        query << field_filter;
    }

    query << ";";
}

void relationalDataSingleDay( QueryWrapper& query, QStringView web_server, const DateTime& date, QStringView field_1, QStringView field_filter_1, QStringView field_2, QStringView field_filter_2 )
{
    query << QStringLiteral(R"(SELECT "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4)")
                .arg( web_server      )
                .arg( date.getYear()  )
                .arg( date.getMonth() )
                .arg( date.getDay()   );

    if ( ! field_filter_1.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( field_1, field_filter_1 );
    }

    if ( ! field_filter_2.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( field_2, field_filter_2 );
    }

    query << QStringLiteral(R"( ORDER BY "hour","minute" ASC;)");
}

void relationalDataPartialMonth( QueryWrapper& query, QStringView web_server, const DateTime& from_date, const DateTime& to_date, QStringView field_1, QStringView field_filter_1, QStringView field_2, QStringView field_filter_2 )
{
    query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day">=%4 AND "day"<=%5)")
                .arg( web_server           )
                .arg( from_date.getYear()  )
                .arg( from_date.getMonth() )
                .arg( from_date.getDay()   )
                .arg( to_date.getDay()     );

    if ( ! field_filter_1.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( field_1, field_filter_1 );
    }

    if ( ! field_filter_2.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( field_2, field_filter_2 );
    }

    query << R"( ORDER BY "day" ASC;)";

    query();
}

void relationalDataFirstMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, const int day, QStringView fields_filters )
{
    query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    query << QStringLiteral(R"( AND "day">=%1)").arg( day );

    if ( ! fields_filters.isEmpty() ) {
        query << fields_filters;
    }

    query << R"( ORDER BY "day" ASC;)";
}

void relationalDataLastMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, const int day, QStringView fields_filters )
{
    query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    query << QStringLiteral(R"( AND "day"<=%1)").arg( day );

    if ( ! fields_filters.isEmpty() ) {
        query << fields_filters;
    }

    query << R"( ORDER BY "day" ASC;)";
}

void relationalDataEntireMonth( QueryWrapper& query, QStringView web_server, const int year, const int month, QStringView fields_filters )
{
    query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                .arg( web_server )
                .arg( year ).arg( month );

    if ( ! fields_filters.isEmpty() ) {
        query << fields_filters;
    }

    query << R"( ORDER BY "day" ASC;)";
}

void globalsData( QueryWrapper& query, QStringView web_server, const int year, const int month )
{
    query << QStringLiteral(R"(SELECT "day","hour","protocol","method","uri","user_agent","time_taken","bytes_sent","bytes_received" FROM "%1" WHERE "year"=%2 AND "month"=%3 ORDER BY "day","hour" ASC;)")
            .arg( web_server )
            .arg( year ).arg( month );
}

} // namespace QueryFactory


#endif // LOGDOCTOR__CRAPVIEW__MODULES__FETCHER__QUERIES_H

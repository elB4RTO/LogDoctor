
#include "fetcher.h"

#include "fetcher/date_time.h"
#include "fetcher/utilities.h"

#include "modules/crapview/datatypes/fwd.h"

#include "modules/database/database.h"

#include "utilities/arrays.h"
#include "utilities/printables.h"
#include "utilities/strings.h"

#include <map>
#include <vector>


using namespace FetcherPrivate;


void Fetcher::setDialogLevel( const DialogsLevel new_level ) noexcept
{
    this->dialog_level = new_level;
}

void Fetcher::setDbPath( std::string&& path ) noexcept
{
    this->db_path = std::move(path);
    this->db_name = QString::fromStdString( this->db_path.substr( this->db_path.find_last_of( '/' ) + 1ul ) );
}


int Fetcher::getMonthNumber( QStringView month_str ) const
{
    for ( const auto& [num,str] : this->MONTHS ) {
        if ( TR::tr(str.c_str()) == month_str ) {
            return num;
        }
    }
    throw DateTimeException( "Unexpected Month name", month_str.toString().toStdString() );
}


int Fetcher::countMonths( QStringView from_year, QStringView from_month, QStringView to_year, QStringView to_month ) const
{
    const int from_year_{ toInt( from_year ) },
              from_month_{ this->getMonthNumber( from_month ) };

    return ::countMonths(
        toInt( from_year ),
        this->getMonthNumber( from_month ),
        to_year.isEmpty() ? from_year_ : toInt( to_year ),
        to_month.isEmpty() ? from_month_ : this->getMonthNumber( to_month )
    );
}


const QString& Fetcher::getDbField( const LogField fld ) const
{
    return this->LogFields_to_DbFields.at( fld );
}

const QString& Fetcher::getDbField( QStringView tr_fld ) const
{
    for ( const auto& [id,str] : this->FIELDS ) {
        if ( TR::tr(str.c_str()) == tr_fld ) {
            return this->LogFields_to_DbFields.at( id );
        }
    }
    throw DatabaseException( std::move(QStringLiteral("Unexpected DbField: ").append(tr_fld)) );
}



// get a fresh map of available dates
void Fetcher::fetchAllDates( std::optional<database_dates_t>& result ) noexcept
{
    database_dates_t dates{ // std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
        {WS_APACHE, {}}, {WS_NGINX, {}}, {WS_IIS, {}}
    };

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    // recursively query years, months and days for every WebServer
    static const std::vector<std::tuple<WebServer, QString>> tables{
        std::make_tuple( WS_APACHE, QStringLiteral("apache") ),
        std::make_tuple( WS_NGINX,  QStringLiteral("nginx")  ),
        std::make_tuple( WS_IIS,    QStringLiteral("iis")    )
    };

    for ( const auto& [ws,tbl] : tables ) {

        QueryWrapper query{ db.getQuery() };

        query( QStringLiteral(R"(SELECT DISTINCT "year", "month", "day" FROM "%1" ORDER BY "year", "month", "day" ASC;)").arg(tbl) );

        auto& years = dates.at( ws );

        while ( query->next() ) {
            years.try_emplace( toInt( query[0] ), std::map<int, std::vector<int>>{} )
                 .first->second
                 .try_emplace( toInt( query[1] ), std::vector<int>{} )
                 .first->second
                 .emplace_back( toInt( query[2] ) );
        }
    }

    result.emplace( std::move(dates) );
}


// get daytime values for the warnings
void Fetcher::fetchWarningsData( std::optional<WarningData>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, QStringView hour_ ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    // setup period limits
    const int year{  toInt( year_ ) };
    const int month{ this->getMonthNumber( month_ ) };
    const int day{   toInt( day_ ) };
    const int hour{  hour_.isEmpty() ? -1 : toInt( hour_ ) };

    QueryWrapper query{ db.getQuery() };

    query << QStringLiteral(R"(SELECT * FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4)")
                .arg( web_server )
                .arg( year ).arg( month ).arg( day );

    WarningData data;

    if ( hour == -1 ) {
        data.setTimelineAsDay();

        query << R"( ORDER BY "hour","minute","second" ASC;)";

    } else {
        data.setTimelineAsHour();

        query << QStringLiteral(R"( AND "hour"=%1 ORDER BY "minute","second" ASC;)")
                    .arg( hour );
    }

    query();

    if ( data.buildFromQuery( query ) ) {
        result.emplace( std::move(data) );
    }
}


// get day-time values for the time-taken field
void Fetcher::fetchSpeedData( std::optional<SpeedData>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, QStringView protocol_f, QStringView method_f, QStringView uri_f, QStringView query_f, QStringView response_f, const qint64 time_interval ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int year{  toInt( year_ ) };
    const int month{ this->getMonthNumber( month_ ) };
    const int day{   toInt( day_ ) };

    QueryWrapper query{ db.getQuery() };

    query << QStringLiteral(R"(SELECT "hour","minute","second","time_taken","uri","query","method","protocol","response" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4 AND "time_taken" IS NOT NULL)")
                .arg( web_server )
                .arg( year ).arg( month ).arg( day );

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

    query();

    TimeManager time{ year, month, day, time_interval };

    SpeedData data;
    if ( data.buildFromQuery( query, std::move(time) ) ) {
        result.emplace( std::move(data) );
    }
}



// get, group and count identical items of a specific field in a date
void Fetcher::fetchCountsData( std::optional<CountData>& result, QStringView web_server, QStringView year, QStringView month, QStringView day, QStringView log_field ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    QueryWrapper query{ db.getQuery() };

    query << QStringLiteral(R"(SELECT "%1" FROM "%2" WHERE "%3" IS NOT NULL AND "year"=%4 AND "month"=%5 AND "day"=%6;)")
                .arg( this->getDbField( log_field ),
                      web_server,
                      this->getDbField( log_field ),
                      year,
                      QString::number( this->getMonthNumber( month ) ),
                      day )
                .replace(QChar('\''),QLatin1String("''"));

    query();

    CountData data;

    if ( data.buildFromQuery( query ) ) {
        result.emplace( std::move(data) );
    }
}



// get and count items with a 10 minutes gap for every hour of the day
void Fetcher::fetchDaytimeData( std::optional<DaytimeData>& result, QStringView web_server, QStringView from_year_, QStringView from_month_, QStringView from_day_, QStringView to_year_, QStringView to_month_, QStringView to_day_, const LogField log_field_, QStringView field_filter ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int from_year{  toInt( from_year_ ) };
    const int from_month{ this->getMonthNumber( from_month_ ) };
    const int from_day{   toInt( from_day_ ) };
    const int to_year{    to_year_.isEmpty()  ? from_year  : toInt( to_year_ )  };
    const int to_month{   to_month_.isEmpty() ? from_month : this->getMonthNumber( to_month_ )  };
    const int to_day{     to_day_.isEmpty()   ? from_day   : toInt( to_day_ )  };

    const QString& log_field{ this->getDbField( log_field_ ) };

    int n_days   { 0 },
        n_months { ::countMonths( from_year, from_month, to_year, to_month ) };

    int year  { from_year },
        month { from_month };

    DaytimeData data;

    if ( n_months == 1 ) {
        QueryWrapper query{ db.getQuery() };

        query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day">=%4 AND "day"<=%5)")
                    .arg( web_server )
                    .arg( year ).arg( month )
                    .arg( from_day ).arg( to_day );

        if ( ! field_filter.isEmpty() ) {
            query << QStringLiteral(R"( AND "%1"%2)").arg( log_field, field_filter );
        }

        query << ";";

        query();

        n_days += data.buildFromQuery( query );

    } else {
        for ( int m{1}; m<=n_months; ++m ) {

            QueryWrapper query{ db.getQuery() };

            query << QStringLiteral(R"(SELECT "day", "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                        .arg( web_server )
                        .arg( year ).arg( month );

            if ( m == 1 ) {
                // first month, only get the days starting from the beginning day
                query << QStringLiteral(R"( AND "day">=%1)").arg( from_day );
            } else if ( m == n_months ) {
                // last month, only get the days until the ending day
                query << QStringLiteral(R"( AND "day"<=%1)").arg( to_day );
            }

            if ( ! field_filter.isEmpty() ) {
                query << QStringLiteral(R"( AND "%1"%2)").arg( log_field, field_filter );
            }

            query << ";";

            query();

            n_days += data.buildFromQuery( query );

            if ( ++month > 12 ) {
                month = 1;
                ++year;
            }
        }
    }
    if ( n_days == 0 ) {
        return;
    }

    data.adjustCounts( n_days );

    result.emplace( std::move(data) );
}



// get and count how many times a specific item value brought to another
void Fetcher::fetchRelationalDataDay( std::optional<RelationalData>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, const LogField log_field_1_, QStringView field_filter_1, const LogField log_field_2_, QStringView field_filter_2 ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int year{  toInt( year_ ) };
    const int month{ this->getMonthNumber( month_ ) };
    const int day{   toInt( day_ ) };

    const QString& log_field_1{ this->getDbField( log_field_1_ ) };
    const QString& log_field_2{ this->getDbField( log_field_2_ ) };

    QueryWrapper query{ db.getQuery() };

    query << QStringLiteral(R"(SELECT "hour", "minute" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4)")
                .arg( web_server )
                .arg( year ).arg( month ).arg( day );

    if ( ! field_filter_1.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( log_field_1, field_filter_1 );
    }

    if ( ! field_filter_2.isEmpty() ) {
        query << QStringLiteral(R"( AND "%1"%2)").arg( log_field_2, field_filter_2 );
    }

    query << QStringLiteral(R"( ORDER BY "hour","minute" ASC;)");

    query();

    const qint64 time_interval_mins{ 20 };
    const qint64 time_interval_secs{ time_interval_mins * 60 };

    TimeManager time{ year, month, day, time_interval_secs };

    RelationalData data;
    if ( data.buildFromQuery( query, std::move(time) ) ) {
        result.emplace( std::move(data) );
    }
}



void Fetcher::fetchRelationalDataPeriod( std::optional<RelationalData>& result, QStringView web_server, QStringView from_year_, QStringView from_month_, QStringView from_day_, QStringView to_year_, QStringView to_month_, QStringView to_day_, const LogField log_field_1_, QStringView field_filter_1, const LogField log_field_2_, QStringView field_filter_2 ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int from_year{  toInt( from_year_ ) };
    const int from_month{ this->getMonthNumber( from_month_ ) };
    const int from_day{   toInt( from_day_ ) };
    const int to_year{    to_year_.isEmpty()  ? from_year  : toInt( to_year_ ) };
    const int to_month{   to_month_.isEmpty() ? from_month : this->getMonthNumber( to_month_ ) };
    const int to_day{     to_day_.isEmpty()   ? from_day   : toInt( to_day_ ) };

    const QString& log_field_1{ this->getDbField( log_field_1_ ) };
    const QString& log_field_2{ this->getDbField( log_field_2_ ) };

    const int n_months{ ::countMonths( from_year, from_month, to_year, to_month ) };

    int year  { from_year  },
        month { from_month };

    if ( n_months == 1 ) {
        // 1 month, no need to loop
        QueryWrapper query{ db.getQuery() };

        query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day">=%4 AND "day"<=%5)")
                    .arg( web_server )
                    .arg( year ).arg( month )
                    .arg( from_day ).arg( to_day );

        if ( ! field_filter_1.isEmpty() ) {
            query << QStringLiteral(R"( AND "%1"%2)").arg( log_field_1, field_filter_1 );
        }

        if ( ! field_filter_2.isEmpty() ) {
            query << QStringLiteral(R"( AND "%1"%2)").arg( log_field_2, field_filter_2 );
        }

        query << R"( ORDER BY "day" ASC;)";

        query();

        DateManager date{ from_year, from_month, from_day };

        const auto last_date{ QDate( to_year, to_month, to_day ).addDays( 1 ) };

        RelationalData data;
        if ( data.buildFromQuery( query, std::move(date), std::move(last_date) ) ) {
            result.emplace( std::move(data) );
        }


    } else {
        DateManager date{ from_year, from_month, from_day };

        RelationalData data;
        data.reserveSpace( countDays( from_year, from_month, from_day, to_year, to_month, to_day ) * 2 );
        bool no_data{ false };

        const QString query_filters{ QStringLiteral("%1%2").arg(
            !field_filter_1.isEmpty() ? QStringLiteral(R"( AND "%1"%2)").arg( log_field_1, field_filter_1 ) : QString(),
            !field_filter_2.isEmpty() ? QStringLiteral(R"( AND "%1"%2)").arg( log_field_2, field_filter_2 ) : QString())
        };

        data.appendFirstEmpty( date );

        for ( int m{1}; m<=n_months; ++m ) {

            const int first_day{ m==1 ? from_day : 1 };
            const int last_day{ m==n_months ? to_day : getMonthDays( year, month ) };

            QueryWrapper query{ db.getQuery() };

            query << QStringLiteral(R"(SELECT "day" FROM "%1" WHERE "year"=%2 AND "month"=%3)")
                        .arg( web_server )
                        .arg( year ).arg( month );

            if ( m == 1 ) {
                // first month, only get the day from the beginning day
                query << QStringLiteral(R"( AND "day">=%1)").arg( from_day );
            } else if ( m == n_months ) {
                // last month, only get the days until the ending day
                query << QStringLiteral(R"( AND "day"<=%1)").arg( to_day );
            }

            if ( ! query_filters.isEmpty() ) {
                query << query_filters;
            }

            query << R"( ORDER BY "day" ASC;)";

            query();

            const QDate initial_date{ year, month, first_day };
            const QDate final_date{   year, month, last_day  };

            no_data |= data.appendFromQuery( query, date, std::move(initial_date), std::move(final_date), last_day );

            // increase the month
            if ( ++month > 12 ) {
                month = 1;
                ++year;
            }
        }
        if ( no_data ) {
            return;
        }

        const auto last_date{ QDate( to_year, to_month, to_day ).addDays( 1 ) };

        data.appendLastEmpty( date, std::move(last_date) );
    }
}



void Fetcher::fetchGlobalsData( std::optional<GlobalsData>& result, QStringView web_server, const stats_dates_t& dates ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, DB_READONLY ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    bool no_data{ true };
    int max_date_year{0}, max_date_month{0}, max_date_day{0};
    double n_days{0.0};
    size_t max_date_count{0};
    std::array<double, 7> week_days_count{ 0, 0, 0, 0, 0, 0, 0 };

    GlobalsData data;

    const auto week_day_from{
        [](const int y, const int m, const int d)->size_t
        { return static_cast<size_t>( QDate(y,m,d).dayOfWeek()-1 ); }
    };

    const auto update_perf{
        [](Perfs& perf, const int val)
        {
            if ( val >= 0 ) {
                if ( const size_t v{static_cast<size_t>(val)}; v > 0ul) [[likely]] {
                    if ( v > perf.max ) {
                        perf.max = v;
                    }
                    perf.total += v;
                }
                ++ perf.count;
            }
        }
    };

    for ( const auto& [year, dates_] : dates ) {
        for ( const auto& [month, dates__] : dates_ ) {

            int d{-1}, h{-1}, tt{-1}, bs{-1}, br{-1},
                day{-1}, hour{-1};
            double hour_count{0};
            size_t day_count{0};
            QString protocol, method, uri, user_agent;

            QueryWrapper query{ db.getQuery() };

            query( QStringLiteral(R"(SELECT "day","hour","protocol","method","uri","user_agent","time_taken","bytes_sent","bytes_received" FROM "%1" WHERE "year"=%2 AND "month"=%3 ORDER BY "day","hour" ASC;)")
                        .arg( web_server )
                        .arg( year ).arg( month ) );

            if ( query.size() == 0ul ) {
                // no data in this month
                continue;
            }
            no_data &= false;

            while ( query->next() ) {

                // day
                if ( ! query[0].isNull() ) {
                    d = toInt( query[0] );
                }
                // hour
                if ( ! query[1].isNull() ) {
                    h = toInt( query[1] );
                }
                // protocol
                if ( ! query[2].isNull() ) {
                    protocol = toString( query[2] );
                }
                // method
                if ( ! query[3].isNull() ) {
                    method = toString( query[3] );
                }
                // uri
                if ( ! query[4].isNull() ) {
                    uri = toString( query[4] );
                }
                // user agent
                if ( ! query[5].isNull() ) {
                    user_agent = toString( query[5] );
                }
                // time taken
                if ( ! query[6].isNull() ) {
                    tt = toInt( query[6] );
                }
                // bytes sent
                if ( ! query[7].isNull() ) {
                    bs = toInt( query[7] );
                }
                // bytes received
                if ( ! query[8].isNull() ) {
                    br = toInt( query[8] );
                }

                // process the day count
                if ( d > 0 ) {
                    if ( day == -1 ) {
                        day = d;
                    }
                    if ( d == day ) {
                        ++ day_count;
                    } else {
                        ++ n_days;
                        // sum the day count to the total count
                        data.req_count += day_count;
                        // sum the day count to the relative day of the week count
                        const size_t week_day{ week_day_from(year,month,day) };
                        data.traf.day[ week_day ] += static_cast<double>(day_count);
                        ++ week_days_count[ week_day ];
                        // check the max date count
                        if ( day_count > max_date_count ) {
                            max_date_count = day_count;
                            max_date_year = year;
                            max_date_month = month;
                            max_date_day = day;
                        }
                        day_count = 1;
                        day = d;
                    }
                }

                // process the hour count
                if ( h >= 0 ) {
                    if ( hour == -1 ) {
                        hour = h;
                    }
                    if ( h == hour ) {
                        ++ hour_count;
                    } else {
                        data.traf.hour[ hour ] += hour_count;
                        hour_count = 1;
                        hour = h;
                    }
                }

                // sum the time taken
                update_perf( data.perf.time_taken, tt );

                // sum the bytes sent
                update_perf( data.perf.bytes_sent, bs );

                // sum the bytes received
                update_perf( data.perf.bytes_recv, br );

                // process the protocol
                if ( ! protocol.isEmpty() ) {
                    ++ data.recurs.protocol[ protocol ];
                }

                // process the method
                if ( ! method.isEmpty() ) {
                    ++ data.recurs.method[ method ];
                }

                // process the uri
                if ( ! uri.isEmpty() ) {
                    ++ data.recurs.uri[ uri ];
                }

                // process the user-agent
                if ( ! user_agent.isEmpty() ) {
                    ++ data.recurs.user_agent[ user_agent ];
                }
            }

            // complete the remaining stats

            // append the last hour
            if ( hour >= 0 ) {
                data.traf.hour[ hour ] += hour_count;
            }

            // sum the day count to the total count
            data.req_count += day_count;

            // sum the day count to the relative day of the week count
            const size_t week_day{ week_day_from(year,month,day) };
            data.traf.day[ week_day ] += static_cast<double>(day_count);
            ++ week_days_count[ week_day ];

            // check the max date count
            if ( day_count > max_date_count ) {
                max_date_count = day_count;
                max_date_year = year;
                max_date_month = month;
                max_date_day = day;
            }
        }
    }
    if ( no_data ) {
        return;
    }

    // finally process some of the values

    // process the hours of the day
    if ( n_days > 0.0 ) {
        std::transform( data.traf.hour.cbegin(), data.traf.hour.cend(), data.traf.hour.begin(),
                        [n_days](const double count){ return count / n_days; } );
    }

    // process the day of the week
    /*std::ranges::for_each( std::views::zip( data.traf.day, week_days_count ),
        [](auto tc){ if (auto& [t,c]{tc}; c>0.0){ t/=c; } });*/
    for ( auto [total,count] : ArrayOps::zip( data.traf.day, week_days_count ) ) {
        if ( count > 0.0 ) {
            total /= count;
        }
    }

    // make the max-date tuple
    data.traf.date = std::make_tuple(
        PrintSec::printableDate( max_date_year, max_date_month, max_date_day ),
        QString::number( max_date_count ) );

    result.emplace( std::move(data) );
}



#include "query.h"

#include "modules/database/database.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "utilities/printables.h"
#include "utilities/strings.h"

#include <QDateTime>

#include <map>
#include <vector>
#include <ranges>


int toInt( const QString& str )
{
    bool ok;
    const int result{ str.toInt( &ok ) };
    if ( ! ok ) {
        DialogSec::errConvertingData(
            QStringLiteral("QString"),
            QStringLiteral("int"),
            str );
        throw LogDoctorException{};
    }
    return result;
}
int toInt( QStringView str )
{
    bool ok;
    const int result{ str.toInt( &ok ) };
    if ( ! ok ) {
        DialogSec::errConvertingData(
            QStringLiteral("QStringView"),
            QStringLiteral("int"),
            str.toString() );
        throw LogDoctorException{};
    }
    return result;
}
int toInt( const QVariant& v )
{
    if ( ! v.canConvert( QMetaType(QMetaType::Int) ) ) {
        DialogSec::errConvertingData(
            QStringLiteral("QVariant"),
            QStringLiteral("int"),
            v.toString() );
        throw LogDoctorException{};
    }
    return v.toInt();
}

QString toString( const QVariant& v )
{
    if ( ! v.canConvert( QMetaType(QMetaType::QString) ) ) {
        DialogSec::errConvertingData(
            QStringLiteral("QVariant"),
            QStringLiteral("QString"),
            v.toString() );
        throw LogDoctorException{};
    }
    return v.toString();
}


void DbQuery::setDialogLevel( const DialogsLevel new_level ) noexcept
{
    this->dialog_level = new_level;
}

void DbQuery::setDbPath( std::string&& path ) noexcept
{
    this->db_path = std::move(path);
    this->db_name = QString::fromStdString( this->db_path.substr( this->db_path.find_last_of( '/' ) + 1ul ) );
}


int DbQuery::getMinuteGap( const int minute, const int gap )
{
    int m{ -1 };
    if ( minute < 0 || minute >= 60 ) {
        // unexpected value
        throw DateTimeException( "Unexpected Minute: " + std::to_string( minute ) );
    }
    int n{ 0 };
    for ( int g{0}; g<60; g+=gap ) {
        if ( minute >= g && minute < g+gap ) {
            m = gap * n;
            break;
        }
        ++n;
    }
    return m;
}

int DbQuery::getMonthDays( const int year, const int month )
{
    int n_days;
    switch (month) {
        case 1:  n_days = 31; break;
        case 2:  n_days = ( year%4 == 0 ) ? 29 : 28 ; break;
        case 3:  n_days = 31; break;
        case 4:  n_days = 30; break;
        case 5:  n_days = 31; break;
        case 6:  n_days = 30; break;
        case 7:  n_days = 31; break;
        case 8:  n_days = 31; break;
        case 9:  n_days = 30; break;
        case 10: n_days = 31; break;
        case 11: n_days = 30; break;
        case 12: n_days = 31; break;
        default:
            // unexpected month
            throw DateTimeException( "Unexpected Month number: " + std::to_string( month ) );
    }
    return n_days;
}


int DbQuery::getMonthNumber( QStringView month_str ) const
{
    for ( const auto& [num,str] : this->MONTHS ) {
        if ( TR::tr(str.c_str()) == month_str ) {
            return num;
        }
    }
    throw DateTimeException( "Unexpected Month name: " + month_str.toString().toStdString() );
}


int DbQuery::countDays( const int from_year, const int from_month, const int from_day, const int to_year, const int to_month, const int to_day )
{
    int n_days{ 1 };
    if ( from_year == to_year ) {
        // 1 year
        if ( from_month == to_month ) {
            // 1 month
            n_days += to_day - from_day + 1;
        } else {
            n_days += getMonthDays( from_year, from_month ) - from_day; // first month's days
            for ( int month=from_month+1; month<to_month; ++month ) {
                n_days += getMonthDays( from_year, month );
            }
            n_days += to_day; // last month's days
        }
    } else {
        n_days += getMonthDays( from_year, from_month ) - from_day;
        if ( from_month < 12 ) {
            for ( int month{from_month+1}; month<=12; ++month ) {
                n_days += getMonthDays( from_year, month );
            }
        }
        for ( int year{from_year+1}; year<=to_year; ++year ) {
            int last_month{ 12 };
            if ( year == to_year ) {
                last_month = to_month-1;
                n_days += to_day; // last month's days, added in advance
            }
            for ( int month{1}; month<=last_month; ++month ) {
                n_days += getMonthDays( year, month );
            }
        }
    }
    return n_days;
}

int DbQuery::countMonths( const int from_year, const int from_month, const int to_year, const int to_month ) noexcept
{
    int n_months{ 0 };
    if ( from_year == to_year ) {
        // same year
        if ( from_month == to_month ) {
            // same month
            n_months = 1;
        } else {
            // different months
            n_months = to_month - from_month + 1;
        }
    } else {
        // different years
        n_months += 13 - from_month; // months to the end of the first year
        n_months += to_month; // months from the beginning of the last year
        n_months += 12 * ( to_year - from_year - 1 ); // 12 months for every middle year (0 if none)
    }
    return n_months;
}

int DbQuery::countMonths( QStringView from_year, QStringView from_month, QStringView to_year, QStringView to_month ) const
{
    const int from_year_{ toInt( from_year ) },
              from_month_{ this->getMonthNumber( from_month ) };

    return this->countMonths(
        toInt( from_year ),
        this->getMonthNumber( from_month ),
        to_year.isEmpty() ? from_year_ : toInt( to_year ),
        to_month.isEmpty() ? from_month_ : this->getMonthNumber( to_month )
    );
}


const QString& DbQuery::getDbField( const LogField fld ) const
{
    return this->LogFields_to_DbFields.at( fld );
}

const QString& DbQuery::getDbField( QStringView tr_fld ) const
{
    for ( const auto& [id,str] : this->FIELDS ) {
        if ( TR::tr(str.c_str()) == tr_fld ) {
            return this->LogFields_to_DbFields.at( id );
        }
    }
    throw DatabaseException( std::move(QStringLiteral("Unexpected DbField: ").append(tr_fld)) );
}



// get a fresh map of available dates
void DbQuery::refreshDates( std::optional<database_dates_t>& result ) noexcept
{
    database_dates_t dates{ // std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
        {11, {}}, {12, {}}, {13, {}}
    };

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    // recursively query years, months and days for every WebServer
    static const std::vector<std::tuple<int, QString>> tables{
        std::make_tuple(11,"apache"),
        std::make_tuple(12,"nginx"),
        std::make_tuple(13,"iis") };

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
void DbQuery::getWarningsData( std::optional<stats_warn_items_t>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, QStringView hour_ ) const
{
    stats_warn_items_t items; // std::vector<std::vector<std::vector<std::array<QString,18>>>>

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    // setup period limits
    const int year{  toInt( year_ ) };
    const int month{ this->getMonthNumber( month_ ) };
    const int day{   toInt( day_ ) };
    const int hour{  hour_.isEmpty() ? -1 : toInt( hour_ ) };

    const auto from_query_data{
        [](const QueryWrapper& query)->std::array<QString,18>
        {
            return {
                toString(query[0]), toString(query[1]), toString(query[2]), // year, month, day
                toString(query[3]), toString(query[4]), toString(query[5]), // hour, minute, second
                toString(query[6]), toString(query[7]), // protocol, method
                toString(query[8]), toString(query[9]), // uri, query
                toString(query[10]), // response
                toString(query[15]), // user agent
                toString(query[14]), // client
                toString(query[16]), // cookie
                toString(query[17]), // referer
                toString(query[13]), // bytes received
                toString(query[12]), // bytes sent
                toString(query[11])  // time taken
            };
        }
    };

    QueryWrapper query{ db.getQuery() };

    query << QStringLiteral(R"(SELECT * FROM "%1" WHERE "year"=%2 AND "month"=%3 AND "day"=%4)")
                .arg( web_server )
                .arg( year ).arg( month ).arg( day );

    if ( hour == -1 ) {
        // entire day
        items.reserve( 24ul );
        for ( size_t h{0ul}; h<24ul; ++h ) {
            items.emplace_back( std::vector<std::vector<std::array<QString,18>>>{} );
            auto& aux{ items.at( h ) };
            aux.reserve( 6ul );
            for ( int m{0}; m<60; m+=10 ) {
                aux.emplace_back( std::vector<std::array<QString,18>>{} );
            }
        }

        query << R"( ORDER BY "hour","minute","second" ASC;)";

        query();

        while ( query->next() ) {
            // append the line
            items.at( static_cast<size_t>( toInt( query[3] ) ) )
                 .at( static_cast<size_t>( this->getMinuteGap( toInt( query[4] ) )/10 ) )
                 .push_back( from_query_data( query ) );
        }

    } else {
        // 1 hour
        items.reserve( 6ul );
        for ( size_t g{0ul}; g<6ul; ++g ) {
            items.emplace_back( std::vector<std::vector<std::array<QString,18>>>{} );
            auto& aux{ items.at( g ) };
            aux.reserve( 10ul );
            for ( int m{0}; m<10; ++m ) {
                aux.emplace_back( std::vector<std::array<QString,18>>{} );
            }
        }

        query << QStringLiteral(R"( AND "hour"=%5 ORDER BY "minute","second" ASC;)")
                    .arg( hour );

        query();

        while ( query->next() ) {
            // append the line
            const int min{ toInt( query[4] ) };
            items.at( static_cast<size_t>( this->getMinuteGap( min )/10 ) )
                 .at( static_cast<size_t>( min % 10 ) )
                 .push_back( from_query_data( query ) );
        }
    }

    result.emplace( std::move(items) );
}


// get day-time values for the time-taken field
void DbQuery::getSpeedData( std::optional<stats_speed_items_t>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, QStringView protocol_f, QStringView method_f, QStringView uri_f, QStringView query_f, QStringView response_f ) const
{
    stats_speed_items_t data; // std::vector<std::tuple<long long, std::array<QString,6>>>

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

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

    if ( const size_t size{ query.size() }; size > 0ul ) {
        data.reserve( size * 3 );
    } else {
        return;
    }

    int h, m, s;
    const auto prev_instant{
        [&h,&m,&s](const int hour, const int minute, const int second)
        {
            h=hour; m=minute; s=second;
            if ( --s < 0 ) { s=59;
            if ( --m < 0 ) { m=59;
            if ( --h < 0 ) { h=m=s=0; }}}
        }
    };
    const auto next_instant{
        [&h,&m,&s](const int hour, const int minute, const int second)
        {
            h=hour; m=minute; s=second;
            if ( ++s > 59 ) { s=0;
            if ( ++m > 59 ) { m=0;
            if ( ++h > 23 ) { h=23;m=59;s=59; }}}
        }
    };

    using data_t = std::array<QString,6>;

    QDateTime time{
        QDate( year, month, day ),
        QTime( 0, 0, 0 )
    };

    const auto push_empty{
        [&data,&time]()
        {
            data.emplace_back( time.toMSecsSinceEpoch(), data_t{} );
        }
    };
    const auto push_data{
        [&data,&time](const auto ...args)
        {
            data.emplace_back( time.toMSecsSinceEpoch(), data_t{args...} );
        }
    };

    const auto set_time{
        [&time](const auto ...args)
        {
            time.setTime( QTime(args...) );
        }
    };

    int hour{-1},  next_hour,   prev_hour{0},
        minute{0}, next_minute, prev_minute{0},
        second{0}, next_second, prev_second{0};
    QString tt, ur, qr, mt, pt, rs;

    // append the first ficticious count
    time.setMSecsSinceEpoch( time.toMSecsSinceEpoch() - 1000 ); // -1s
    push_empty();
    time.setMSecsSinceEpoch( time.toMSecsSinceEpoch() + 1000 ); // +1s

    while ( query->next() ) {
        next_hour   = toInt( query[0] );
        next_minute = toInt( query[1] );
        next_second = toInt( query[2] );

        if ( next_hour == hour && next_minute == minute && next_second == second ) {
            set_time( hour, minute, second );
            push_data( tt,ur,qr,mt,pt,rs );
        } else {
            if ( next_hour == hour ) {
                prev_instant( hour, minute, second );
                // append the second before the last one found, if it is not equal to the prev
                if ( prev_hour < h || prev_minute < m || prev_second < s ) {
                    set_time( h, m, s );
                    push_empty();
                }
                // same hour new minute/second, append the last count
                set_time( hour, minute, second );
                push_data( tt,ur,qr,mt,pt,rs );
                // append the second after the last one found, if it is not equal to the next
                next_instant( hour, minute, second );
                if ( next_hour > h || next_minute > m || next_second > s ) {
                    set_time( h, m, s );
                    push_empty();
                }
                prev_hour = hour; // update now to avoid getting next_hour's value
            } else {
                // minute & second are always different when the hour is different
                if ( hour >= 0 ) {
                    // append the prev as zero
                    prev_instant( hour, minute, second );
                    if ( prev_hour < h || prev_minute < m || prev_second < s ) {
                        set_time( h, m, s );
                        push_empty();
                    }
                    // apend the last p count if not in the first round of the loop
                    set_time( hour, minute, second );
                    push_data( tt,ur,qr,mt,pt,rs );
                    // append the next as zero
                    next_instant( hour, minute, second );
                    if ( next_hour > h || next_minute > m || next_second > s ) {
                        set_time( h, m, s );
                        push_empty();
                    }
                } else {
                    // hout < 0 only in the first round of the loop
                    // append the second 0 of the day, if it is not the one found
                    if ( next_hour > 0 || next_minute > 0 || next_second > 0 ) {
                        push_empty();
                        // append the second before the first found
                        prev_instant( next_hour, next_minute, next_second );
                        if ( h > 0 || m > 0 || s > 0 ) {
                            set_time( h, m, s );
                            push_empty();
                        }
                    }
                }
                prev_hour = hour;
                hour = next_hour;
            }
            prev_minute = minute;
            minute = next_minute;
            prev_second = second;
            second = next_second;
        }
        tt = toString( query[3] ); // time taken
        ur = toString( query[4] ); // uri
        qr = toString( query[5] ); // query
        mt = toString( query[6] ); // method
        pt = toString( query[7] ); // protocol
        rs = toString( query[8] ); // response
    }
    // last one, append the prev count
    prev_instant( hour, minute, second );
    if ( prev_hour < h || prev_minute < m || prev_second < s ) {
        set_time( h, m, s );
        push_empty();
    }
    // append the last count
    set_time( hour, minute, second );
    push_data( tt,ur,qr,mt,pt,rs );
    if ( hour < 23 && minute < 59 && second < 59 ) {
        // append 1 second after the last
        next_instant( hour, minute, second );
        set_time( h, m, s );
        push_empty();
    }
    // append the last fictitious count
    time.setTime( QTime( 23, 59, 59 ) );
    time.setMSecsSinceEpoch( time.toMSecsSinceEpoch() + 1000 ); // +1s
    push_empty();

    if ( data.capacity() > data.size() ) {
        data.shrink_to_fit();
    }

    result.emplace( std::move(data) );
}



// get, group and count identical items of a specific field in a date
void DbQuery::getItemsCount( std::optional<stats_count_items_t>& result, QStringView web_server, QStringView year, QStringView month, QStringView day, QStringView log_field ) const
{
    QHash<QString, unsigned> aux_items;
    stats_count_items_t items; // std::map<QString, unsigned int>>

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

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

    if ( const size_t size{ query.size() }; size > 0ul ) {
        aux_items.reserve( size * 3 );
    } else {
        return;
    }

    while ( query->next() ) {
        const QString item{ toString( query[0] ) };
        if ( ! item.isEmpty() ) {
            ++ aux_items[ item ];
        }
    }

    // morph tha QHash into an ordered map
    QHashIterator iter{ aux_items };
    while ( iter.hasNext() ) {
        iter.next();
        items.emplace( iter.value(), iter.key() );
    }

    result.emplace( std::move(items) );
}



// get and count items with a 10 minutes gap for every hour of the day
void DbQuery::getDaytimeCounts( std::optional<stats_day_items_t>& result, QStringView web_server, QStringView from_year_, QStringView from_month_, QStringView from_day_, QStringView to_year_, QStringView to_month_, QStringView to_day_, const LogField log_field_, QStringView field_filter ) const
{
    stats_day_items_t data{ // std::unordered_map<int, std::unordered_map<int, int>>
        {0,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {1,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {2,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {3,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {4,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {5,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {6,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {7,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {8,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {9,  {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {10, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {11, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {12, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {13, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {14, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {15, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {16, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {17, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {18, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {19, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {20, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {21, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
        {22, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},  {23, {{0,0},{10,0},{20,0},{30,0},{40,0},{50,0}}},
    };

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int from_year{  toInt( from_year_ ) };
    const int from_month{ this->getMonthNumber( from_month_ ) };
    const int from_day{   toInt( from_day_ ) };
    const int to_year{    to_year_.isEmpty()  ? from_year  : toInt( to_year_ )  };
    const int to_month{   to_month_.isEmpty() ? from_month : this->getMonthNumber( to_month_ )  };
    const int to_day{     to_day_.isEmpty()   ? from_day   : toInt( to_day_ )  };

    const QString& log_field{ this->getDbField( log_field_ ) };

    int n_days   { 0 },
        n_months { this->countMonths( from_year, from_month, to_year, to_month ) };

    int year  { from_year },
        month { from_month };
    std::unordered_map<int,int> days_l;
    days_l.reserve( 31ul );

    if ( n_months == 1 ) {
        // 1 month, no need to loop
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

        while ( query->next() ) {
            const int day{    toInt( query[0] ) };
            const int hour{   toInt( query[1] ) };
            const int minute{ toInt( query[2] ) };
            // increase the count
            ++ data.at( hour ).at( this->getMinuteGap( minute ) );
            // append the day as newly found if not found yet
            ++ days_l[ day ];
        }
        n_days += static_cast<int>(days_l.size());


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

            while ( query->next() ) {
                const int day{    toInt( query[0] ) };
                const int hour{   toInt( query[1] ) };
                const int minute{ toInt( query[2] ) };
                // increase the count
                ++ data.at( hour ).at( this->getMinuteGap( minute ) );
                // append the day as newly found if not found yet
                ++ days_l[ day ];
            }
            n_days += static_cast<int>(days_l.size());
            ++ month;
            if ( month > 12 ) {
                month = 1;
                ++ year;
            }
        }
    }
    if ( n_days == 0 ) {
        // no data
        return;
    }

    // divide the count by the number of days to get the mean value
    for ( const auto& [h,data_] : data ) {
        for ( const auto& [m,c] : data_ ) {
            int& count{ data.at( h ).at( m ) };
            if ( count > 0 ) {
                count /= n_days;
                if ( count == 0 ) {
                    ++ count;
                }
            }
        }
    }

    result.emplace( std::move(data) );
}



// get and count how many times a specific item value brought to another
void DbQuery::getRelationalCountsDay( std::optional<stats_relat_items_t>& result, QStringView web_server, QStringView year_, QStringView month_, QStringView day_, const LogField log_field_1_, QStringView field_filter_1, const LogField log_field_2_, QStringView field_filter_2 ) const
{
    stats_relat_items_t data; // std::vector<std::tuple<qint64, int>>
    int gap = 20;

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

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

    if ( query.size() == 0ul ) {
        return;
    }

    QDateTime time{
        QDate( year, month, day ),
        QTime( 0, 0, 0 )
    };

    data.reserve( static_cast<size_t>( 24*(60/gap) ) );

    const auto push_data{
        [&data,&time](const int count)
        {
            data.emplace_back( time.toMSecsSinceEpoch(), count );
        }
    };

    const auto set_time{
        [&time](const auto ...args)
        {
            time.setTime( QTime(args...) );
        }
    };

    int hour{-1},  next_hour,
        minute{0}, next_minute,
        count{0};

    while ( query->next() ) {
        next_hour   = toInt( query[0] );
        next_minute = this->getMinuteGap( toInt( query[1] ), gap );
        if ( next_hour == hour && next_minute == minute ) {
            ++ count;
        } else {
            if ( next_hour == hour ) {
                // same hour new minute gap, append the last count
                set_time( hour, minute );
                push_data( count );
                // and any missing gap
                for ( int m{minute+gap}; m<next_minute; m+=gap ) {
                    set_time( hour, m );
                    push_data( 0 );
                }
            } else {
                // minute is always different when the hour is different
                if ( hour >= 0 ) {
                    // apend the last minute-gap count if not in the first round of the loop
                    set_time( hour, minute );
                    push_data( count );
                    // append any missing gap in the current hour
                    for ( int m{minute+gap}; m<60; m+=gap ) {
                        set_time( hour, m );
                        push_data( 0 );
                    }
                    ++ hour;
                } else {
                    // prepare to add missing gaps from 00:00 (+gap will be added to the minute)
                    hour = 0;
                }
                // append any missing gap in every hour between the current and the next found (aux)
                for ( int h{hour}; h<next_hour; ++h ) {
                    for ( int m{0}; m<60; m+=gap ) {
                        set_time( h, m );
                        push_data( 0 );
                    }
                }
                // append any missing gap in the netx found hour
                for ( int m{0}; m<next_minute; m+=gap ) {
                    set_time( next_hour, m );
                    push_data( 0 );
                }
                hour = next_hour;
            }
            minute = next_minute;
            count = 1;
        }
    }
    // append the last count
    set_time( hour, minute );
    push_data( count );
    // append any missing gap in the last hour
    for ( int m{minute+gap}; m<60; m+=gap ) {
        set_time( hour, m );
        push_data( 0 );
    }
    // append any missing data up to the end of the day
    for ( int h{hour+1}; h<24; ++h ) {
        for ( int m{0}; m<60; m+=gap ) {
            set_time( h, m );
            push_data( 0 );
        }
    }
    // append the real last fictitious count
    int d{ day }, m{ month }, y{ year };
    if ( ++d > this->getMonthDays( year, month ) ) {
        if ( ++m > 12 ) {
            m = 1;
            ++y;
        }
        d = 1;
    }
    time.setDate( QDate( y, m , d ) );
    time.setTime( QTime( 0, 0, 0 ) );
    push_data( 0 );

    result.emplace( std::move(data) );
}



void DbQuery::getRelationalCountsPeriod( std::optional<stats_relat_items_t>& result, QStringView web_server, QStringView from_year_, QStringView from_month_, QStringView from_day_, QStringView to_year_, QStringView to_month_, QStringView to_day_, const LogField log_field_1_, QStringView field_filter_1, const LogField log_field_2_, QStringView field_filter_2 ) const
{
    stats_relat_items_t data; // std::vector<std::tuple<qint64, int>>

    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    const int from_year{  toInt( from_year_ ) };
    const int from_month{ this->getMonthNumber( from_month_ ) };
    const int from_day{   toInt( from_day_ ) };
    const int to_year{    to_year_.isEmpty()  ? from_year  : toInt( to_year_ ) };
    const int to_month{   to_month_.isEmpty() ? from_month : this->getMonthNumber( to_month_ ) };
    const int to_day{     to_day_.isEmpty()   ? from_day   : toInt( to_day_ ) };

    const QString& log_field_1{ this->getDbField( log_field_1_ ) };
    const QString& log_field_2{ this->getDbField( log_field_2_ ) };

    const int n_months{ this->countMonths( from_year, from_month, to_year, to_month ) };

    QDateTime time;

    const auto set_date{
        [&time](const auto ...args)
        {
            time.setDate( QDate(args...) );
        }
    };

    const auto push_data{
        [&data,&time](const int count)
        {
            data.emplace_back( time.toMSecsSinceEpoch(), count );
        }
    };

    const auto prev_instant{
        [this](int& y, int& m, int& d)
        {
            if ( --d < 1 ) {
                if ( --m < 1 ) {
                    m=12; --y;
                }
                d = this->getMonthDays( y, m );
            }
        }
    };
    const auto next_instant{
        [this](int& y, int& m, int& d)
        {
            if ( ++d > this->getMonthDays( y, m ) ) {
                if ( ++m > 12 ) {
                    m=1; ++y;
                }
                d = 1;
            }
        }
    };

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

        if ( query.size() == 0ul ) {
            return;
        }

        data.reserve( to_day - from_day );

        int day{0}, count{0};

        while ( query->next() ) {
            const int next_day{ toInt( query[0] ) };
            if ( next_day == day ) {
                ++ count;
                continue; // avoids resetting the count at the end

            } else if ( day > 0 ) {
                // any loop-round except the first
                set_date( year, month , day );
                push_data( count );
                for ( int d{day+1}; d<next_day; ++d ) {
                    // append any missing day with a zero value
                    set_date( year, month , d );
                    push_data( 0 );
                }

            } else {
                // day == 0 only in the first round of the loop
                // append any missing day from 1 day before the first until the next found
                int d{ from_day }, m{ month }, y{ year };
                prev_instant( y, m, d );
                while ( d!=next_day ) {
                    set_date( y, m , d );
                    push_data( 0 );
                    next_instant( y, m, d );
                }
            }
            day = next_day;
            count = 1;
        }
        // append the last count
        set_date( year, month , day );
        push_data( count );
        // append any missing day from the last found until 1 day after the last one
        next_instant( year, month, day );
        int max_day{ to_day }, max_month{ to_month }, max_year{ to_year };
        next_instant( max_year, max_month, max_day );
        while ( year<=max_year && month<=max_month && day<=max_day ) {
            set_date( year, month , day );
            push_data( 0 );
            next_instant( year, month, day );
        }


    } else {
        data.reserve( this->countDays( from_year, from_month, from_day, to_year, to_month, to_day ) );
        bool no_data{ true };

        const QString query_filters{ QStringLiteral("%1%2").arg(
            !field_filter_1.isEmpty() ? QStringLiteral(R"( AND "%1"%2)").arg( log_field_1, field_filter_1 ) : QString(),
            !field_filter_2.isEmpty() ? QStringLiteral(R"( AND "%1"%2)").arg( log_field_2, field_filter_2 ) : QString())
        };

        for ( int m{1}; m<=n_months; ++m ) {

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

            if ( query.size() == 0ul ) {
                // no data found for this month, append missing days with 0 value
                const int max_d{ m==n_months ? to_day : this->getMonthDays( year, month ) };
                int d{ m==1 ? from_day : 1 };
                for ( ; d<=max_d; ++d ) {
                    set_date( year, month , d );
                    push_data( 0 );
                }

            } else {
                no_data &= false;

                int day{0}, count{0};
                while ( query->next() ) {
                    const int next_day{ toInt( query[0] ) };
                    if ( next_day == day ) {
                        ++ count;
                        continue; // avoids resetting the count at the end

                    } else  if ( day > 0 ) {
                        // any loop-round except the first
                        set_date( year, month, day++ );
                        push_data( count );
                        while ( day<next_day ) {
                            // append any missing day with a zero value
                            set_date( year, month, day++ );
                            push_data( 0 );
                        }

                    } else {
                        // day == 0 only in the first round of the loop
                        // append any missing day until one before the next day with a zero value
                        int d{ m==1 ? from_day : 2 }, m{ month }, y{ year };
                        prev_instant( y, m, d );
                        while ( y<=year && m<=month && d<next_day ) {
                            set_date( y, m, d );
                            push_data( 0 );
                            next_instant( y, m, d );
                        }
                    }
                    day = next_day;
                    count = 1;
                }
                // append the last count
                if ( day > 0 ) {
                    set_date( year, month , day );
                    push_data( count );
                }
                // append any missing day up to the last one with a zero value
                const int max_d{ m==n_months ? to_day : this->getMonthDays( year, month ) };
                int d{ day>0 ? day : 1 }, m{ month }, y{ year };
                while ( y<=year && m<=month && d<=max_d ) {
                    set_date( y, m, d );
                    push_data( 0 );
                    next_instant( y, m, d );
                }
            }
            // increase the month
            if ( ++month > 12 ) {
                month = 1;
                ++year;
            }
        }
        if ( no_data ) {
            return;
        }
        // append one day after the last one
        int day{ to_day };
        next_instant( year, month, day );
        set_date( year, month , day );
        push_data( 0 );
    }

    if ( data.capacity() > data.size() ) {
        data.shrink_to_fit();
    }

    result.emplace( std::move(data) );
}



void DbQuery::getGlobalCounts( std::optional<GlobalsData>& result, QStringView web_server, const stats_dates_t& dates ) const
{
    DatabaseWrapper db{ DatabaseHandler::get( DatabaseType::Data, true ) };

    db.open( this->db_path, this->dialog_level==DL_EXPLANATORY );

    bool no_data{ true };
    int max_date_year, max_date_month, max_date_day;
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
    for ( auto [total,count] : std::views::zip( data.traf.day, week_days_count ) ) {
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


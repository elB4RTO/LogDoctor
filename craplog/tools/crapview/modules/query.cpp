
#include "query.h"

#include "modules/dialogs.h"
#include "utilities/strings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>


DbQuery::DbQuery()
{

}


void DbQuery::setDialogLevel( const int new_level )
{
    this->dialog_level = new_level;
}

void DbQuery::setDbPath( const std::string& path )
{
    this->db_path = path;
    this->db_name = QString::fromStdString( this->db_path.substr( this->db_path.find_last_of( '/' ) + 1 ) );
}


const int DbQuery::getMinuteGap(const int minute , const int gap )
{
    int m = -1;
    if ( minute < 0 || minute >= 60 ) {
        // unexpected value
        throw ( "Unexpected Minute: "[minute] );
    }
    int n = 0;
    for ( int g=0; g<60; g+=gap ) {
        if ( minute >= g && minute < g+gap ) {
            m = gap * n;
            break;
        }
        n++;
    }
    return m;
}

const int DbQuery::getMonthDays( const int year, const int month )
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
            throw ( "Unexpected Month: "[month] );
    }
    return n_days;
}

const int DbQuery::getMonthsCount( const QString& from_year, const QString& from_month, const QString& to_year, const QString& to_month )
{
    int from_year_, from_month_, to_year_, to_month_;
    try {
        from_year_  = from_year.toInt();
        from_month_ = this->Months_s2i.value( from_month );
        to_year_  = ( to_year.size() == 0 )  ? from_year_  : to_year.toInt() ;
        to_month_ = ( to_month.size() == 0 ) ? from_month_ : this->Months_s2i.value( to_month ) ;
    } catch (...) {
        // failed to convert to integers
        throw std::exception();
    }
    return this->getMonthsCount( from_year_, from_month_, to_year_, to_month_ );
}

const int DbQuery::getMonthsCount( const int from_year, const int from_month, const int to_year, const int to_month )
{
    int n_months = 0;
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


// get a fresh map of available dates
void DbQuery::refreshDates(std::tuple<bool, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int> > > > > > &result)
{
    bool successful = true;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>> dates = {
        {11,{ {1,{}},{2,{}} }}, {12,{ {1,{}},{2,{}} }}, {13,{ {1,{}},{2,{}} }} };

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        // recursively query years, months and days for every WebServer
        std::vector<std::tuple<int, int, QString>> tables;
        tables.push_back( std::make_tuple(11,1,"apache_access") );
        tables.push_back( std::make_tuple(11,2,"apache_error") );
        tables.push_back( std::make_tuple(12,1,"nginx_access") );
        tables.push_back( std::make_tuple(12,2,"nginx_error") );
        tables.push_back( std::make_tuple(13,1,"iis_access") );
        tables.push_back( std::make_tuple(13,2,"iis_error") );

        QSqlQuery Y_query = QSqlQuery( db ),
                  M_query = QSqlQuery( db ),
                  D_query = QSqlQuery( db );

        for ( const auto& table : tables ) {
            if ( successful == false ) { break; }

            int ws = std::get<0>(table);
            int lt = std::get<1>(table);
            QString tbl = std::get<2>(table);

            if ( Y_query.exec( QString("SELECT DISTINCT \"year\" FROM \"%1\" ORDER BY \"year\" ASC;").arg(tbl) ) == false ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, Y_query.lastQuery(), Y_query.lastError().text() );
                break;

            } else {
                int year, month, day;
                while ( Y_query.next() ) {
                    try {
                        year = Y_query.value(0).toInt();
                    } catch (...) {
                        // failed to convert to integer
                        successful = false;
                        QString err_msg = QString("An error occured while parsing Years from the database");
                        if ( this->dialog_level > 0 ) {
                            err_msg += QString("\n\nValue responsible for the error:\n%1").arg(Y_query.value(0).toString());
                            if ( this->dialog_level == 2 ) {
                                err_msg += QString("\n\nDatabase table name:\n%1").arg(tbl);
                            }
                        }
                        DialogSec::errGeneric( nullptr, err_msg );
                        break;
                    }
                    // successfully get the year
                    dates.at( ws ).at( lt ).emplace( year, std::unordered_map<int, std::vector<int>>() );
                    // query any available month
                    if ( M_query.exec( QString("SELECT DISTINCT \"month\" FROM \"%1\" WHERE \"year\"=%2 ORDER BY \"month\" ASC;").arg(tbl).arg(year) ) == false ) {
                        // error querying database
                        successful = false;
                        DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, M_query.lastQuery(), M_query.lastError().text() );
                        break;

                    } else {
                        while ( M_query.next() ) {
                            try {
                                month = M_query.value(0).toInt();
                            } catch (...) {
                                // failed to convert to integer
                                successful = false;
                                QString err_msg = QString("An error occured while parsing Months from the database");
                                if ( this->dialog_level > 0 ) {
                                    err_msg += QString("\n\nValue responsible for the error:\n%1").arg(M_query.value(0).toString());
                                    if ( this->dialog_level == 2 ) {
                                        err_msg += QString("\n\nDatabase table name:\n%1").arg(tbl);
                                    }
                                }
                                DialogSec::errGeneric( nullptr, err_msg );
                                break;
                            }
                            // successfully get the month
                            dates.at( ws ).at( lt ).at( year ).emplace( month, std::vector<int>() );
                            // query any available day
                            if ( D_query.exec( QString("SELECT DISTINCT \"day\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3 ORDER BY \"day\" ASC;").arg(tbl).arg(year).arg(month) ) == false ) {
                                // error querying database
                                successful = false;
                                DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, D_query.lastQuery(), D_query.lastError().text() );
                                break;

                            } else {
                                while ( D_query.next() ) {
                                    try {
                                        day = D_query.value(0).toInt();
                                    } catch (...) {
                                        // failed to convert to integer
                                        successful = false;
                                        QString err_msg = QString("An error occured while parsing Days from the database");
                                        if ( this->dialog_level > 0 ) {
                                            err_msg += QString("\n\nValue responsible for the error:\n%1").arg(D_query.value(0).toString());
                                            if ( this->dialog_level == 2 ) {
                                                err_msg += QString("\n\nDatabase table name:\n%1").arg(tbl);
                                            }
                                        }
                                        DialogSec::errGeneric( nullptr, err_msg );
                                        break;
                                    }
                                    // successfully get the day
                                    dates.at( ws ).at( lt ).at( year ).at( month ).push_back( day );
                                }
                                D_query.finish();
                                // break if something went wrong
                                if ( successful == false ) { break; }
                            }
                        }
                        M_query.finish();
                        // break if something went wrong
                        if ( successful == false ) { break; }
                    }
                }
                Y_query.finish();
                // break if something went wrong
                if ( successful == false ) { break; }
            }
        }
    }

    if ( successful == false ) {
        dates.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, dates );
}



// get day-time values for the time-taken field
void DbQuery::getSpeedData(std::tuple<bool, std::vector<std::tuple<long long, std::vector<QString>>>>& result, const QString& web_server, const QString& year_, const QString& month_, const QString& day_, const QString& protocol_f, const QString& method_f, const QString& uri_f, const QString& query_f, const QString& response_f )
{
    bool successful = true;
    std::vector<std::tuple<long long, std::vector<QString>>> data;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        QString table;
        if ( web_server == "Apache2" ) {
            table += "apache_access";
        } else if ( web_server == "Nginx" ) {
            table += "nginx_access";
        } else if ( web_server == "Apache2" ) {
            table += "iis_access";
        } else {
            // unexpected WebServer
            successful = false;
            DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_WS).arg( web_server ), true );
        }
        int year, month, day;
        if ( successful == true ) {
            // setup period limits
            try {
                year  = year_.toInt();
                month = this->Months_s2i.value( month_ );
                day   = day_.toInt();
            } catch (...) {
                // failed to convert to integers
                successful = false;
                DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING_DATES, true );
            }
        }
        if ( successful == true ) {
            QDateTime time;
            time.setDate( QDate( year, month , day ) );
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt, filter;

            // prepare the statement
            stmt = QString("SELECT \"hour\",\"minute\",\"second\",\"time_taken\",\"uri\",\"query\",\"method\",\"protocol\",\"response\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3 AND \"day\"=%4 AND \"time_taken\" IS NOT NULL")
                .arg( table.replace("'","''") )
                .arg( year ).arg( month )
                .arg( day );

            // apply a filter if present
            if ( protocol_f.size() > 0 ) {
                stmt += QString(" AND \"protocol\" LIKE '%1' || '%'")
                    .arg( QString(protocol_f).replace("'","''") );
            }
            if ( method_f.size() > 0 ) {
                stmt += QString(" AND \"method\" LIKE '%1' || '%'")
                    .arg( QString(method_f).replace("'","''") );
            }
            if ( uri_f.size() > 0 ) {
                stmt += QString(" AND \"uri\" LIKE '%1' || '%'")
                    .arg( QString(uri_f).replace("'","''") );
            }
            if ( query_f.size() > 0 ) {
                stmt += QString(" AND \"query\" LIKE '%1' || '%'")
                    .arg( QString(query_f).replace("'","''") );
            }
            if ( response_f.size() > 0 ) {
                // numbers
                if ( StringOps::isNumeric( response_f.toStdString() ) == true ) {
                    // no operator found, set defult to '='
                    filter = QString("=%1").arg( response_f );
                } else {
                    filter = response_f;
                }
                stmt += QString(" AND \"response\"%1")
                    .arg( filter.replace("'","''") );
            }

            stmt += QString(" ORDER BY \"hour\",\"minute\",\"second\" ASC;");
            if ( query.exec( stmt ) == false ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

            } else {
                int hour, aux_hour, minute, aux_minute, second, aux_second;
                try {
                    // get query data
                    int gap = 5;
                    hour = -1;
                    minute = second = 0;
                    if ( query.size() == 0 ) {
                        // no result found
                        ;

                    } else {
                        QString tt, ur, qr, mt, pt, rs;
                        while ( query.next() ) {
                            aux_hour   = query.value(0).toInt();
                            aux_minute = query.value(1).toInt();
                            aux_second = query.value(2).toInt();

                            if ( aux_hour == hour && aux_minute == minute && aux_second == second ) {
                                time.setTime( QTime( hour, minute, second ));
                                data.push_back( std::make_tuple(
                                    time.toMSecsSinceEpoch(),
                                    std::vector<QString>{tt,ur,qr,mt,pt,rs} ));
                            } else {
                                if ( aux_hour == hour ) {
                                    // same hour new minute/second, append the last count
                                    time.setTime( QTime( hour, minute, second ));
                                    data.push_back( std::make_tuple(
                                        time.toMSecsSinceEpoch(),
                                        std::vector<QString>{tt,ur,qr,mt,pt,rs} ));
                                    // append the second after the last one with value 0, if it is not equaò to the next
                                    if ( aux_minute > minute
                                      || (aux_minute == minute && aux_second > second+1) ) {
                                        time.setTime( QTime( hour, minute, second+1 ));
                                        data.push_back( std::make_tuple(
                                            time.toMSecsSinceEpoch(),
                                            std::vector<QString>{"","","","","",""} ));
                                    }
                                } else {
                                    // minute/second are always different when the hour is different
                                    if ( hour >= 0 ) {
                                        // apend the last p count if not in the first round of the loop
                                        time.setTime( QTime( hour, minute, second ));
                                        data.push_back( std::make_tuple(
                                            time.toMSecsSinceEpoch(),
                                            std::vector<QString>{tt,ur,qr,mt,pt,rs} ));
                                        // append the next as zero
                                        time.setTime( QTime( hour, minute, second+1 ));
                                        data.push_back( std::make_tuple(
                                            time.toMSecsSinceEpoch(),
                                            std::vector<QString>{"","","","","",""} ));
                                        hour ++;
                                    } else {
                                        // append the second 0 of the day, if it is not the next found
                                        if ( aux_hour != 0 || aux_minute != 0 || aux_second > 0 ) {
                                            time.setTime( QTime( 0, 0, 0 ));
                                            data.push_back( std::make_tuple(
                                                time.toMSecsSinceEpoch(),
                                                std::vector<QString>{"","","","","",""} ));
                                            // append the second before the first found with value 0
                                            int h=aux_hour, m=aux_minute, s=aux_second;
                                            s --;
                                            if ( s < 0 ) {
                                                s = 59;
                                                m --;
                                                if ( m < 0 ) {
                                                    m = 59;
                                                    h --;
                                                    if ( h < 0 ) {
                                                        // abort
                                                        h=0, m=0, s=0;
                                                    }
                                                }
                                            }
                                            if ( h != 0 || m != 0 || s != 0 ) {
                                                time.setTime( QTime( h, m, s ));
                                                data.push_back( std::make_tuple(
                                                    time.toMSecsSinceEpoch(),
                                                    std::vector<QString>{"","","","","",""} ));
                                            }
                                        }
                                    }
                                    hour = aux_hour;
                                }
                                minute = aux_minute;
                                second = aux_second;
                            }
                            tt = query.value(3).toString();
                            ur = query.value(4).toString();
                            qr = query.value(5).toString();
                            mt = query.value(6).toString();
                            pt = query.value(7).toString();
                            rs = query.value(8).toString();
                        }
                        // append the last count
                        time.setTime( QTime( hour, minute, second ));
                        data.push_back( std::make_tuple(
                            time.toMSecsSinceEpoch(),
                            std::vector<QString>{tt,ur,qr,mt,pt,rs} ));
                        // append 1 second after the last with 0
                        if ( hour != 23 || minute != 59 || second < 59 ) {
                            time.setTime( QTime( hour, minute, second+1 ));
                            data.push_back( std::make_tuple(
                                time.toMSecsSinceEpoch(),
                                std::vector<QString>{"","","","","",""} ));
                        }
                        // append the real last fictitious count
                        day ++;
                        if ( day > this->getMonthDays( year, month ) ) {
                            day = 1;
                            month ++;
                            if ( month > 12 ) {
                                month = 1;
                                year ++;
                            }
                        }
                        time.setDate( QDate( year, month , day ) );
                        time.setTime( QTime( 0, 0, 0 ));
                        data.push_back( std::make_tuple(
                            time.toMSecsSinceEpoch(),
                            std::vector<QString>{"","","","","",""} ));
                    }
                } catch (...) {
                    // something failed
                    successful = false;
                    DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                }
            }
        }
    }

    if ( successful == false ) {
        data.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, data );
}



// get, group and count identical items of a specific field in a date
void DbQuery::getItemsCount( std::tuple<bool, std::vector<std::tuple<QString, int>>>& result, const QString& web_server, const QString& log_type, const QString& year, const QString& month, const QString& day, const QString& log_field )
{
    bool successful = true;
    QHash<QString, int> aux_items;
    std::vector<std::tuple<QString, int>> items;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        QString table;
        if ( web_server == "Apache2" ) {
            table += "apache_";
        } else if ( web_server == "Nginx" ) {
            table += "nginx_";
        } else if ( web_server == "Apache2" ) {
            table += "iis_";
        } else {
            // unexpected WebServer
            successful = false;
            DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_WS).arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == TYPES.value(1) ) {
                table += "access";
            } else if ( log_type == TYPES.value(2) ) {
                table += "error";
            } else {
                // unexpected LogType
                successful = false;
                DialogSec::errGeneric( nullptr, QString("Unexpected LogType:\n%1").arg( web_server ), true );
            }
        }
        if ( successful == true ) {
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt = QString("SELECT \"%1\" FROM \"%2\" WHERE \"%3\" IS NOT NULL AND \"year\"=%4 AND \"month\"=%5 AND \"day\"=%6;")
                .arg( this->LogFields_to_DbFields.value( log_field ))
                .arg( table )
                .arg( this->LogFields_to_DbFields.value( log_field ))
                .arg( year )
                .arg( this->Months_s2i.value( month ) )
                .arg( day );
            // quary the database
            if ( query.exec( stmt.replace("'","''") ) == false ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

            } else {
                try {
                    // get data
                    QString item;
                    while ( query.next() ) {
                        item = query.value(0).toString();
                        if ( item.size() > 0 ) {
                            if ( aux_items.find( item ) == aux_items.end() ) {
                                // not present yet
                                aux_items.insert( item, 1 );
                            } else {
                                // already in the list, increase the count
                                aux_items[ item ] ++;
                            }
                        }
                    }
                } catch (...) {
                    // something failed
                    successful = false;
                    DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                }
            }
        }
        if ( successful == true ) {
            // sort the list
            std::tuple<QString, int> item;
            // morph tha HashMap into a Vector of Tuples
            for ( const QString& v : aux_items.keys() ) {
                item = std::make_tuple( v, aux_items.value( v ) );
                items.push_back( item );
            }
            // sort the vector
            for ( int i=0; i<items.size(); i++ ) {
                for ( int j=0; j<items.size(); j++ ) {
                    if ( std::get<1>(items.at( i )) > std::get<1>(items.at( j )) ) {
                        item = items.at( i );
                        items.at( i ) = items.at( j );
                        items.at( j ) = item;
                    }
                }
            }
        }
        aux_items.clear();
    }

    if ( successful == false ) {
        items.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, items );
}



// get and count items with a 10 minutes gap for every hour of the day
void DbQuery::getDaytimeCounts( std::tuple<bool, std::unordered_map<int, std::unordered_map<int, int>>>& result, const QString& web_server, const QString& log_type, const QString& from_year_, const QString& from_month_, const QString& from_day_, const QString& to_year_, const QString& to_month_, const QString& to_day_, const QString& log_field_, const QString& field_filter )
{
    bool successful = true;
    std::unordered_map<int, std::unordered_map<int, int>> data = {
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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        QString table;
        if ( web_server == "Apache2" ) {
            table += "apache_";
        } else if ( web_server == "Nginx" ) {
            table += "nginx_";
        } else if ( web_server == "Apache2" ) {
            table += "iis_";
        } else {
            // unexpected WebServer
            successful = false;
            DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_WS).arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == TYPES.value(1) ) {
                table += "access";
            } else if ( log_type == TYPES.value(2) ) {
                table += "error";
            } else {
                // unexpected LogType
                successful = false;
                DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_LT).arg( log_type ), true );
            }
        }
        int from_year, from_month, from_day,
            to_year, to_month, to_day;
        if ( successful == true ) {
            // setup period limits
            try {
                from_year  = from_year_.toInt();
                from_month = this->Months_s2i.value( from_month_ );
                from_day   = from_day_.toInt();
                to_year  = ( to_year_.size() == 0 )  ? from_year  : to_year_.toInt() ;
                to_month = ( to_month_.size() == 0 ) ? from_month : this->Months_s2i.value( to_month_ ) ;
                to_day   = ( to_day_.size() == 0 )   ? from_day   : to_day_.toInt() ;
            } catch (...) {
                // failed to convert to integers
                successful = false;
                DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING_DATES, true );
            }
        }
        if ( successful == true ) {
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt;
            QString log_field = this->LogFields_to_DbFields.value( log_field_ );

            int n_days   = 0,
                n_months = this->getMonthsCount( from_year, from_month, to_year, to_month );

            int year = from_year,
                month = from_month,
                day, hour, minute;
            QList<int> days_l;

            if ( n_months == 1 ) {
                // 1 month, no need to loop
                stmt = QString("SELECT \"day\", \"hour\", \"minute\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3 AND \"day\">=%4 AND \"day\"<=%5")
                    .arg( table.replace("'","''") )
                    .arg( year )
                    .arg( month )
                    .arg( from_day ).arg( to_day );

                // only select non-NULL values
                stmt += QString(" AND \"%1\" IS NOT NULL")
                    .arg( log_field.replace("'","''") );
                // apply a filter if present
                if ( field_filter.size() > 0 ) {
                    stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                        .arg( log_field.replace("'","''") )
                        .arg( QString(field_filter).replace("'","''") );
                }

                stmt += ";";
                if ( query.exec( stmt ) == false ) {
                    // error querying database
                    successful = false;
                    DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

                } else {
                    try {
                        // clear the list of found days
                        days_l.clear();
                        // get query data
                        while ( query.next() ) {
                            day    = query.value(0).toInt();
                            hour   = query.value(1).toInt();
                            minute = query.value(2).toInt();
                            // increase the count
                            data.at( hour ).at( this->getMinuteGap( minute ) ) ++;
                            // append the day as newly found if not found yet
                            if ( days_l.indexOf( day ) < 0 ) {
                                days_l.push_back( day );
                            }
                        }
                        n_days += days_l.size();
                    } catch (...) {
                        // something failed
                        successful = false;
                        DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                    }
                }


            } else {
                for ( int m=1; m<=n_months; m++ ) {
                    stmt = QString("SELECT \"day\", \"hour\", \"minute\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3")
                        .arg( table )
                        .arg( year )
                        .arg( month );
                    if ( m == 1 ) {
                        // first month, only get the day from the beginning day
                        stmt += QString(" AND \"day\">=%1").arg( from_day );
                    } else if ( m == n_months ) {
                        // last month, only get the days until the ending day
                        stmt += QString(" AND \"day\"<=%1").arg( to_day );
                    }

                    // only select non-NULL values
                    stmt += QString(" AND \"%1\" IS NOT NULL")
                        .arg( log_field.replace("'","''") );
                    // apply a filter if present
                    if ( field_filter.size() > 0 ) {
                        stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                            .arg( log_field.replace("'","''") )
                            .arg( QString(field_filter).replace("'","''") );
                    }

                    // quary the database
                    stmt += ";";
                    if ( query.exec( stmt ) == false ) {
                        // error querying database
                        successful = false;
                        DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

                    } else {
                        try {
                            // clear the list of found days
                            days_l.clear();
                            // get query data
                            while ( query.next() ) {
                                day    = query.value(0).toInt();
                                hour   = query.value(1).toInt();
                                minute = query.value(2).toInt();
                                // increase the count
                                data.at( hour ).at( this->getMinuteGap( minute ) );
                                // append the day as newly found if not found yet
                                if ( days_l.indexOf( day ) < 0 ) {
                                    days_l.push_back( day );
                                }
                            }
                            n_days += days_l.size();
                            month ++;
                            if ( month > 12 ) {
                                month = 1;
                                year ++;
                            }
                        } catch (...) {
                            // something failed
                            successful = false;
                            DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                            break;
                        }
                    }
                }
            }
            if ( successful == true && n_days > 0 ) {
                // divide the count by the number of days to get the mean value
                for ( const auto& [h,data_] : data ) {
                    for ( const auto& [m,c] : data_ ) {
                        data.at( h ).at( m ) /= n_days;
                    }
                }
            }
        }
    }


    if ( successful == false ) {
        data.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, data );
}



// get and count how many times a specific item value brought to another
void DbQuery::getRelationalCountsDay(std::tuple<bool, std::vector<std::tuple<long long, int>>> &result, const QString& web_server, const QString& log_type, const QString& year_, const QString& month_, const QString& day_, const QString& log_field_1_, const QString& field_filter_1, const QString& log_field_2_, const QString& field_filter_2 )
{
    bool successful = true;
    std::vector<std::tuple<long long, int>> data;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        QString table;
        if ( web_server == "Apache2" ) {
            table += "apache_";
        } else if ( web_server == "Nginx" ) {
            table += "nginx_";
        } else if ( web_server == "Apache2" ) {
            table += "iis_";
        } else {
            // unexpected WebServer
            successful = false;
            DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_WS).arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == TYPES.value(1) ) {
                table += "access";
            } else if ( log_type == TYPES.value(2) ) {
                table += "error";
            } else {
                // unexpected LogType
                successful = false;
                DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_LT).arg( log_type ), true );
            }
        }

        int year, month, day;
        if ( successful == true ) {
            // setup period limits
            try {
                year  = year_.toInt();
                month = this->Months_s2i.value( month_ );
                day   = day_.toInt();
            } catch (...) {
                // failed to convert to integers
                successful = false;
                DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING_DATES, true );
            }
        }
        if ( successful == true ) {
            QDateTime time;
            time.setDate( QDate( year, month , day ) );
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt;
            QString log_field_1 = this->LogFields_to_DbFields.value( log_field_1_ ),
                    log_field_2 = this->LogFields_to_DbFields.value( log_field_2_ );

            int hour, aux_hour, minute, aux_minute, count;

            // 1 month, no need to loop
            stmt = QString("SELECT \"hour\", \"minute\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3 AND \"day\"=%4")
                .arg( table.replace("'","''") )
                .arg( year ).arg( month )
                .arg( day );

            // only select non-NULL values
            stmt += QString(" AND \"%1\" IS NOT NULL")
                .arg( log_field_1.replace("'","''") );
            // apply a filter if present
            if ( field_filter_1.size() > 0 ) {
                QString filter = field_filter_1;
                if ( log_field_1 == "response"
                  || log_field_1 == "time_taken"
                  || log_field_1 == "bytes_sent"
                  || log_field_1 == "bytes_received" ) {
                    // numbers
                    if ( StringOps::isNumeric( field_filter_1.toStdString() ) == true ) {
                        // no operator found, set defult to '='
                        filter = QString("=%1").arg( field_filter_1 );
                    }
                    stmt += QString(" AND \"%1\"%2")
                        .arg( log_field_1.replace("'","''") )
                        .arg( filter.replace("'","''") );

                } else {
                    stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                        .arg( log_field_1.replace("'","''") )
                        .arg( filter.replace("'","''") );
                }
            }
            // only select non-NULL values
            stmt += QString(" AND \"%1\" IS NOT NULL")
                .arg( log_field_2.replace("'","''") );
            // apply a filter if present
            if ( field_filter_2.size() > 0 ) {
                QString filter = field_filter_2;
                if ( log_field_2 == "response"
                  || log_field_2 == "time_taken"
                  || log_field_2 == "bytes_sent"
                  || log_field_2 == "bytes_received" ) {
                    // numbers
                    if ( StringOps::isNumeric( field_filter_2.toStdString() ) == true ) {
                        // no operator found, set defult to '='
                        filter = QString("=%1").arg( field_filter_2 );
                    }
                    stmt += QString(" AND \"%1\"%2")
                        .arg( log_field_2.replace("'","''") )
                        .arg( filter.replace("'","''") );

                } else {
                    stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                        .arg( log_field_2.replace("'","''") )
                        .arg( QString(field_filter_2).replace("'","''") );
                }
            }

            stmt += QString(" ORDER BY \"hour\",\"minute\" ASC;");
            if ( query.exec( stmt ) == false ) {
                // error querying database
                successful = false;
                DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

            } else {
                try {
                    // get query data
                    int gap = 20;
                    hour = -1;
                    minute = count = 0;
                    if ( query.size() == 0 ) {
                        // no result found, append missing days with 0 value
                        for ( int h=0; h<24; h++ ) {
                            for ( int m=0; m<60; m+=gap ) {
                                time.setTime( QTime( h, m ) );
                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                            }
                        }

                    } else {
                        while ( query.next() ) {
                            aux_hour   = query.value(0).toInt();
                            aux_minute = this->getMinuteGap( query.value(1).toInt(), gap );
                            if ( aux_hour == hour && aux_minute == minute ) {
                                count ++;
                            } else {
                                if ( aux_hour == hour ) {
                                    // same hour new minute gap, append the last count
                                    time.setTime( QTime( hour, minute ) );
                                    data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                                    // and any missing gap
                                    for ( int m=minute+gap; m<aux_minute; m+=gap ) {
                                        time.setTime( QTime( hour, m ) );
                                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                    }
                                } else {
                                    // minute is always different when the hour is different
                                    if ( hour >= 0 ) {
                                        // apend the last minute-gap count if not in the first round of the loop
                                        time.setTime( QTime( hour, minute ) );
                                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                                        // append any missing gap in the current hour
                                        for ( int m=minute+gap; m<60; m+=gap ) {
                                            time.setTime( QTime( hour, m ) );
                                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                        }
                                        hour ++;
                                    } else {
                                        // prepare to add missing gaps from 00:00 (+gap will be added to the minute)
                                        hour = 0;
                                    }
                                    // append any missing gap in every hour between the current and the next found (aux)
                                    for ( int h=hour; h<aux_hour; h++ ) {
                                        for ( int m=0; m<60; m+=gap ) {
                                            time.setTime( QTime( h, m ) );
                                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                        }
                                    }
                                    // append any missing gap in the netx found hour
                                    for ( int m=0; m<aux_minute; m+=gap ) {
                                        time.setTime( QTime( aux_hour, m ) );
                                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                    }
                                    hour = aux_hour;
                                }
                                minute = aux_minute;
                                count = 1;
                            }
                        }
                        // append the last count
                        time.setTime( QTime( hour, minute ) );
                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                        // yet again, append any missing gap
                        for ( int m=minute+gap; m<60; m+=gap ) {
                            time.setTime( QTime( hour, m ) );
                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                        }
                        for ( int h=hour+1; h<24; h++ ) {
                            for ( int m=0; m<60; m+=gap ) {
                                time.setTime( QTime( h, m ) );
                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                            }
                        }
                        // append the real last fictitious count
                        day ++;
                        if ( day > this->getMonthDays( year, month ) ) {
                            day = 1;
                            month ++;
                            if ( month > 12 ) {
                                month = 1;
                                year ++;
                            }
                        }
                        time.setDate( QDate( year, month , day ) );
                        time.setTime( QTime( 0, 0 ) );
                        //time.setTime( QTime( 23, 59, 59, 999 ) );
                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                    }
                } catch (...) {
                    // something failed
                    successful = false;
                    DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                }
            }
        }
    }

    if ( successful == false ) {
        data.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, data );
}



void DbQuery::getRelationalCountsPeriod(std::tuple<bool, std::vector<std::tuple<long long, int>>> &result, const QString& web_server, const QString& log_type, const QString& from_year_, const QString& from_month_, const QString& from_day_, const QString& to_year_, const QString& to_month_, const QString& to_day_, const QString& log_field_1_, const QString& field_filter_1, const QString& log_field_2_, const QString& field_filter_2 )
{
    bool successful = true;
    std::vector<std::tuple<long long, int>> data;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, this->db_name, err_msg );

    } else {
        QString table;
        if ( web_server == "Apache2" ) {
            table += "apache_";
        } else if ( web_server == "Nginx" ) {
            table += "nginx_";
        } else if ( web_server == "Apache2" ) {
            table += "iis_";
        } else {
            // unexpected WebServer
            successful = false;
            DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_WS).arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == TYPES.value(1) ) {
                table += "access";
            } else if ( log_type == TYPES.value(2) ) {
                table += "error";
            } else {
                // unexpected LogType
                successful = false;
                DialogSec::errGeneric( nullptr, QString("%1:\n%2").arg(this->MSG_ERR_UNX_LT).arg( log_type ), true );
            }
        }
        int from_year, from_month, from_day,
            to_year, to_month, to_day;
        if ( successful == true ) {
            // setup period limits
            try {
                from_year  = from_year_.toInt();
                from_month = this->Months_s2i.value( from_month_ );
                from_day   = from_day_.toInt();
                to_year  = ( to_year_.size() == 0 )  ? from_year  : to_year_.toInt() ;
                to_month = ( to_month_.size() == 0 ) ? from_month : this->Months_s2i.value( to_month_ ) ;
                to_day   = ( to_day_.size() == 0 )   ? from_day   : to_day_.toInt() ;
            } catch (...) {
                // failed to convert to integers
                successful = false;
                DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING_DATES, true );
            }
        }
        if ( successful == true ) {
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt;
            QString log_field_1 = this->LogFields_to_DbFields.value( log_field_1_ ),
                    log_field_2 = this->LogFields_to_DbFields.value( log_field_2_ );

            int n_days   = 0,
                n_months = this->getMonthsCount( from_year, from_month, to_year, to_month );

            QDateTime time;
            int year  = from_year,
                month = from_month,
                day, aux_day, count;

            if ( n_months == 1 ) {
                // 1 month, no need to loop
                stmt = QString("SELECT \"day\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3 AND \"day\">=%4 AND \"day\"<=%5")
                    .arg( table.replace("'","''") )
                    .arg( year ).arg( month )
                    .arg( from_day ).arg( to_day );

                // only select non-NULL values
                stmt += QString(" AND \"%1\" IS NOT NULL")
                    .arg( log_field_1.replace("'","''") );
                // apply a filter if present
                if ( field_filter_1.size() > 0 ) {
                    QString filter = field_filter_1;
                    if ( log_field_1 == "response"
                      || log_field_1 == "time_taken"
                      || log_field_1 == "bytes_sent"
                      || log_field_1 == "bytes_received" ) {
                    // numbers
                    if ( StringOps::isNumeric( field_filter_1.toStdString() ) == true ) {
                        // no operator found, set defult to '='
                        filter = QString("=%1").arg( field_filter_1 );
                    }
                    stmt += QString(" AND \"%1\"%2")
                        .arg( log_field_1.replace("'","''") )
                        .arg( filter.replace("'","''") );

                    } else {
                        stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                            .arg( log_field_1.replace("'","''") )
                            .arg( filter.replace("'","''") );
                    }
                }
                // only select non-NULL values
                stmt += QString(" AND \"%1\" IS NOT NULL")
                    .arg( log_field_2.replace("'","''") );
                // apply a filter if present
                if ( field_filter_2.size() > 0 ) {
                    QString filter = field_filter_2;
                    if ( log_field_2 == "response"
                      || log_field_2 == "time_taken"
                      || log_field_2 == "bytes_sent"
                      || log_field_2 == "bytes_received" ) {
                    // numbers
                    if ( StringOps::isNumeric( field_filter_2.toStdString() ) == true ) {
                        // no operator found, set defult to '='
                        filter = QString("=%1").arg( field_filter_2 );
                    }
                    stmt += QString(" AND \"%1\"%2")
                        .arg( log_field_2.replace("'","''") )
                        .arg( filter.replace("'","''") );

                    } else {
                        stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                            .arg( log_field_2.replace("'","''") )
                            .arg( QString(field_filter_2).replace("'","''") );
                    }
                }

                stmt += QString(" ORDER BY \"day\" ASC;");
                if ( query.exec( stmt ) == false ) {
                    // error querying database
                    successful = false;
                    DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

                } else {
                    try {
                        // get query data
                        day = count = 0;
                        if ( query.size() == 0 ) {
                            // no days found, append missing days with 0 value
                            for ( int d=from_day; d<=to_day; d++ ) {
                                time.setDate( QDate( year, month , d ) );
                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                            }

                        } else {
                            while ( query.next() ) {
                                aux_day = query.value(0).toInt();
                                if ( aux_day == day ) {
                                    count ++;
                                } else {
                                    if ( day > 0 ) {
                                        // any loop-round except the first
                                        time.setDate( QDate( year, month , day ) );
                                        data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                                        for ( int d=day+1; d<aux_day; d++ ) {
                                            // append any missing day with a zero value
                                            time.setDate( QDate( year, month , d ) );
                                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                        }
                                    } else {
                                        // append any missing day from 1 day before the first until the next found
                                        int d = from_day - 1,
                                            m = month,
                                            y = year;
                                        if ( d < 1 ) {
                                            m --;
                                            if ( m < 1 ) {
                                                m = 12;
                                                y --;
                                            }
                                            d = this->getMonthDays( y, m );
                                        }
                                        for ( ; d!=aux_day; d++ ) {
                                            if ( d > this->getMonthDays( y, m ) ) {
                                                d = 1;
                                                m ++;
                                                if ( m > 12 ) {
                                                    m = 1;
                                                    y ++;
                                                }
                                            }
                                            time.setDate( QDate( y, m , d ) );
                                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                        }
                                    }
                                    day = aux_day;
                                    count = 1;
                                }
                            }
                            // append the last count
                            time.setDate( QDate( year, month , day ) );
                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                            // append any missing day from the last found until 1 day fater the last one
                            day++;
                            if ( day > this->getMonthDays( year, month ) ) {
                                month ++;
                                if ( month > 12 ) {
                                    month = 1;
                                    year ++;
                                }
                                day = this->getMonthDays( year, month );
                            }
                            to_day += 2;
                            if ( to_day > this->getMonthDays( year, month ) ) {
                                int m = month + 1,
                                    y = year;
                                if ( m > 12 ) {
                                    m = 1;
                                    y ++;
                                }
                                to_day = this->getMonthDays( y, m );
                            }
                            for ( ; day!=to_day; day++ ) {
                                if ( day > this->getMonthDays( year, month ) ) {
                                    day = 1;
                                    month ++;
                                    if ( month > 12 ) {
                                        month = 1;
                                        year ++;
                                    }
                                }
                                time.setDate( QDate( year, month , day ) );
                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                            }
                        }
                    } catch (...) {
                        // something failed
                        successful = false;
                        DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                    }
                }

            } else {
                for ( int m=1; m<=n_months; m++ ) {
                    stmt = QString("SELECT \"day\" FROM \"%1\" WHERE \"year\"=%2 AND \"month\"=%3")
                        .arg( table.replace("'","''") )
                        .arg( year )
                        .arg( month );

                    if ( m == 1 ) {
                        // first month, only get the day from the beginning day
                        stmt += QString(" AND \"day\">=%1").arg( from_day );
                    } else if ( m == n_months ) {
                        // last month, only get the days until the ending day
                        stmt += QString(" AND \"day\"<=%1").arg( to_day );
                    }

                    // only select non-NULL values
                    stmt += QString(" AND \"%1\" IS NOT NULL")
                        .arg( log_field_1.replace("'","''") );
                    // apply a filter if present
                    if ( field_filter_1.size() > 0 ) {
                        QString filter = field_filter_1;
                        if ( log_field_1 == "response"
                          || log_field_1 == "time_taken"
                          || log_field_1 == "bytes_sent"
                          || log_field_1 == "bytes_received" ) {
                            // numbers
                            if ( StringOps::isNumeric( field_filter_1.toStdString() ) == true ) {
                                // no operator found, set defult to '='
                                filter = QString("=%1").arg( field_filter_1 );
                            }
                            stmt += QString(" AND \"%1\"%2")
                                .arg( log_field_1.replace("'","''") )
                                .arg( filter.replace("'","''") );

                        } else {
                            stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                                .arg( log_field_1.replace("'","''") )
                                .arg( filter.replace("'","''") );
                        }
                    }
                    // only select non-NULL values
                    stmt += QString(" AND \"%1\" IS NOT NULL")
                        .arg( log_field_2.replace("'","''") );
                    // apply a filter if present
                    if ( field_filter_2.size() > 0 ) {
                        QString filter = field_filter_2;
                        if ( log_field_2 == "response"
                          || log_field_2 == "time_taken"
                          || log_field_2 == "bytes_sent"
                          || log_field_2 == "bytes_received" ) {
                            // numbers
                            if ( StringOps::isNumeric( field_filter_2.toStdString() ) == true ) {
                                // no operator found, set defult to '='
                                filter = QString("=%1").arg( field_filter_2 );
                            }
                            stmt += QString(" AND \"%1\"%2")
                                .arg( log_field_2.replace("'","''") )
                                .arg( filter.replace("'","''") );

                        } else {
                            stmt += QString(" AND \"%1\" LIKE '%2' || '%'")
                                .arg( log_field_2.replace("'","''") )
                                .arg( QString(field_filter_2).replace("'","''") );
                        }
                    }

                    // quary the database
                    stmt += " ORDER BY \"day\" ASC;";
                    if ( query.exec( stmt ) == false ) {
                        // error querying database
                        successful = false;
                        DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

                    } else {
                        try {
                            // get query data
                            day = count = 0;
                            if ( query.size() == 0 ) {
                                // no days found, append missing days with 0 value
                                int f_d = 1,
                                    t_d = this->getMonthDays( year, month );
                                if ( m == 1 ) {
                                    // first month, only get the day from the beginning day
                                    f_d = from_day;
                                } else if ( m == n_months ) {
                                    // last month, only get the days until the ending day
                                    t_d = to_day;
                                }
                                for ( ; f_d<=t_d; f_d++ ) {
                                    time.setDate( QDate( year, month , f_d ) );
                                    data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                }

                            } else {
                                while ( query.next() ) {
                                    aux_day = query.value(0).toInt();
                                    if ( aux_day == day ) {
                                        count ++;
                                    } else {
                                        if ( day > 0 ) {
                                            // any loop-round except the first
                                            time.setDate( QDate( year, month , day ) );
                                            data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                                            for ( int d=day+1; d<aux_day; d++ ) {
                                                // append any missing day with a zero value
                                                time.setDate( QDate( year, month , d ) );
                                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                            }
                                        } else {
                                            // append any missing day until the next found day
                                            day = 1;
                                            for ( int d=day; d<aux_day; d++ ) {
                                                time.setDate( QDate( year, month , d ) );
                                                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                            }
                                        }
                                        day = aux_day;
                                        count = 1;
                                    }
                                }
                                // append the last count
                                if ( day > 0 ) {
                                    time.setDate( QDate( year, month , day ) );
                                    data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), count ) );
                                }
                                // append any missing day to the end of the month with a zero value
                                for ( int d=day+1; d<=this->getMonthDays(year,month); d++ ) {
                                    time.setDate( QDate( year, month , d ) );
                                    data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
                                }
                            }
                            // increase the month
                            month ++;
                            if ( month > 12 ) {
                                month = 1;
                                year ++;
                            }
                        } catch (...) {
                            // something failed
                            successful = false;
                            DialogSec::errGeneric( nullptr, this->MSG_ERR_PROCESSING, true );
                            break;
                        }
                    }
                }
                // append the first day of 1 month after the last one as the last one day
                day = 1;
                month = to_month +1;
                if ( month > 12 ) {
                    month = 1;
                    year ++;
                }
                time.setDate( QDate( year, month , day ) );
                data.push_back( std::make_tuple( time.toMSecsSinceEpoch(), 0 ) );
            }
        }
    }

    if ( successful == false ) {
        data.clear();
    }
    if ( db.isOpen() == true ) {
        db.close();
    }
    result = std::make_tuple( successful, data );
}


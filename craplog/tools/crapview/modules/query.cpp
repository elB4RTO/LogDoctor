
#include "query.h"

#include "modules/dialogs.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>


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


// get a fresh map of available dates
const std::tuple<const bool, const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>> DbQuery::refreshDates()
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

            if ( Y_query.exec( QString("SELECT DISTINCT year FROM %1 ORDER BY year ASC;").arg(tbl) ) == false ) {
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
                    if ( M_query.exec( QString("SELECT DISTINCT month FROM %1 WHERE year=%2 ORDER BY month ASC;").arg(tbl).arg(year) ) == false ) {
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
                            if ( D_query.exec( QString("SELECT DISTINCT day FROM %1 WHERE year=%2 AND month=%3 ORDER BY day ASC;").arg(tbl).arg(year).arg(month) ) == false ) {
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
    return std::make_tuple( successful, dates );
}


// get, group and count items of a specific field in a date
const std::tuple<bool, std::vector<std::tuple<QString, int>>> DbQuery::getItemsCount(const QString& web_server, const QString& log_type, const QString& year, const QString& month, const QString& day, const QString& log_field )
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
            DialogSec::errGeneric( nullptr, QString("Unexpected WebServer:\n%1").arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == "Access" ) {
                table += "access";
            } else if ( web_server == "Error" ) {
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
            QString stmt = QString("SELECT %1 FROM %2 WHERE year=%3 AND month=%4 AND day=%5;")
                .arg( this->LogFields_to_DbFields.value( log_field ))
                .arg( table )
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
                    DialogSec::errGeneric( nullptr, QString("An error occured while processing"), true );
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
    return std::make_tuple( successful, items );
}


const std::tuple<bool, std::unordered_map<int, std::unordered_map<int, int>>> DbQuery::getDaytimeCounts(const QString& web_server, const QString& log_type, const QString& from_year_, const QString& from_month_, const QString& from_day_, const QString& to_year_, const QString& to_month_, const QString& to_day_, const QString& log_field, const QString& field_filter )
{
    bool successful = true;
    std::unordered_map<int, std::unordered_map<int, int>> data = {
        {0,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {1,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {2,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {3,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {4,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {5,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {6,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {7,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {8,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {9,  {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {10, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {11, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {12, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {13, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {14, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {15, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {16, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {17, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {18, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {19, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {20, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {21, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
        {22, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},  {23, {{10,0},{20,0},{30,0},{40,0},{50,0},{60,0}}},
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
            DialogSec::errGeneric( nullptr, QString("Unexpected WebServer:\n%1").arg( web_server ), true );
        }
        if ( successful == true ) {
            if ( log_type == "Access" ) {
                table += "access";
            } else if ( web_server == "Error" ) {
                table += "error";
            } else {
                // unexpected LogType
                successful = false;
                DialogSec::errGeneric( nullptr, QString("Unexpected LogType:\n%1").arg( web_server ), true );
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
                DialogSec::errGeneric( nullptr, QString("An error occured while processing dates"), true );
            }
        }
        if ( successful == true ) {
            // build the query statement
            QSqlQuery query = QSqlQuery( db );
            QString stmt;

            // select years
            for ( int year=from_year; year<=to_year; year++ ) {
                stmt = QString("SELECT %1, year, month, day, hour, minute FROM %2 WHERE year=%3")
                    .arg( this->LogFields_to_DbFields.value( log_field ))
                    .arg( table )
                    .arg( year );

                for ( int month=from_month; month<=to_month; month++ ) {


                }


                // quary the database
                if ( query.exec( stmt.replace("'","''") ) == false ) {
                    // error querying database
                    successful = false;
                    DialogSec::errDatabaseFailedExecuting( nullptr, this->db_name, query.lastQuery(), query.lastError().text() );

                } else {
                    try {
                        // get data
                        while ( query.next() ) {

                        }
                    } catch (...) {
                        // something failed
                        successful = false;
                        DialogSec::errGeneric( nullptr, QString("An error occured while processing"), true );
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
    return std::make_tuple( successful, data );
}


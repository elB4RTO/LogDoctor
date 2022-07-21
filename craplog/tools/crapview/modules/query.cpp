
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
}


//select distinct hour from apache_access where year=2021 and month=6;

const std::tuple<const bool, const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>> DbQuery::refreshDates()
{
    bool successful = true;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>> dates = {
        {11,{ {1,{}},{2,{}} }}, {12,{ {1,{}},{2,{}} }}, {13,{ {1,{}},{2,{}} }} };

    QString db_name = QString::fromStdString( this->db_path.substr( this->db_path.find_last_of( '/' ) + 1 ) );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QString::fromStdString( this->db_path ));

    if ( db.open() == false ) {
        // error opening database
        successful = false;
        QString err_msg = "";
        if ( this->dialog_level == 2 ) {
            err_msg = db.lastError().text();
        }
        DialogSec::errDatabaseFailedOpening( nullptr, db_name, err_msg );

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
                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, Y_query.lastQuery(), Y_query.lastError().text() );
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
                        DialogSec::errDatabaseFailedExecuting( nullptr, db_name, M_query.lastQuery(), M_query.lastError().text() );
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
                                DialogSec::errDatabaseFailedExecuting( nullptr, db_name, D_query.lastQuery(), D_query.lastError().text() );
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

/*const std::vector<QString> DbQuery::getYears( const QString& web_server, const QString& log_type )
{

}

const std::vector<QString> DbQuery::getMonths( const QString& web_server, const QString& log_type, const QString& year )
{

}

const std::vector<QString> DbQuery::getDays( const QString& web_server, const QString& log_type, const QString& year, const QString& month )
{

}*/

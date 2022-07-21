
#include "crapview.h"

#include <iostream> // !!! REMOVE !!!


Crapview::Crapview()
{

}


const int Crapview::getDialogLevel()
{
    return this->dialog_level;
}
void Crapview::setDialogLevel( const int new_level )
{
    this->dialog_level = new_level;
}


void Crapview::setDbPath( const std::string& path )
{
    this->dbQuery.setDbPath( path );
}


void Crapview::refreshDates()
{
    auto result = this->dbQuery.refreshDates();
    if ( std::get<0>(result) == true ) {
        this->dates = std::get<1>(result);
    }
}



const QStringList Crapview::getYears( const QString& web_server, const QString& logs_type )
{
    QStringList years;
    for ( const auto& [year, data] : this->dates.at( this->WebServer_s2i.value( web_server ) ).at( this->LogsType_s2i.value( logs_type ) ) ) {
        years.push_back( QString::fromStdString( std::to_string( year ) ) );
    }
    return years;
}
const QStringList Crapview::getMonths( const QString& web_server, const QString& logs_type, const QString& year )
{
    QStringList months;
    for ( const auto& [month, data] : this->dates.at( this->WebServer_s2i.value( web_server ) ).at( this->LogsType_s2i.value( logs_type )).at( year.toInt() ) ) {
        months.push_back( Months_i2s.value( month ) );
    }
    return months;
}
const QStringList Crapview::getDays( const QString& web_server, const QString& logs_type, const QString& year, const QString& month )
{
    QStringList days;
    for ( const int day : this->dates.at( this->WebServer_s2i.value( web_server ) ).at( this->LogsType_s2i.value( logs_type )).at( year.toInt() ).at( this->Months_s2i.value( month ) ) ) {
        days.push_back( QString::fromStdString( std::to_string( day ) ) );
    }
    return days;
}

const QStringList Crapview::getFields( const QString& tab, const QString& logs_type )
{
    return this->fields.value( tab ).value( logs_type );
}

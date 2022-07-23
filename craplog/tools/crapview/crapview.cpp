
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


int Crapview::getMonthNumber( const QString& month_str )
{
    return this->Months_s2i.value( month_str );
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


////////////////
//// CHARTS ////
////////////////
void Crapview::drawSpeed(QTableWidget* table, QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& protocol, const QString& method, const QString& uri, const QString& query, const QString& response )
{

}



void Crapview::drawCount(QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& year, const QString& month, const QString& day, const QString& field )
{
    auto result = this->dbQuery.getItemsCount( web_server, log_type, year, month, day, field );
    if ( std::get<0>(result) == true ) {
        // get data
        std::vector<std::tuple<QString, int>> aux_items = std::get<1>(result);
        std::vector<std::tuple<QString, int>> items;
        items.reserve( 32 );

        // cut off exdceeding elements for the chart
        const int max_items=15;
        int oth_count=0;
        for ( int i=0; i<aux_items.size(); i++ ) {
            if ( i >= max_items ) {
                oth_count += std::get<1>( aux_items.at(i) );
            } else {
                items.push_back( aux_items.at( i ) );
            }
        }
        aux_items.clear();

        // draw the chart
        QPieSeries *pie = new QPieSeries();
        for ( const auto& item : items ) {
            pie->append( std::get<0>(item), std::get<1>(item) );
        }

        if ( oth_count > 0 ) {
            pie->append( "Others", oth_count );
            QPieSlice *slice = pie->slices().at( pie->count()-1 );
            slice->setBrush( Qt::gray );
        }
        pie->setLabelsVisible();

        QChart *p_chart = new QChart();
        p_chart->addSeries( pie );
        p_chart->setTitle( field );
        p_chart->setTitleFont( fonts.at( "main_big" ) );
        p_chart->legend()->hide();
        //p_chart->setTheme( QChart::ChartTheme::ChartThemeDark );

        chart->setChart( p_chart );
        chart->setRenderHint( QPainter::Antialiasing );
    }
}



void Crapview::drawDay(QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& from_year, const QString& from_month, const QString& from_day, const QString& to_year, const QString& to_month, const QString& to_day, const QString& field , const QString& filter )
{
    auto result = this->dbQuery.getDaytimeCounts(
        web_server, log_type,
        from_year, from_month, from_day,
        to_year, to_month, to_day,
        field, filter );
    if ( std::get<0>(result) == true ) {
        // get data
        // { hour : { 10th_minutes : count } }
        const std::unordered_map<int, std::unordered_map<int, int>> items = std::get<1>(result);

        // draw the chart
        QColor col = Qt::GlobalColor::darkGreen;
        QBarSet *b_10 = new QBarSet( "" );
        b_10->setColor( col );
        QBarSet *b_20 = new QBarSet( "" );
        b_20->setColor( col );
        QBarSet *b_30 = new QBarSet( " 10 minutes gap per hour" );
        b_30->setColor( col );
        QBarSet *b_40 = new QBarSet( "" );
        b_40->setColor( col );
        QBarSet *b_50 = new QBarSet( "" );
        b_50->setColor( col );
        QBarSet *b_60 = new QBarSet( "" );
        b_60->setColor( col );

        int count, max_count=0;
        for ( int h=0; h<24; h++ ) {
            auto& data = items.at( h );
            count = data.at( 10 );
            *b_10 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 20 );
            *b_20 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 30 );
            *b_30 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 40 );
            *b_40 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 50 );
            *b_50 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 60 );
            *b_60 << count;
            if ( count > max_count ) {
                max_count = count;
            }
        }

        QBarSeries *bars = new QBarSeries();
        bars->append( b_10 ); bars->append( b_20 ); bars->append( b_30 );
        bars->append( b_40 ); bars->append( b_50 ); bars->append( b_60 );
        bars->setBarWidth( 1 );

        QChart *t_chart = new QChart();
        t_chart->addSeries( bars );
        t_chart->setTitle( QString("Time of Day Count: %1").arg( field ) );
        t_chart->setTitleFont( fonts.at("main") );
        t_chart->legend()->setFont( fonts.at("main_small") );
        t_chart->legend()->setVisible( false );
        t_chart->setAnimationOptions( QChart::SeriesAnimations );
        //t_chart->setBackgroundBrush( Qt::darkGray );

        QStringList categories;
        categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                   << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append( categories );
        axisX->setLabelsFont( fonts.at( "main_small" ) );
        t_chart->addAxis( axisX, Qt::AlignBottom );
        bars->attachAxis( axisX );

        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 16 ) ? max_count : 16 );
        axisY->setRange( 0, max_count );
        axisY->setLabelsFont( fonts.at( "main_small" ) );
        t_chart->addAxis( axisY, Qt::AlignLeft );
        bars->attachAxis( axisY) ;

        t_chart->legend()->setVisible( true );
        t_chart->legend()->setAlignment( Qt::AlignBottom );
        //t_chart->setTheme( QChart::ChartTheme::ChartThemeBrownSand );

        chart->setChart( t_chart );
        chart->setRenderHint( QPainter::Antialiasing );
    }
}



void Crapview::drawRelat(QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& from_year, const QString& from_month, const QString& from_day, const QString& to_year, const QString& to_month, const QString& to_day, const QString& field_1, const QString& filter_1, const QString& field_2, const QString& filter_2 )
{

}

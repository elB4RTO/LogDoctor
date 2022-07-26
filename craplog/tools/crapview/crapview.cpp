
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


const QString Crapview::printableDate( const QString& year, const int month, const QString& day )
{
    QString date = QString("%1-").arg( year );
    if ( month < 10 ) {
        date += QString("0%1-").arg( month );
    } else {
        date += QString("%1-").arg( month );
    }
    if ( day.size() < 2 ) {
        date += QString("0%1").arg( day );
    } else {
        date += day;
    }
    return date;
}


void Crapview::refreshDates()
{
    std::tuple<bool, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>> result;
    this->dbQuery.refreshDates( result );
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
const QStringList Crapview::getHours( const QString& web_server, const QString& logs_type, const QString& year, const QString& month, const QString& day )
{
    return QStringList({"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"});
}

const QStringList Crapview::getFields( const QString& tab, const QString& logs_type )
{
    return this->fields.value( tab ).value( logs_type );
}


////////////////
//// CHARTS ////
////////////////
void drawWarn( QTableWidget* table, QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& hour )
{

}



void Crapview::drawSpeed(QTableWidget* table, QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& protocol, const QString& method, const QString& uri, const QString& query, const QString& response )
{
    std::tuple<bool, std::vector<std::tuple<long long, std::vector<QString>>>> result;
    this->dbQuery.getSpeedData(
        result,
        web_server,
        year, month, day,
        protocol, method, uri, query, response );
    if ( std::get<0>(result) == true ) {
        // get data
        // { hour : { 10th_minutes : count } }
        std::vector<std::tuple<long long, std::vector<QString>>> &items = std::get<1>(result);

        // draw the relational chart
        QLineSeries *line = new QLineSeries();
        line->setName( this->printableDate( year, this->Months_s2i.value(month), day ));

        // build the line upon data
        int i=0, max_i=items.size(), max_t=0, aux;
        long long time /* xD */, aux_time=0, t=0, aux_t, count=1;
        time = std::get<0>(items.at(0));
        QDateTime dt;
        std::vector<QString> data;
        for ( const auto& item : items ) {
            i++;
            // append a value to the chart
            aux_time = std::get<0>(item);
            data = std::get<1>(item);
            aux_t = data.at( 0 ).toLongLong();
            // append only if the second is different, else sum
            if ( aux_time > time ) {
                t = t/count;
                line->append( time, t );
                if ( t > max_t ) {
                    max_t = t;
                }
                time = aux_time;
                t = aux_t;
                count = 1;
                if ( i == max_i ) {
                    // final
                    line->append( time, t );
                    if ( t > max_t ) {
                        max_t = t;
                    }
                }
            } else {
                count ++;
                t += aux_t;
                if ( i == max_i ) {
                    // final
                    t = t/count;
                    line->append( aux_time, t );
                    if ( t > max_t ) {
                        max_t = t;
                    }
                }
            }
            // fill the teble with data
            if ( data.at(0).size() > 0 || data.at(1).size() > 0 || data.at(2).size() > 0 || data.at(3).size() > 0 || data.at(4).size() > 0 || data.at(5).size() > 0 ) {
                aux = table->rowCount();
                table->insertRow( aux );
                table->setItem( aux,  0, new QTableWidgetItem( data.at(0) ));
                table->setItem( aux,  1, new QTableWidgetItem( data.at(1) ));
                table->setItem( aux,  2, new QTableWidgetItem( data.at(2) ));
                table->setItem( aux,  3, new QTableWidgetItem( data.at(3) ));
                table->setItem( aux,  4, new QTableWidgetItem( data.at(4) ));
                table->setItem( aux,  5, new QTableWidgetItem( data.at(5) ));
                dt = QDateTime::fromMSecsSinceEpoch( aux_time );
                table->setItem( aux,  6, new QTableWidgetItem( dt.time().toString("hh:mm:ss") ));
            }
        }
        table->verticalHeader()->setVisible( false );

        // fictitious line
        QLineSeries *line_ = new QLineSeries();

        // color the area
        QColor col1 = Qt::GlobalColor::red,
               col2 = Qt::GlobalColor::green,
               col3 = Qt::GlobalColor::blue;
        //area->setColor( col );
        QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
        gradient.setColorAt(0.3, col1.lighter( 90 ) );
        gradient.setColorAt(0.7, col2.lighter( 90 ) );
        gradient.setColorAt(1.0, col3.lighter( 90 ) );
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

        QPen pen = line->pen();
        pen.setBrush( gradient );
        pen.setWidth( 1 );
        line->setPen(pen);

        pen = line_->pen();
        pen.setBrush( gradient );
        pen.setWidth( 1 );
        line_->setPen(pen);

        // build the chart
        QChart *l_chart = new QChart();
        l_chart->addSeries( line );
        l_chart->addSeries( line_ );
        l_chart->setTitle( this->TITLE_SPEED );
        l_chart->setTitleFont( fonts.at("main") );
        l_chart->legend()->setFont( fonts.at( "main_small" ) );
        l_chart->legend()->setAlignment( Qt::AlignBottom );

        // set-up the date-time axis (X)
        QDateTimeAxis *axisX = new QDateTimeAxis();
        axisX->setLabelsFont( fonts.at( "main_small" ) );
        axisX->setFormat( "hh:mm" );
        axisX->setTickCount( 25 );
        l_chart->addAxis( axisX, Qt::AlignBottom );
        line->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_t < 16 ) ? max_t : 16 );
        axisY->setRange( 0, max_t );
        axisY->setLabelsFont( fonts.at( "main_small" ) );
        l_chart->addAxis( axisY, Qt::AlignLeft );
        line->attachAxis( axisY) ;

        // add the chart to the view
        chart->setChart( l_chart );
        chart->setRenderHint(QPainter::Antialiasing);

        items.clear();
    }
}



void Crapview::drawCount( QTableWidget* table, QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& year, const QString& month, const QString& day, const QString& field )
{
    std::tuple<bool, std::vector<std::tuple<QString, int>>> result;
    this->dbQuery.getItemsCount(
        result,
        web_server, log_type,
        year, month, day,
        field );
    if ( std::get<0>(result) == true ) {
        // get data
        std::vector<std::tuple<QString, int>> &aux_items = std::get<1>(result);

        // make the pie
        QPieSeries *pie = new QPieSeries();
        // cut off exdceeding elements for the chart
        const int max_items=15;
        int aux, count, oth_count=0;
        QString item;
        for ( int i=0; i<aux_items.size(); i++ ) {
            item = std::get<0>( aux_items.at(i) );
            count = std::get<1>( aux_items.at(i) );
            if ( i >= max_items ) {
                oth_count += count;
            } else {
                pie->append( item, count );
                //items.push_back( aux_items.at( i ) );
            }
            aux = table->rowCount();
            table->insertRow( aux );
            table->setItem( aux,  0, new QTableWidgetItem( QString::fromStdString( std::to_string(count) )));
            table->setItem( aux,  1, new QTableWidgetItem( std::get<0>( aux_items.at(i) ) ));
        }
        table->verticalHeader()->setVisible( false );
        aux_items.clear();

        if ( oth_count > 0 ) {
            pie->append( this->TEXT_COUNT_OTHERS, oth_count );
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
    std::tuple<bool, std::unordered_map<int, std::unordered_map<int, int>>> result;
    this->dbQuery.getDaytimeCounts(
        result,
        web_server, log_type,
        from_year, from_month, from_day,
        to_year, to_month, to_day,
        field, filter );
    if ( std::get<0>(result) == true ) {
        // get data
        // { hour : { 10th_minutes : count } }
        std::unordered_map<int, std::unordered_map<int, int>> &items = std::get<1>(result);

        // draw the chart
        QColor col = Qt::GlobalColor::darkGreen;
        QBarSet *b_10 = new QBarSet( "" );
        b_10->setColor( col );
        QBarSet *b_20 = new QBarSet( "" );
        b_20->setColor( col );
        QBarSet *b_30 = new QBarSet( this->LEGEND_DAY );
        b_30->setColor( col );
        QBarSet *b_40 = new QBarSet( "" );
        b_40->setColor( col );
        QBarSet *b_50 = new QBarSet( "" );
        b_50->setColor( col );
        QBarSet *b_60 = new QBarSet( "" );
        b_60->setColor( col );

        // build the bars upon data
        int count, max_count=0;
        for ( int h=0; h<24; h++ ) {
            auto& data = items.at( h );
            count = data.at( 0 );
            *b_10 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 10 );
            *b_20 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 20 );
            *b_30 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 30 );
            *b_40 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 40 );
            *b_50 << count;
            if ( count > max_count ) {
                max_count = count;
            }
            count = data.at( 50 );
            *b_60 << count;
            if ( count > max_count ) {
                max_count = count;
            }
        }

        // apply the bars to the bars serie
        QBarSeries *bars = new QBarSeries();
        bars->append( b_10 ); bars->append( b_20 ); bars->append( b_30 );
        bars->append( b_40 ); bars->append( b_50 ); bars->append( b_60 );
        bars->setBarWidth( 1 );

        // set-up the bars chart
        QChart *b_chart = new QChart();
        b_chart->addSeries( bars );
        b_chart->setTitle( QString("%1: %2").arg( this->TITLE_DAY ).arg( field ) );
        b_chart->setTitleFont( fonts.at("main") );
        b_chart->legend()->setFont( fonts.at("main_small") );
        //b_chart->legend()->setVisible( true );
        b_chart->legend()->setAlignment( Qt::AlignBottom );
        b_chart->setAnimationOptions( QChart::SeriesAnimations );
        //b_chart->setTheme( QChart::ChartTheme::ChartThemeBrownSand );
        //t_chart->setBackgroundBrush( Qt::darkGray );

        // craft the X-axis labels
        QStringList categories;
        categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                   << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

        // set-up the time-of-day axis (X)
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append( categories );
        axisX->setLabelsFont( fonts.at( "main_small" ) );
        b_chart->addAxis( axisX, Qt::AlignBottom );
        bars->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 16 ) ? max_count : 16 );
        axisY->setRange( 0, max_count );
        axisY->setLabelsFont( fonts.at( "main_small" ) );
        b_chart->addAxis( axisY, Qt::AlignLeft );
        bars->attachAxis( axisY) ;

        // apply the chart to the view
        chart->setChart( b_chart );
        chart->setRenderHint( QPainter::Antialiasing );

        items.clear();
    }
}



void Crapview::drawRelat(QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& from_year, const QString& from_month, const QString& from_day, const QString& to_year, const QString& to_month, const QString& to_day, const QString& field_1, const QString& filter_1, const QString& field_2, const QString& filter_2 )
{
    bool period = true;
    std::tuple<bool, std::vector<std::tuple<long long, int>>> result;
    if ( from_year == to_year
      && from_month == to_month
      && from_day == to_day ) {
        period = false;
        this->dbQuery.getRelationalCountsDay(
            result,
            web_server, log_type,
            from_year, from_month, from_day,
            field_1, filter_1,
            field_2, filter_2 );
    } else {
        this->dbQuery.getRelationalCountsPeriod(
            result,
            web_server, log_type,
            from_year, from_month, from_day,
            to_year, to_month, to_day,
            field_1, filter_1,
            field_2, filter_2 );
    }

    if ( std::get<0>(result) == true ) {
        // get data
        // { hour : { 10th_minutes : count } }
        std::vector<std::tuple<long long, int>> &items = std::get<1>(result);

        // draw the relational chart
        QLineSeries *line = new QLineSeries();
        // build the line upon data
        int count, max_count=0;
        long long time; // xD
        for ( const auto& item : items ) {
            time  = std::get<0>(item);
            count = std::get<1>(item);
            line->append( time, (double)count );
            //*line << QPoint( time, (double)count );
            if ( count > max_count ) {
                max_count = count;
            }
        }

        // build the area
        QAreaSeries *area  = new QAreaSeries( line );
        if ( period == false ) {
            area->setName( this->printableDate( from_year, this->Months_s2i.value(from_month), from_day ));
        } else {
            area->setName(QString("%1 %2 %3 %4")
                .arg( this->LEGEND_RELAT_FROM )
                .arg( this->printableDate( from_year, this->Months_s2i.value(from_month), from_day ))
                .arg( this->LEGEND_RELAT_TO )
                .arg( this->printableDate( to_year, this->Months_s2i.value(to_month), to_day )));
        }

        // color the area
        QColor col1 = Qt::GlobalColor::red,
               col2 = Qt::GlobalColor::yellow,
               col3 = Qt::GlobalColor::magenta;
        //area->setColor( col );
        QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
        gradient.setColorAt(0.25, col3.lighter( 70 ) );
        gradient.setColorAt(0.5,  col1.lighter( 80 ) );
        gradient.setColorAt(1.0,  col2.lighter( 90 ) );
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        area->setBrush( gradient );
        area->setBorderColor( col1.lighter( 50 ) );

        // fictitious area, just to have another label
        QAreaSeries *area_ = new QAreaSeries( );
        area_->setBrush( gradient );
        area_->setBorderColor( col1.lighter( 50 ) );

        // build the area
        QChart *a_chart = new QChart();
        a_chart->addSeries( area );
        a_chart->addSeries( area_ );
        a_chart->setTitle( QString("%1: %2 -> %3").arg(this->TITLE_RELAT).arg(field_1).arg(field_2) );
        a_chart->setTitleFont( fonts.at("main") );
        a_chart->legend()->setFont( fonts.at( "main_small" ) );
        a_chart->legend()->setAlignment( Qt::AlignBottom );

        // set-up the date-time axis (X)
        QDateTimeAxis *axisX = new QDateTimeAxis();
        axisX->setLabelsFont( fonts.at( "main_small" ) );
        if ( period == true ) {
            //axisX->setLabelsVisible( false );
            axisX->setFormat( "yyyy-MM" );
            int ticks = this->dbQuery.getMonthsCount( from_year, from_month, to_year, to_month );
            if ( ticks == 1 ) {
                axisX->setFormat( "yyyy-MM-dd" );
                ticks = to_day.toInt() - from_day.toInt() +2;
            }
            axisX->setTickCount( ticks+1 );
        } else {
            axisX->setFormat( "hh:mm" );
            axisX->setTickCount( 25 );
        }
        a_chart->addAxis( axisX, Qt::AlignBottom );
        area->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 16 ) ? max_count : 16 );
        axisY->setRange( 0, max_count );
        axisY->setLabelsFont( fonts.at( "main_small" ) );
        a_chart->addAxis( axisY, Qt::AlignLeft );
        area->attachAxis( axisY) ;

        // add the chart to the view
        chart->setChart( a_chart );
        chart->setRenderHint(QPainter::Antialiasing);

        items.clear();
    }
}

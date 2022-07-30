
#include "crapview.h"
#include "utilities/strings.h"

#include <QGraphicsItem>
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


const QString Crapview::printableDate( const int year, const int month, const int day )
{
    QString date;
    if ( year < 10 ) {
        date += QString("0%1:").arg( year );
    } else {
        date += QString("%1:").arg( year );
    }
    if ( month < 10 ) {
        date += QString("0%1:").arg( month );
    } else {
        date += QString("%1:").arg( month );
    }
    if ( day < 10 ) {
        date += QString("0%1").arg( day );
    } else {
        date += QString("%1").arg( day );
    }
    return date;
}


const QString Crapview::printableTime( const int hour, const int minute, const int second )
{
    QString time;
    if ( hour < 10 ) {
        time += QString("0%1:").arg( hour );
    } else {
        time += QString("%1:").arg( hour );
    }
    if ( minute < 10 ) {
        time += QString("0%1:").arg( minute );
    } else {
        time += QString("%1:").arg( minute );
    }
    if ( second < 10 ) {
        time += QString("0%1").arg( second );
    } else {
        time += QString("%1").arg( second );
    }
    return time;
}


const QStringList Crapview::getWarnHeader( const QString& log_type )
{
    QStringList header;
    if ( log_type == TYPES.value(1) ) {
        header = QStringList({
            FIELDS.value(0),
            this->DATE,this->TIME,
            FIELDS.value(10),FIELDS.value(11),FIELDS.value(12),FIELDS.value(13),FIELDS.value(14),
            FIELDS.value(18),FIELDS.value(22),FIELDS.value(21),FIELDS.value(20),
            FIELDS.value(17),FIELDS.value(16),FIELDS.value(15),"rowid" });
    } else if ( log_type == TYPES.value(2) ) {
        header = QStringList({
            FIELDS.value(0),
            this->DATE,this->TIME,
            FIELDS.value(31),FIELDS.value(32),FIELDS.value(33),
            FIELDS.value(30),FIELDS.value(20),"rowid" });
    } else {
        // unexpected LogType
        throw ("Unexpected LogType: "+log_type.toStdString() );
    }
    return header;
}


const QString Crapview::printableWarn( const int value )
{
    if ( value == 0 ) {
        return "FALSE";
    } else {
        return "TRUE";
    }
}


const QString Crapview::parseBooleanFilter( const QString& filter_str )
{
    QString aux = filter_str;
    aux = aux.replace( "TRUE", "1", Qt::CaseSensitivity::CaseInsensitive );
    aux = aux.replace( "FALSE","0", Qt::CaseSensitivity::CaseInsensitive );
    return this->parseNumericFilter( aux );
}


const QString Crapview::parseNumericFilter( const QString& filter_str )
{
    QString final_str = "";
    if ( filter_str.size() > 0 ) {
        QString aux = this->parseTextualFilter( filter_str );
        if ( aux == "NULL" || aux == "NOT NULL" ) {
            final_str = aux;
        } else {
            std::vector<std::string> f_list;
            StringOps::splitrip( f_list, filter_str.toStdString(), " " );
            if ( f_list.size() > 0 ) {
                std::string& aux = f_list.at(0);
                if ( StringOps::isNumeric( aux ) == true ) {
                    // no symbol specified, set '=' as default
                    final_str += "=";
                    final_str += QString::fromStdString( aux );
                } else {
                    if ( StringOps::isNumeric( StringOps::lstrip( aux, "<=>" ) ) == true ) {
                        // symbol/value
                        final_str += QString::fromStdString( aux ).replace("==","=");
                    } else if ( StringOps::lstrip( aux, "<=>" ).size() == 0 ) {
                        // symbol at first, maybe a value follows
                        if ( f_list.size() > 1 ) {
                            final_str += QString::fromStdString( aux ).replace("==","=");
                            int ck = final_str.size();
                            for ( int i=1; i<f_list.size(); i++ ) {
                                aux = f_list.at( i );
                                if ( StringOps::isNumeric( aux ) ) {
                                    final_str += QString::fromStdString( aux );
                                    break;
                                }
                            }
                            if ( final_str.size() == ck ) {
                                final_str = "";
                            }
                        }

                    }/* else {
                        // skip
                    }*/
                }
            }
            f_list.clear();
        }
    }
    return final_str.replace("==","=");
}


const QString Crapview::parseTextualFilter( const QString& filter_str )
{
    QString aux = filter_str;
    if ( filter_str.size() > 0 ) {
        std::string str = StringOps::strip( filter_str.toUpper().toStdString() );
        if ( str == "NULL" ) {
            aux = "NULL";
        } else if ( str == "NOT NULL" || str == "*" ) {
            aux = "NOT NULL";
        }
    }
    return aux;
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
const QStringList Crapview::getHours()
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
void Crapview::updateWarn( QTableWidget* table , const QString& web_server, const QString& log_type )
{
    std::vector<std::tuple<int, int>> updates; // { (rowid, warn) }
    for ( int i=0; i<table->rowCount(); i++ ) {
        QTableWidgetItem* item = table->item( i, 0 );
        if ( item->checkState() == Qt::CheckState::Checked && item->text() == "FALSE" ) {
            // remove warning
            updates.push_back( std::make_tuple( table->item( i, table->columnCount()-1 )->text().toInt(), 1 ) );
        } else if (item->checkState() == Qt::CheckState::Unchecked && item->text() == "TRUE" ) {
            // add warning
            updates.push_back( std::make_tuple( table->item( i, table->columnCount()-1 )->text().toInt(), 0 ) );
        }
    }
    this->dbQuery.updateWarnings( web_server, log_type, updates );
    updates.clear();
}

void Crapview::drawWarn( QTableWidget* table, QtCharts::QChartView* chart, const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type, const QString& year, const QString& month, const QString& day, const QString& hour )
{
    std::tuple<bool, std::vector<std::vector<std::vector<std::vector<QString>>>>> result;
    this->dbQuery.getWarnCounts(
        result,
        web_server, log_type,
        year, month, day, hour );
    if ( std::get<0>(result) == true ) {
        // get data
        // { hour : { 10th_minutes : count } }
        std::vector<std::vector<std::vector<std::vector<QString>>>> &items = std::get<1>(result);

        // bars
        std::vector<std::vector<QBarSet*>> sets;

        // table
        QStringList header_labels = this->getWarnHeader( log_type );
        table->setColumnCount( header_labels.size() );
        table->setHorizontalHeaderLabels( header_labels );

        // build the bars and the table upon data
        QColor warn_col = QColor( 255, 140, 0, 255 );
        int norm_count, warn_count, sum_count, max_count=0, aux;
        if ( hour.size() == 0 ) {
            // entire day
            for ( int i=0; i<6; i++ ) {
                sets.push_back( std::vector<QBarSet*>() );
                sets.at( i ).push_back( new QBarSet("") );
                sets.at( i ).push_back( new QBarSet("") );
            }
            for ( int h=0; h<24; h++ ) {
                for ( int m=0; m<6; m++ ) {
                    auto& data = items.at( h ).at( m );
                    norm_count = warn_count = 0;
                    for ( const std::vector<QString>& line : data ) {
                        aux = table->rowCount();
                        table->insertRow( aux );
                        table->setItem( aux, 0, new QTableWidgetItem( this->printableWarn( line.at( 0 ).toInt() )));
                        if ( line.at( 0 ).toInt() != 0 ) {
                            table->item( aux, 0 )->setForeground( warn_col );
                            table->item( aux, 0 )->setCheckState( Qt::CheckState::Checked );
                        } else {
                            table->item( aux, 0 )->setCheckState( Qt::CheckState::Unchecked );
                        }
                        table->setItem( aux, 1, new QTableWidgetItem( this->printableDate( line.at( 1 ).toInt(), line.at( 2 ).toInt(), line.at( 3 ).toInt() )));
                        table->setItem( aux, 2, new QTableWidgetItem( this->printableTime( line.at( 4 ).toInt(), line.at( 5 ).toInt(), line.at( 6 ).toInt() )));
                        for ( int i=7; i<line.size(); i++ ) {
                            table->setItem( aux, i-4, new QTableWidgetItem( line.at( i ) ));
                        }
                        if ( line.at( 0 ) == "0" ) {
                            norm_count ++;
                        } else {
                            warn_count ++;
                        }
                    }
                    sets.at( m ).at( 0 )->append( norm_count );
                    sets.at( m ).at( 1 )->append( warn_count );
                    sum_count = norm_count + warn_count;
                    if ( sum_count > max_count ) {
                        max_count = sum_count;
                    }
                }
            }
            sets.at( 2 ).at( 1 )->setLabel( this->printableDate( year, this->getMonthNumber( month ), day ) );
        } else {
            // 1 hour
            for ( int i=0; i<10; i++ ) {
                sets.push_back( std::vector<QBarSet*>() );
                sets.at( i ).push_back( new QBarSet("") );
                sets.at( i ).push_back( new QBarSet("") );
            }
            for ( int g=0; g<6; g++ ) {
                for ( int m=0; m<10; m++ ) {
                    auto& data = items.at( g ).at( m );
                    norm_count = warn_count = 0;
                    for ( const std::vector<QString>& line : data ) {
                        aux = table->rowCount();
                        table->insertRow( aux );
                        table->setItem( aux, 0, new QTableWidgetItem( this->printableWarn( line.at( 0 ).toInt() )));
                        if ( line.at( 0 ).toInt() != 0 ) {
                            table->item( aux, 0 )->setForeground( warn_col );
                            table->item( aux, 0 )->setCheckState( Qt::CheckState::Checked );
                        } else {
                            table->item( aux, 0 )->setCheckState( Qt::CheckState::Unchecked );
                        }
                        table->setItem( aux, 1, new QTableWidgetItem( this->printableDate( line.at( 1 ).toInt(), line.at( 2 ).toInt(), line.at( 3 ).toInt() )));
                        table->setItem( aux, 2, new QTableWidgetItem( this->printableTime( line.at( 4 ).toInt(), line.at( 5 ).toInt(), line.at( 6 ).toInt() )));
                        for ( int i=7; i<line.size(); i++ ) {
                            table->setItem( aux, i-4, new QTableWidgetItem( line.at( i ) ));
                        }
                        if ( line.at( 0 ) == "0" ) {
                            norm_count ++;
                        } else {
                            warn_count ++;
                        }
                    }
                    sets.at( m ).at( 0 )->append( norm_count );
                    sets.at( m ).at( 1 )->append( warn_count );
                    sum_count = norm_count + warn_count;
                    if ( sum_count > max_count ) {
                        max_count = sum_count;
                    }
                }
            }
            sets.at( 4 ).at( 1 )->setLabel( this->printableDate( year, this->getMonthNumber( month ), day ) + ", h " + hour );
        }
        table->verticalHeader()->setVisible( false );

        // apply the colors and append to the series
        QColor cols[] = {QColor(127,127,127), QColor(237,80,61)};
        //QColor cols[] = {QColor(18,175,194), QColor(237,80,61)};
        std::vector<QStackedBarSeries*> b_series;
        for ( int i=0; i<sets.size(); i++ ) {
            auto& set = sets.at( i );
            b_series.push_back( new QStackedBarSeries() );
            for ( int w=0; w<2; w++ ) {
                QBarSet* b = set.at( w );
                b->setColor( cols[ w ] );
                b_series.at( i )->append( b );
            }
        }
        for ( auto *s : b_series ) {
            s->setBarWidth( 1 );
        }

        // set-up the bars chart
        QChart *b_chart = new QChart();
        foreach ( auto& bars, b_series ) {
            b_chart->addSeries( bars );
        }
        //b_chart->setTitle( QString("%1: %2").arg( this->TITLE_WARN ) );
        b_chart->setTitle( this->TITLE_WARN );
        b_chart->setTitleFont( fonts.at("main") );
        //b_chart->legend()->setVisible( false );
        b_chart->legend()->setFont( fonts.at("main_small") );
        b_chart->legend()->setAlignment( Qt::AlignBottom );
        b_chart->setAnimationOptions( QChart::SeriesAnimations );
        //b_chart->setTheme( QChart::ChartTheme::ChartThemeBrownSand );
        //t_chart->setBackgroundBrush( Qt::darkGray );

        // craft the X-axis labels
        QStringList categories;
        if ( hour.size() == 0 ) {
            categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                       << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";
        } else {
            categories << "00" << "10" << "20" << "30" << "40" << "50";
        }

        // set-up the time-of-day axis (X)
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append( categories );
        axisX->setLabelsFont( fonts.at( "main_small" ) );
        b_chart->addAxis( axisX, Qt::AlignBottom );
        for ( auto *s : b_series ) {
            s->attachAxis( axisX );
        }

        // set-up the count values axis (Y)
        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 16 ) ? max_count : 16 );
        axisY->setRange( 0, max_count );
        axisY->setLabelsFont( fonts.at( "main_small" ) );
        b_chart->addAxis( axisY, Qt::AlignLeft );
        for ( auto *s : b_series ) {
            s->attachAxis( axisY );
        }

        // apply the chart to the view
        chart->setChart( b_chart );
        chart->setRenderHint( QPainter::Antialiasing );

        items.clear();
    }
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
        QBarSet *b_30 = new QBarSet( "" );
        if ( to_year.size() == 0 || to_month.size() == 0 || to_day.size() == 0 ) {
            b_30->setLabel( this->printableDate( from_year, this->Months_s2i.value(from_month), from_day ) );
        } else {
            b_30->setLabel( QString("%1 %2 %3 %4")
                .arg( this->LEGEND_FROM )
                .arg( this->printableDate( from_year, this->Months_s2i.value(from_month), from_day ))
                .arg( this->LEGEND_TO )
                .arg( this->printableDate( to_year, this->Months_s2i.value(to_month), to_day )) );
        }
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
                .arg( this->LEGEND_FROM )
                .arg( this->printableDate( from_year, this->Months_s2i.value(from_month), from_day ))
                .arg( this->LEGEND_TO )
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


// calculate global informations
void Crapview::calcGlobals( const std::unordered_map<std::string, QFont>& fonts, const QString& web_server, const QString& log_type )
{

}

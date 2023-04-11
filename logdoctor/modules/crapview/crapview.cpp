
#include "crapview.h"

#include "utilities/printables.h"
#include "utilities/strings.h"

#include <QGraphicsItem>
#include <QTableWidget>


const int& Crapview::getDialogsLevel() const
{
    return this->dialogs_level;
}
void Crapview::setDialogsLevel( const int new_level )
{
    this->dialogs_level = new_level;
}


void Crapview::setDbPath( const std::string& path )
{
    this->dbQuery.setDbPath( path + "/collection.db" );
}


const QString Crapview::parseBooleanFilter( const QString& filter_str ) const
{
    QString aux{ filter_str };
    aux = aux.replace( "NOT TRUE",  "!= 1", Qt::CaseSensitivity::CaseInsensitive );
    aux = aux.replace( "NOT FALSE", "!= 0", Qt::CaseSensitivity::CaseInsensitive );
    aux = aux.replace( "TRUE",  "1", Qt::CaseSensitivity::CaseInsensitive );
    aux = aux.replace( "FALSE", "0", Qt::CaseSensitivity::CaseInsensitive );
    return this->parseNumericFilter( aux );
}


const QString Crapview::parseNumericFilter( const QString& filter_str ) const
{
    QString final_str;
    if ( ! filter_str.isEmpty() ) {
        QString aux_{ this->parseTextualFilter( filter_str ) };
        if ( aux_ == "NULL" || aux_ == "NOT NULL" ) {
            final_str = aux_;
        } else {
            std::vector<std::string> f_list;
            StringOps::splitrip( f_list, filter_str.toStdString(), ' ' );
            if ( ! f_list.empty() ) {
                std::string& aux{ f_list.at(0) };
                if ( StringOps::isNumeric( aux ) ) {
                    // no symbol specified, set '=' as default
                    final_str += "=";
                    final_str += QString::fromStdString( aux );
                } else {
                    if ( StringOps::isNumeric( StringOps::lstrip( aux, "!<=>" ) ) ) {
                        // symbol/value
                        final_str += QString::fromStdString( aux ).replace("==","=");
                    } else if ( StringOps::lstrip( aux, "!<=>" ).empty() ) {
                        // symbol at first, maybe a value follows
                        if ( f_list.size() > 1 ) {
                            final_str += QString::fromStdString( aux ).replace("==","=");
                            const size_t ck = final_str.size();
                            const size_t max = f_list.size();
                            for ( size_t i{1}; i<max; i++ ) {
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


const QString Crapview::parseTextualFilter( const QString& filter_str ) const
{
    QString aux{ filter_str };
    if ( ! filter_str.isEmpty() ) {
        const std::string str{ StringOps::strip( filter_str.toUpper().toStdString() ) };
        if ( str == "NULL" ) {
            aux = "NULL";
        } else if ( str == "NOT NULL" || str == "*" ) {
            aux = "NOT NULL";
        }
    }
    return aux;
}



const QString Crapview::getLogFieldString ( const int field_id ) const
{
    return TR::tr( this->dbQuery.FIELDS.at( field_id ).c_str() );
}

const int Crapview::getLogFieldID ( const QString& field_str ) const
{
    int f{ 0 };
    for ( const auto& [id,str] : this->dbQuery.FIELDS ) {
        if ( TR::tr(str.c_str()) == field_str ) {
            f = id;
            break;
        }
    }
    return f;
}


const int Crapview::getMonthNumber( const QString& month_str ) const
{
    int m{ 0 };
    for ( const auto& [num,str] : this->dbQuery.MONTHS ) {
        if ( TR::tr(str.c_str()) == month_str ) {
            m = num;
            break;
        }
    }
    return m;
}


void Crapview::refreshDates()
{
    Result<stats_dates_t> result;
    this->dbQuery.refreshDates( result );
    if ( result ) {
        this->dates.clear();
        // std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
        // { web_server_id : { year : { month : [ days ] } } }
        this->dates = std::move( result.getData() );
    }
}
void Crapview::clearDates()
{
    this->dates.clear();
}

const QStringList Crapview::getYears( const QString& web_server ) const
{
    QStringList years;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( ! this->dates.at( ws ).empty() ) {
            const auto& y{ this->dates.at( ws ) };
            std::transform( y.cbegin(), y.cend(),
                            std::back_inserter( years ),
                            [](const auto& p){ return QString::number( p.first ); } );
        }
    }
    return years;
}
const QStringList Crapview::getMonths( const QString& web_server, const QString& year ) const
{
    QStringList months;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( ! this->dates.at( ws ).empty() ) {
            const int y{ year.toInt() };
            if ( ! this->dates.at( ws ).at( y ).empty() ) {
                const auto& m{ this->dates.at( ws ).at( y ) };
                const auto& m_tr{ this->dbQuery.MONTHS };
                std::transform( m.cbegin(), m.cend(),
                                std::back_inserter( months ),
                                [&m_tr](const auto& p){ return TR::tr(m_tr.at( p.first ).c_str()); } );
            }
        }
    }
    return months;
}
const QStringList Crapview::getDays( const QString& web_server, const QString& year, const QString& month ) const
{
    QStringList days;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( ! this->dates.at( ws ).empty() ) {
            const int y{ year.toInt() };
            if ( ! this->dates.at( ws ).at( y ).empty() ) {
                const int m{ this->getMonthNumber( month ) };
                if ( ! this->dates.at( ws ).at( y ).at( m ).empty() ) {
                    const auto& d{ this->dates.at( ws ).at( y ).at( m ) };
                    std::transform( d.cbegin(), d.cend(),
                                    std::back_inserter( days ),
                                    [](const int day){ return QString::number( day ); } );
                }
            }
        }
    }
    return days;
}
const QStringList Crapview::getHours() const
{
    return QStringList{"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
}

const QStringList Crapview::getFields( const std::string& tab ) const
{
    QStringList list;
    const auto& f{ this->fields.at( tab ) };
    std::transform( f.cbegin(), f.cend(),
                    std::back_inserter( list ),
                    [](const auto& field){ return TR::tr( field.c_str() ); } );
    return list;
}


////////////////
//// CHARTS ////
////////////////
void Crapview::sliceClicked( QtCharts::QPieSlice* slice )
{
    slice->setExploded( !slice->isExploded() );
    slice->setLabelVisible( !slice->isLabelVisible() );
}


void Crapview::updateWarn( QTableWidget* table , const QString& web_server ) const
{
    std::vector<std::tuple<int, int>> updates; // { (rowid, warn) }
    for ( int i{0}; i<table->rowCount(); i++ ) {
        QTableWidgetItem* item = table->item( i, 0 );
        if ( item->checkState() == Qt::CheckState::Checked && item->text() == TR::tr( BOOLS__FALSE.c_str() ) ) {
            // remove warning
            updates.push_back( std::make_tuple( table->item( i, table->columnCount()-1 )->text().toInt(), 1 ) );
        } else if (item->checkState() == Qt::CheckState::Unchecked && item->text() == TR::tr( BOOLS__TRUE.c_str() ) ) {
            // add warning
            updates.push_back( std::make_tuple( table->item( i, table->columnCount()-1 )->text().toInt(), 0 ) );
        }
    }
    this->dbQuery.updateWarnings( web_server, updates );
}

void Crapview::drawWarn( QTableWidget* table, QtCharts::QChartView* chart, const QChart::ChartTheme& theme, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& hour ) const
{
    Result<stats_warn_items_t> result;
    this->dbQuery.getWarnCounts(
        result,
        web_server,
        year, month, day, hour );
    if ( result ) {
        // std::vector<std::vector<std::vector<std::vector<QString>>>>
        // day  -> [ hours[ 10th_minutes[ lines[ log_data ] ] ] ]
        // hour -> [ 10th_minutes[ minute[ lines[ log_data ] ] ] ]
        auto& items = result.getData();

        // bars
        std::vector<std::vector<QBarSet*>> sets;

        // build the bars and the table upon data
        QString date;
        QColor warn_col{ 255, 140, 0, 255 };
        int norm_count, warn_count, sum_count, max_count=0, n_rows=0;
        if ( hour.isEmpty() ) {
            // entire day
            for ( int i{0}; i<6; i++ ) {
                sets.push_back( std::vector<QBarSet*>() );
                sets.back().push_back( new QBarSet("") );
                sets.back().push_back( new QBarSet("") );
            }
            for ( size_t h{0}; h<24ul; h++ ) {
                for ( size_t m{0}; m<6ul; m++ ) {
                    const auto& data{ items.at( h ).at( m ) };
                    norm_count = warn_count = 0;
                    for ( const std::vector<QString>& line : data ) {
                        table->insertRow( n_rows );
                        table->setItem( n_rows, 0, new QTableWidgetItem( PrintSec::printableBool( line.at( 0 ).toInt() )));
                        if ( line.at( 0 ).toInt() != 0 ) {
                            table->item( n_rows, 0 )->setForeground( warn_col );
                            table->item( n_rows, 0 )->setCheckState( Qt::CheckState::Checked );
                        } else {
                            table->item( n_rows, 0 )->setCheckState( Qt::CheckState::Unchecked );
                        }
                        table->setItem( n_rows, 1, new QTableWidgetItem( PrintSec::printableDate( line.at( 1 ).toInt(), line.at( 2 ).toInt(), line.at( 3 ).toInt() )));
                        table->setItem( n_rows, 2, new QTableWidgetItem( PrintSec::printableTime( line.at( 4 ).toInt(), line.at( 5 ).toInt(), line.at( 6 ).toInt() )));
                        int col{ 3 };
                        const size_t max{ line.size() };
                        for ( size_t i{7}; i<max; i++ ) {
                            QTableWidgetItem* itm;
                            if ( (col == 7 || col >= 12) && !line.at(i).isEmpty() ) {
                                itm = new QTableWidgetItem();
                                itm->setData( Qt::DisplayRole, line.at( i ).toInt() );
                            } else {
                                itm = new QTableWidgetItem( line.at( i ) );
                            }
                            table->setItem( n_rows, col, itm );
                            col ++;
                        }
                        if ( line.at( 0 ) == "0" ) {
                            norm_count ++;
                        } else {
                            warn_count ++;
                        }
                        n_rows ++;
                    }
                    sets.at( m ).at( 0 )->append( norm_count );
                    sets.at( m ).at( 1 )->append( warn_count );
                    sum_count = norm_count + warn_count;
                    if ( sum_count > max_count ) {
                        max_count = sum_count;
                    }
                }
            }
            date = PrintSec::printableDate( year, this->getMonthNumber( month ), day );
        } else {
            // 1 hour
            for ( int i{0}; i<10; i++ ) {
                sets.push_back( std::vector<QBarSet*>() );
                sets.back().push_back( new QBarSet("") );
                sets.back().push_back( new QBarSet("") );
            }
            for ( size_t g{0}; g<6ul; g++ ) {
                for ( size_t m{0}; m<10ul; m++ ) {
                    const auto& data{ items.at( g ).at( m ) };
                    norm_count = warn_count = 0;
                    for ( const std::vector<QString>& line : data ) {
                        n_rows = table->rowCount();
                        table->insertRow( n_rows );
                        table->setItem( n_rows, 0, new QTableWidgetItem( PrintSec::printableBool( line.at( 0 ).toInt() )));
                        if ( line.at( 0 ).toInt() != 0 ) {
                            table->item( n_rows, 0 )->setForeground( warn_col );
                            table->item( n_rows, 0 )->setCheckState( Qt::CheckState::Checked );
                        } else {
                            table->item( n_rows, 0 )->setCheckState( Qt::CheckState::Unchecked );
                        }
                        table->setItem( n_rows, 1, new QTableWidgetItem( PrintSec::printableDate( line.at( 1 ).toInt(), line.at( 2 ).toInt(), line.at( 3 ).toInt() )));
                        table->setItem( n_rows, 2, new QTableWidgetItem( PrintSec::printableTime( line.at( 4 ).toInt(), line.at( 5 ).toInt(), line.at( 6 ).toInt() )));
                        for ( int i=7; i<line.size(); i++ ) {
                            table->setItem( n_rows, i-4, new QTableWidgetItem( line.at( i ) ));
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
            date = PrintSec::printableDate( year, this->getMonthNumber( month ), day ) + ", h " + hour;
        }
        table->verticalHeader()->setVisible( false );

        // apply the colors and append to the series
        QColor cols[]{ QColor(127,127,127), QColor(237,80,61) };
        //QColor cols[] = {QColor(18,175,194), QColor(237,80,61)};
        std::vector<QStackedBarSeries*> b_series;
        const size_t max{ sets.size() };
        for ( size_t i{0}; i<max; i++ ) {
            const auto& set{ sets.at( i ) };
            b_series.push_back( new QStackedBarSeries() );
            for ( size_t w{0}; w<2ul; w++ ) {
                QBarSet* b = set.at( w );
                b->setColor( cols[ w ] );
                b_series.at( i )->append( b );
            }
        }
        for ( auto *s : b_series ) {
            s->setBarWidth( 1 );
        }

        // set-up the bars chart
        QChart* b_chart{ new QChart() };
        // apply the theme
        b_chart->setTheme( theme );
        // append the bars
        foreach ( auto& bars, b_series ) {
            b_chart->addSeries( bars );
        }
        b_chart->setTitle( TR::tr( "Log Lines Marked as Warning" ) );
        b_chart->legend()->setVisible( false );
        /*b_chart->legend()->setAlignment( Qt::AlignBottom );
        b_chart->setAnimationOptions( QChart::SeriesAnimations );*/

        // craft the X-axis labels
        QStringList categories;
        if ( hour.isEmpty() ) {
            categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                       << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";
        } else {
            categories << "00" << "10" << "20" << "30" << "40" << "50";
        }

        // set-up the time-of-day axis (X)
        QBarCategoryAxis* axisX{ new QBarCategoryAxis() };
        axisX->append( categories );
        axisX->setTitleText( date );
        b_chart->addAxis( axisX, Qt::AlignBottom );
        for ( auto* s : b_series ) {
            s->attachAxis( axisX );
        }

        // set-up the count values axis (Y)
        QValueAxis* axisY{ new QValueAxis() };
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 8 ) ? max_count : 8 );
        axisY->setRange( 0, max_count );
        b_chart->addAxis( axisY, Qt::AlignLeft );
        for ( auto* s : b_series ) {
            s->attachAxis( axisY );
        }

        // apply the chart to the view
        chart->setChart( b_chart );
        chart->setRenderHint( QPainter::Antialiasing );
    }
}



void Crapview::drawSpeed( QTableWidget* table, QtCharts::QChartView* chart, const QChart::ChartTheme& theme, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& protocol, const QString& method, const QString& uri, const QString& query, const QString& response ) const
{
    Result<stats_speed_items_t> result;
    this->dbQuery.getSpeedData(
        result,
        web_server,
        year, month, day,
        protocol, method, uri, query, response );
    if ( result ) {
        // std::vector<std::tuple<long long, std::vector<QString>>>
        // [ ( epoch_msec, [ log_data ] ) ]
        auto& items{ result.getData() };

        // draw the speed chart
        QLineSeries* line{ new QLineSeries() };

        // build the line upon data
        size_t i{ 0 };
        const size_t max_i{ items.size() };
        int value{0}, aux_value, max_value{0}, n_rows{0};
        long long time /* xD */, aux_time, count{1};
        time = std::get<0>(items.at(0));
        QDateTime dt;
        std::vector<QString> data;
        for ( const auto& item : items ) {
            i++;
            // append a value to the chart
            aux_time = std::get<0>(item);
            data = std::get<1>(item);
            aux_value = data.at( 0 ).toInt();
            // append only if the second is different, else sum
            if ( aux_time > time ) {
                value = value/count;
                line->append( time, value );
                if ( value > max_value ) {
                    max_value = value;
                }
                time = aux_time;
                value = aux_value;
                count = 1;
                if ( i == max_i ) {
                    // final
                    line->append( time, value );
                    if ( value > max_value ) {
                        max_value = value;
                    }
                }
            } else {
                count ++;
                value += aux_value;
                if ( i == max_i ) {
                    // final
                    value = value/count;
                    line->append( aux_time, value );
                    if ( value > max_value ) {
                        max_value = value;
                    }
                }
            }
            // fill the teble with data
            if ( !data.at(0).isEmpty() || !data.at(1).isEmpty() || !data.at(2).isEmpty() || !data.at(3).isEmpty() || !data.at(4).isEmpty() || !data.at(5).isEmpty() ) {
                table->insertRow( n_rows );
                auto* tt{ new QTableWidgetItem() };
                tt->setData( Qt::DisplayRole, aux_value );
                table->setItem( n_rows, 0, tt );
                table->setItem( n_rows, 1, new QTableWidgetItem( data.at(1) ));
                table->setItem( n_rows, 2, new QTableWidgetItem( data.at(2) ));
                table->setItem( n_rows, 3, new QTableWidgetItem( data.at(3) ));
                table->setItem( n_rows, 4, new QTableWidgetItem( data.at(4) ));
                table->setItem( n_rows, 5, new QTableWidgetItem( data.at(5) ));
                dt = QDateTime::fromMSecsSinceEpoch( aux_time );
                table->setItem( n_rows, 6, new QTableWidgetItem( dt.time().toString("hh:mm:ss") ));
                n_rows ++;
            }
        }
        table->verticalHeader()->setVisible( false );

        // fictitious line
        QLineSeries* line_{ new QLineSeries() };

        // color the area
        QColor col1{ Qt::GlobalColor::red   },
               col2{ Qt::GlobalColor::green },
               col3{ Qt::GlobalColor::blue  };
        //area->setColor( col );
        QLinearGradient gradient{ QPointF(0, 0), QPointF(0, 1) };
        gradient.setColorAt(0.3, col1.lighter( 90 ) );
        gradient.setColorAt(0.7, col2.lighter( 90 ) );
        gradient.setColorAt(1.0, col3.lighter( 90 ) );
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

        QPen pen{ line->pen() };
        pen.setBrush( gradient );
        pen.setWidth( 1 );
        line->setPen(pen);

        pen = line_->pen();
        pen.setBrush( gradient );
        pen.setWidth( 1 );
        line_->setPen(pen);

        // build the chart
        QChart* l_chart{ new QChart() };
        l_chart->setTheme( theme );
        l_chart->addSeries( line );
        l_chart->addSeries( line_ );
        l_chart->setTitle( TR::tr( "Time Taken to Serve Requests" ) );
        /*l_chart->legend()->setAlignment( Qt::AlignBottom );*/
        l_chart->legend()->setVisible( false );

        // set-up the date-time axis (X)
        QDateTimeAxis* axisX{ new QDateTimeAxis() };
        axisX->setFormat( "hh:mm" );
        axisX->setTickCount( 25 );
        axisX->setTitleText( PrintSec::printableDate( year, this->getMonthNumber(month), day ) );
        l_chart->addAxis( axisX, Qt::AlignBottom );
        line->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis* axisY{ new QValueAxis() };
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_value < 8 ) ? max_value : 8 );
        axisY->setMinorTickCount( 4 );
        if ( max_value == 1 ) {
            max_value = 0;
        }
        axisY->setRange( 0, max_value );
        l_chart->addAxis( axisY, Qt::AlignLeft );
        line->attachAxis( axisY) ;

        // add the chart to the view
        chart->setChart( l_chart );
        chart->setRenderHint(QPainter::Antialiasing);
    }
}



void Crapview::drawCount( QTableWidget* table, QtCharts::QChartView* chart, const QChart::ChartTheme& theme, const QString& web_server, const QString& year, const QString& month, const QString& day, const QString& field ) const
{
    Result<stats_count_items_t> result;
    this->dbQuery.getItemsCount(
        result,
        web_server,
        year, month, day,
        field );
    if ( result ) {
        // std::vector<std::tuple<QString, int>>
        // [ ( log_item, count ) ]
        auto& items{ result.getData() };

        // make the pie
        QPieSeries* pie{ new QPieSeries() };
        // cut off exdceeding elements for the chart
        const int max_items{ 15 };
        int count, oth_count{0}, n_rows{0};
        QString item;
        // bring items in reverse order
        stats_count_items_t::const_reverse_iterator iter = items.rbegin();
        while ( iter != items.rend() ) {
            item = iter->second;
            count = iter->first;
            ++iter;
            if ( n_rows >= max_items ) {
                oth_count += count;
            } else {
                pie->append( item, count );
            }
            table->insertRow( n_rows );
            auto* ic{ new QTableWidgetItem() };
            ic->setData( Qt::DisplayRole, count );
            table->setItem( n_rows, 0, ic );
            table->setItem( n_rows, 1, new QTableWidgetItem( item ));
            n_rows ++;
        }
        table->verticalHeader()->setVisible( false );

        if ( oth_count > 0 ) {
            pie->append( TR::tr( "Others" ), oth_count );
            QPieSlice* slice = pie->slices().at( pie->count()-1 );
            slice->setBrush( Qt::gray );
        }
        pie->setPieSize( 0.65 );
        pie->setLabelsVisible( false );
        connect( pie, &QPieSeries::clicked, this, &Crapview::sliceClicked );

        QChart* p_chart{ new QChart() };
        p_chart->setTheme( theme );
        p_chart->addSeries( pie );
        p_chart->setTitle( field );
        p_chart->legend()->hide();

        chart->setChart( p_chart );
        chart->setRenderHint( QPainter::Antialiasing );
    }
}



void Crapview::drawDay( QtCharts::QChartView* chart, const QChart::ChartTheme& theme, const QString& web_server, const QString& from_year, const QString& from_month, const QString& from_day, const QString& to_year, const QString& to_month, const QString& to_day, const QString& field , const QString& filter ) const
{
    Result<stats_day_items_t> result;
    this->dbQuery.getDaytimeCounts(
        result,
        web_server,
        from_year, from_month, from_day,
        to_year, to_month, to_day,
        field, filter );
    if ( result ) {
        // std::unordered_map<int, std::unordered_map<int, int>>
        // { hour : { 10th_minutes : count } }
        auto& items{ result.getData() };

        // draw the chart
        QString date;
        QColor col{ Qt::GlobalColor::darkGreen };
        QBarSet* b_10{ new QBarSet( "" ) };
        b_10->setColor( col );
        QBarSet* b_20{ new QBarSet( "" ) };
        b_20->setColor( col );
        QBarSet* b_30{ new QBarSet( "" ) };
        if ( to_year.isEmpty() || to_month.isEmpty() || to_day.isEmpty() ) {
            date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
        } else {
            date = QString("%1 %2 %3 %4")
                .arg( TR::tr( "from" ),
                      PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
                      TR::tr( "to" ),
                      PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ) );
        }
        b_30->setColor( col );
        QBarSet* b_40{ new QBarSet( "" ) };
        b_40->setColor( col );
        QBarSet* b_50{ new QBarSet( "" ) };
        b_50->setColor( col );
        QBarSet* b_60{ new QBarSet( "" ) };
        b_60->setColor( col );

        // build the bars upon data
        int count, max_count{0};
        for ( int h{0}; h<24; h++ ) {
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
        QBarSeries* bars{ new QBarSeries() };
        bars->append( b_10 ); bars->append( b_20 ); bars->append( b_30 );
        bars->append( b_40 ); bars->append( b_50 ); bars->append( b_60 );
        bars->setBarWidth( 1 );

        // set-up the bars chart
        QChart* b_chart{ new QChart() };
        b_chart->setTheme( theme );
        b_chart->addSeries( bars );
        b_chart->setTitle( QString("%1: %2").arg( TR::tr( "Time of Day Count" ), field ) );
        b_chart->legend()->setVisible( false );
        /*b_chart->legend()->setAlignment( Qt::AlignBottom );
        b_chart->setAnimationOptions( QChart::SeriesAnimations );*/

        // craft the X-axis labels
        QStringList categories;
        categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                   << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

        // set-up the time-of-day axis (X)
        QBarCategoryAxis* axisX{ new QBarCategoryAxis() };
        axisX->append( categories );
        axisX->setTitleText( date );
        b_chart->addAxis( axisX, Qt::AlignBottom );
        bars->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis* axisY{ new QValueAxis() };
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 8 ) ? max_count : 8 );
        axisY->setRange( 0, max_count );
        b_chart->addAxis( axisY, Qt::AlignLeft );
        bars->attachAxis( axisY) ;

        // apply the chart to the view
        chart->setChart( b_chart );
        chart->setRenderHint( QPainter::Antialiasing );
    }
}



void Crapview::drawRelat( QtCharts::QChartView* chart, const QChart::ChartTheme& theme, const QString& web_server, const QString& from_year, const QString& from_month, const QString& from_day, const QString& to_year, const QString& to_month, const QString& to_day, const QString& field_1, const QString& filter_1, const QString& field_2, const QString& filter_2 ) const
{
    bool period{ true };
    Result<stats_relat_items_t> result;
    if ( from_year == to_year
      && from_month == to_month
      && from_day == to_day ) {
        period &= false;
        this->dbQuery.getRelationalCountsDay(
            result,
            web_server,
            from_year, from_month, from_day,
            field_1, filter_1,
            field_2, filter_2 );
    } else {
        this->dbQuery.getRelationalCountsPeriod(
            result,
            web_server,
            from_year, from_month, from_day,
            to_year, to_month, to_day,
            field_1, filter_1,
            field_2, filter_2 );
    }

    if ( result ) {
        // std::vector<std::tuple<long long, int>>
        // [ ( epoch_ms, count ) ]
        auto& items{ result.getData() };

        // draw the relational chart
        QLineSeries* line{ new QLineSeries() };
        // build the line upon data
        int count, max_count{0};
        long long time; // xD
        for ( const auto& item : items ) {
            time  = std::get<0>(item);
            count = std::get<1>(item);
            line->append( time, (double)count );
            if ( count > max_count ) {
                max_count = count;
            }
        }

        // build the area
        QString date;
        QAreaSeries* area{ new QAreaSeries( line ) };
        if ( ! period ) {
            date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
        } else {
            date = QString("%1 %2 %3 %4")
                .arg( TR::tr( "from" ),
                      PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
                      TR::tr( "to" ),
                      PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ));
        }

        // color the area
        QColor col1{ Qt::GlobalColor::red },
               col2{ Qt::GlobalColor::yellow },
               col3{ Qt::GlobalColor::magenta };
        //area->setColor( col );
        QLinearGradient gradient{ QPointF(0, 0), QPointF(0, 1) };
        gradient.setColorAt(0.25, col3.lighter( 70 ) );
        gradient.setColorAt(0.5,  col1.lighter( 80 ) );
        gradient.setColorAt(1.0,  col2.lighter( 90 ) );
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        area->setBrush( gradient );
        area->setBorderColor( col1.lighter( 50 ) );

        // fictitious area, just to have another label
        QAreaSeries* area_{ new QAreaSeries( ) };
        area_->setBrush( gradient );
        area_->setBorderColor( col1.lighter( 50 ) );

        // build the area
        QChart* a_chart{ new QChart() };
        a_chart->setTheme( theme );
        a_chart->addSeries( area );
        a_chart->addSeries( area_ );
        a_chart->setTitle( QString("%1: %2 -> %3").arg( TR::tr( "Time of Day Count" ), field_1, field_2) );
        a_chart->legend()->setVisible( false );
        /*a_chart->legend()->setFont( fonts.at( "main_small" ) );
        a_chart->legend()->setAlignment( Qt::AlignBottom );*/

        // set-up the date-time axis (X)
        QDateTimeAxis* axisX{ new QDateTimeAxis() };
        if ( period ) {
            //axisX->setLabelsVisible( false );
            axisX->setFormat( "yyyy-MM" );
            int ticks{ this->dbQuery.countMonths( from_year, from_month, to_year, to_month ) };
            if ( ticks == 1 ) {
                axisX->setFormat( "yyyy-MM-dd" );
                ticks = to_day.toInt() - from_day.toInt() +2;
            }
            axisX->setTickCount( ticks+1 );
        } else {
            axisX->setFormat( "hh:mm" );
            axisX->setTickCount( 25 );
        }
        axisX->setTitleText( date );
        a_chart->addAxis( axisX, Qt::AlignBottom );
        area->attachAxis( axisX );

        // set-up the count values axis (Y)
        QValueAxis* axisY{ new QValueAxis() };
        axisY->setLabelFormat( "%d" );
        axisY->setTickCount( ( max_count < 8 ) ? max_count : 8 );
        axisY->setRange( 0, max_count );
        a_chart->addAxis( axisY, Qt::AlignLeft );
        area->attachAxis( axisY) ;

        // add the chart to the view
        chart->setChart( a_chart );
        chart->setRenderHint(QPainter::Antialiasing);
    }
}


// calculate global informations
const bool Crapview::calcGlobals( std::vector<std::tuple<QString,QString>>& recur_list, std::vector<std::tuple<QString,QString>>& traffic_list, std::vector<std::tuple<QString,QString>>& perf_list, std::vector<QString>& work_list, const QString& web_server ) const
{
    bool result{ false };

    if ( ! this->dates.at( this->WebServer_s2i.value( web_server ) ).empty() ) {

        // { { item, count } } // 0:protocol, 1:method, 1:uri, 3:user-agent
        std::vector<std::unordered_map<QString, unsigned>> recurs{ {}, {}, {}, {} };
        // ( date_str, count )
        std::tuple<QString, int> traf_date;
        // { day_name : total_count }
        std::unordered_map<int, double> traf_day{ {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0} };
        // { hour : total_count }
        std::unordered_map<int, double> traf_hour{
            {0,0},  {1,0},  {2,0},  {3,0},  {4,0},  {5,0},  {6,0},  {7,0},  {8,0},  {9,0},  {10,0}, {11,0},
            {12,0}, {13,0}, {14,0}, {15,0}, {16,0}, {17,0}, {18,0}, {19,0}, {20,0}, {21,0}, {22,0}, {23,0} };
        // [ max, total, num ]
        std::vector<long long> perf_time;
        // [ max, total, num ]
        std::vector<long long> perf_sent;
        // [ max, total, num ]
        std::vector<long long> perf_receiv;
        // count
        long req_count{ 0 };

        result = this->dbQuery.getGlobalCounts(
            web_server, this->dates.at( this->WebServer_s2i.value( web_server ) ),
            recurs,
            traf_date, traf_day, traf_hour,
            perf_time, perf_sent, perf_receiv,
            req_count );

        if ( result ) {
            // compose the results

            // max request elements
            for ( size_t i{0}; i<4ul; i++ ) {
                unsigned max{ 0 };
                QString max_str{ "-" };
                const auto& aux{ recurs.at( i ) };
                for ( const auto& [s,c] : aux ) {
                    if ( c > max ) {
                        max = c;
                        max_str = s;
                    }
                }
                recur_list.push_back( std::make_tuple( max_str, QString("%1").arg(max) ) );
            }

            // max date ever
            traffic_list.push_back( std::make_tuple( std::get<0>(traf_date), QString("%1").arg( std::get<1>(traf_date) ) ) );

            // max day of the week
            double max{ 0.d };
            int max_{ 0 };
            for ( const auto& [d,c] : traf_day ) {
                if ( c > max ) {
                    max = c;
                    max_ = d;
                }
            }
            if ( max_ == 0.d ) {
                traffic_list.push_back( std::make_tuple( "-", "0" ) );
            } else {
                const int f{ static_cast<int>(max) };
                const int d{ (max<10.d) ? static_cast<int>(max*100.d)%100 : static_cast<int>(max*10.d)%10 };
                QString count{ QString("%1").arg( f ) };
                if ( d > 0 ) {
                    count += QString(".%1").arg( d );
                }
                traffic_list.push_back( std::make_tuple( TR::tr(this->dbQuery.DAYS.at(max_).c_str()), count ));
            }

            // max hour of the day
            max=0.d, max_=-1.d;
            for ( const auto& [h,c] : traf_hour ) {
                if ( c > max ) {
                    max = c;
                    max_ = h;
                }
            }
            if ( max_ < 0 ) {
                traffic_list.push_back( std::make_tuple( "", "0" ) );
            } else {
                const int f{ static_cast<int>(max) };
                const int d{ (max<10.d) ? static_cast<int>(max*100.d)%100 : static_cast<int>(max*10.d)%10 };
                QString count = QString("%1").arg( f );
                if ( d > 0 ) {
                    count += QString(".%1").arg( d );
                }
                QString h{ (max_<10) ? QString("0%1").arg(max_) : QString("%1").arg(max_) };
                traffic_list.push_back( std::make_tuple( h, count ) );
            }

            // mean/max time-taken
            perf_list.push_back( std::make_tuple(
                QString("%1 ms").arg( (perf_time.at(2)>0ll) ? perf_time.at(1)/perf_time.at(2) : perf_time.at(1) ),
                QString("%1 ms").arg( perf_time.at(0) ) ));
            perf_list.push_back( std::make_tuple(
                QString("%1 B").arg( (perf_sent.at(2)>0ll) ? perf_sent.at(1)/perf_sent.at(2) : perf_time.at(1) ),
                QString("%1 B").arg( perf_sent.at(0) ) ));
            perf_list.push_back( std::make_tuple(
                QString("%1 B").arg( (perf_receiv.at(2)>0ll) ? perf_receiv.at(1)/perf_receiv.at(2) : perf_receiv.at(1) ),
                QString("%1 B").arg( perf_receiv.at(0) ) ));

            // overall work list
            work_list.push_back( QString("%1").arg( req_count ) );

            float t = static_cast<float>( perf_time.at(1) );
            if ( t < 0.f ) {
                work_list.push_back( "0" );
            } else {
                int h{0}, m{0}, s{0}, ms{0};
                if ( t > 1000.f ) {
                    ms = static_cast<int>( t ) %1000;
                    t /= 1000.f;
                }
                if ( t > 60.f ) {
                    s = static_cast<int>( t );
                    s /= 60;
                    if ( s > 60 ) {
                        m = s/60;
                        s = s%60;
                        if ( m > 60 ) {
                            h = m/60;
                            m = m%60;
                        }
                    }
                }
                work_list.push_back( QString("%1 h %2 m %3.%4 s").arg(h).arg(m).arg(s).arg(ms) );
            }

            float b = static_cast<float>( perf_sent.at(1) );
            if ( b < 0.f ) {
                work_list.push_back( "0" );
            } else {
                int f{0}, d{0};
                QString sfx{ "B" };
                if ( b > 1024.f ) {
                    b /= 1024.f;
                    sfx = "KiB";
                    if ( b > 1024.f ) {
                        b /= 1024.f;
                        sfx = "MiB";
                        if ( b > 1024.f ) {
                            b /= 1024.f;
                            sfx = "GiB";
                        }
                    }
                }
                f = static_cast<int>( b );
                d = static_cast<int>( b*1000.f ) %1000;
                work_list.push_back( QString("%1.%2 %3").arg(f).arg(d).arg(sfx) );
            }
        }
    }

    return result;
}

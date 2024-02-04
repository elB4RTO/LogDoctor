
#include "crapview.h"

#include "globals/db_names.h"

#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/warnlists/modules/warnlist.h"

#include "modules/crapview/modules/lib.h"

#include <ranges>

#include <QGraphicsItem>
#include <QTableWidget>


DialogsLevel Crapview::getDialogsLevel() const noexcept
{
    return this->dialogs_level;
}
void Crapview::setDialogsLevel( const DialogsLevel new_level ) noexcept
{
    this->dialogs_level = new_level;
}


void Crapview::setDbPath( const std::string& path ) noexcept
{
    this->dbQuery.setDbPath( path + "/" + DatabasesNames::data );
}



int Crapview::getMonthNumber( const QString& month_str ) const noexcept
{
    const auto pos{ std::find_if( this->dbQuery.MONTHS.cbegin(), this->dbQuery.MONTHS.cend(),
        [&month_str](const auto& it){ return TR::tr(it.second.c_str()) == month_str; } ) };
    if ( pos != this->dbQuery.MONTHS.cend() ) {
        return pos->first;
    }
    return 0;
}


void Crapview::refreshDates()
{
    std::optional<database_dates_t> result;

    try {

        this->dbQuery.refreshDates( result );

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( result ) {
        // std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
        // { web_server_id : { year : { month : [ days ] } } }
        this->dates = std::move( *result );
    }
}
void Crapview::clearDates() noexcept
{
    this->dates.clear();
}

QStringList Crapview::getYears( const QString& web_server ) const noexcept
{
    QStringList years;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( const auto& y{ this->dates.at( ws ) }; !y.empty() ) {
            std::transform( y.cbegin(), y.cend(),
                            std::back_inserter( years ),
                            [](const auto& p){ return QString::number( p.first ); } );
        }
    }
    return years;
}
QStringList Crapview::getMonths( const QString& web_server, const QString& year ) const noexcept
{
    QStringList months;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( const auto& y{ this->dates.at( ws ) }; !y.empty() ) {
            const int year_{ year.toInt() };
            if ( const auto& m{ this->dates.at( ws ).at( year_ ) }; !m.empty() ) {
                const auto& m_tr{ this->dbQuery.MONTHS };
                std::transform( m.cbegin(), m.cend(),
                                std::back_inserter( months ),
                                [&m_tr](const auto& p){ return TR::tr(m_tr.at( p.first ).c_str()); } );
            }
        }
    }
    return months;
}
QStringList Crapview::getDays( const QString& web_server, const QString& year, const QString& month ) const noexcept
{
    QStringList days;
    if ( ! this->dates.empty() ) {
        const int ws{ this->WebServer_s2i.value( web_server ) };
        if ( const auto& y{ this->dates.at( ws ) }; !y.empty() ) {
            const int year_{ year.toInt() };
            if ( const auto& m{ this->dates.at( ws ).at( year_ ) }; !m.empty() ) {
                const int month_{ this->getMonthNumber( month ) };
                if ( const auto& d{ m.at( month_ ) }; !d.empty() ) {
                    std::transform( d.cbegin(), d.cend(),
                                    std::back_inserter( days ),
                                    [](const int day){ return QString::number( day ); } );
                }
            }
        }
    }
    return days;
}
QStringList Crapview::getHours() const noexcept
{
    static const QStringList hours{"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
    return hours;
}

QStringList Crapview::getWarnHeaderColumns() const noexcept
{
    return {
        TR::tr( FIELDS__WARNING.c_str()        ),
        TR::tr( WORDS__DATE.c_str()            ),
        TR::tr( WORDS__TIME.c_str()            ),
        TR::tr( FIELDS__PROTOCOL.c_str()       ),
        TR::tr( FIELDS__METHOD.c_str()         ),
        TR::tr( FIELDS__URI.c_str()            ),
        TR::tr( FIELDS__QUERY.c_str()          ),
        TR::tr( FIELDS__RESPONSE_CODE.c_str()  ),
        TR::tr( FIELDS__USER_AGENT.c_str()     ),
        TR::tr( FIELDS__CLIENT.c_str()         ),
        TR::tr( FIELDS__COOKIE.c_str()         ),
        TR::tr( FIELDS__REFERRER.c_str()       ),
        TR::tr( FIELDS__BYTES_RECEIVED.c_str() ),
        TR::tr( FIELDS__BYTES_SENT.c_str()     ),
        TR::tr( FIELDS__TIME_TAKEN.c_str()     )
    };
}

QStringList Crapview::getSpeedHeaderColumns() const noexcept
{
    return {
        TR::tr( FIELDS__TIME_TAKEN.c_str()     ),
        TR::tr( FIELDS__URI.c_str()            ),
        TR::tr( FIELDS__QUERY.c_str()          ),
        TR::tr( FIELDS__METHOD.c_str()         ),
        TR::tr( FIELDS__PROTOCOL.c_str()       ),
        TR::tr( FIELDS__RESPONSE_CODE.c_str()  ),
        TR::tr( WORDS__TIME.c_str()            )
    };
}


////////////////
//// CHARTS ////
////////////////
void Crapview::sliceClicked( QPieSlice* slice )
{
    slice->setExploded( !slice->isExploded() );
    slice->setLabelVisible( !slice->isLabelVisible() );
}


void Crapview::drawWarn( QTableWidget* table, QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString hour, const Warnlist& warnlist ) const
{
    std::optional<stats_warn_items_t> result;

    try {

        this->dbQuery.getWarningsData( result, web_server, year, month, day, hour );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // std::vector<std::vector<std::vector<std::array<QString,18>>>>
    // day  -> [ hours[ 10th_minutes[ lines[ log_data ] ] ] ]
    // hour -> [ 10th_minutes[ minute[ lines[ log_data ] ] ] ]
    const auto items{ std::move(*result) };

    const auto check_warnings{
        [&warnlist](const std::array<QString,18>& line)->std::tuple<bool,std::vector<int>>
        {
            bool is_warning{ false };
            std::vector<int> warning_cols;
            warning_cols.reserve(4ul);
            if ( warnlist.method.used ) {
                if ( VecOps::contains( warnlist.method.list, line.at( 7ul ).toStdString() ) ) {
                    is_warning |= true;
                    warning_cols.emplace_back( 4 );
                }
            }
            if ( warnlist.uri.used ) {
                const auto target{ line.at( 8ul ).toStdString() };
                const auto& list{ warnlist.uri.list };
                if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
                    // match found! skip this line
                    is_warning |= true;
                    warning_cols.emplace_back( 5 );
                }
            }
            if ( warnlist.client.used ) {
                const auto target{ line.at( 12ul ).toStdString() };
                const auto& list{ warnlist.client.list };
                if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
                    // match found! skip this line
                    is_warning |= true;
                    warning_cols.emplace_back( 9 );
                }
            }
            if ( warnlist.user_agent.used ) {
                const auto target{ line.at( 11ul ).toStdString() };
                const auto& list{ warnlist.user_agent.list };
                if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
                    // match found! skip this line
                    is_warning |= true;
                    warning_cols.emplace_back( 8 );
                }
            }
            return std::make_tuple( is_warning, warning_cols );
        }
    };

    const QColor& warn_color{ this->warning_color };

    const auto append_to_table{
        [&table,&warn_color](const std::array<QString,18>& line, const bool is_warning, const std::vector<int>& warning_cols )
        {
            const int n_rows{ table->rowCount() };
            table->insertRow( n_rows );
            table->setItem( n_rows, 0, new QTableWidgetItem( PrintSec::printableBool( is_warning )));
            table->setItem( n_rows, 1, new QTableWidgetItem( PrintSec::printableDate( line.at( 0 ).toInt(), line.at( 1 ).toInt(), line.at( 2 ).toInt() )));
            table->setItem( n_rows, 2, new QTableWidgetItem( PrintSec::printableTime( line.at( 3 ).toInt(), line.at( 4 ).toInt(), line.at( 5 ).toInt() )));
            int col{ 3 };
            for ( size_t i{6ul}; i<18ul; ++i ) {
                QTableWidgetItem* itm;
                if ( (col == 7 || col >= 12) && !line.at(i).isEmpty() ) {
                    itm = new QTableWidgetItem();
                    itm->setData( Qt::DisplayRole, line.at( i ).toInt() );
                } else {
                    itm = new QTableWidgetItem( line.at( i ) );
                }
                table->setItem( n_rows, col, itm );
                ++ col;
            }
            if ( is_warning ) {
                table->item( n_rows, 0 )->setForeground( warn_color );
                for ( const int col : warning_cols ) {
                    table->item( n_rows, col )->setForeground( warn_color );
                }
            }
        }
    };

    // bars
    std::vector<std::vector<QBarSet*>> sets;

    // build the bars and the table upon data
    QString date;
    int norm_count, warn_count, sum_count, max_count=0;
    if ( hour.isEmpty() ) {
        // entire day
        for ( int i{0}; i<6; ++i ) {
            sets.push_back( std::vector<QBarSet*>() );
            sets.back().push_back( new QBarSet("") );
            sets.back().push_back( new QBarSet("") );
        }
        for ( size_t h{0}; h<24ul; ++h ) {
            for ( size_t m{0}; m<6ul; ++m ) {
                const auto& data{ items.at( h ).at( m ) };
                norm_count = warn_count = 0;
                for ( const std::array<QString,18>& line : data ) {
                    const auto [is_warning, warning_cols]{ check_warnings( line ) };
                    append_to_table( line, is_warning, warning_cols );
                    is_warning ? ++warn_count : ++norm_count;
                }
                sets.at( m ).at( 0ul )->append( norm_count );
                sets.at( m ).at( 1ul )->append( warn_count );
                sum_count = norm_count + warn_count;
                if ( sum_count > max_count ) {
                    max_count = sum_count;
                }
            }
        }
        date = PrintSec::printableDate( year, this->getMonthNumber( month ), day );
    } else {
        // 1 hour
        for ( int i{0}; i<10; ++i ) {
            sets.push_back( std::vector<QBarSet*>() );
            sets.back().push_back( new QBarSet("") );
            sets.back().push_back( new QBarSet("") );
        }
        for ( size_t g{0ul}; g<6ul; ++g ) {
            for ( size_t m{0ul}; m<10ul; ++m ) {
                const auto& data{ items.at( g ).at( m ) };
                norm_count = warn_count = 0;
                for ( const std::array<QString,18>& line : data ) {
                    const auto [is_warning, warning_cols]{ check_warnings( line ) };
                    append_to_table( line, is_warning, warning_cols );
                    is_warning ? ++warn_count : ++norm_count;
                }
                sets.at( m ).at( 0ul )->append( norm_count );
                sets.at( m ).at( 1ul )->append( warn_count );
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
    std::vector<QStackedBarSeries*> b_series;
    const size_t max{ sets.size() };
    for ( size_t i{0}; i<max; ++i ) {
        const auto& set{ sets.at( i ) };
        b_series.push_back( new QStackedBarSeries() );
        for ( size_t w{0}; w<2ul; ++w ) {
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
        categories = this->getHours();
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
    axisY->setTickCount( ( max_count < 9 ) ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    b_chart->addAxis( axisY, Qt::AlignLeft );
    for ( auto* s : b_series ) {
        s->attachAxis( axisY );
    }

    // apply the chart to the view
    chart->setChart( b_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawSpeed( QTableWidget* table, QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString protocol, const QString method, const QString uri, const QString query, const QString response ) const
{
    std::optional<stats_speed_items_t> result;

    try {

        this->dbQuery.getSpeedData( result, web_server, year, month, day, protocol, method, uri, query, response );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // std::vector<std::tuple<long long, std::vector<QString>>>
    // [ ( epoch_msec, [ log_data ] ) ]
    const auto items{ std::move(*result) };

    // draw the speed chart
    QLineSeries* line{ new QLineSeries() };

    // build the line upon data
    size_t i{ 0 };
    const size_t max_i{ items.size() };
    int value{0}, count{1}, aux_value, max_value{0}, n_rows{0};
    bool first_count{ true };
    qint64 time{ std::get<0>(items.at(0ul)) };
    QDateTime dt;
    for ( const auto& item : items ) {
        ++i;
        // append a value to the chart
        const qint64 aux_time{ std::get<0>(item) };
        const std::array<QString,6>& data{ std::get<1>(item) };
        aux_value = data.at( 0ul ).toInt();
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
            first_count |= true;
            if ( i == max_i ) {
                // final
                line->append( time, value );
                if ( value > max_value ) {
                    max_value = value;
                }
            }
        } else {
            if ( first_count ) {
                first_count &= false;
            } else {
                ++ count;
            }
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
        if ( !data.at(0ul).isEmpty() || !data.at(1ul).isEmpty() || !data.at(2ul).isEmpty() || !data.at(3ul).isEmpty() || !data.at(4ul).isEmpty() || !data.at(5ul).isEmpty() ) {
            table->insertRow( n_rows );
            auto* tt{ new QTableWidgetItem() };
            tt->setData( Qt::DisplayRole, aux_value );
            table->setItem( n_rows, 0, tt );
            table->setItem( n_rows, 1, new QTableWidgetItem( data.at(1ul) ));
            table->setItem( n_rows, 2, new QTableWidgetItem( data.at(2ul) ));
            table->setItem( n_rows, 3, new QTableWidgetItem( data.at(3ul) ));
            table->setItem( n_rows, 4, new QTableWidgetItem( data.at(4ul) ));
            table->setItem( n_rows, 5, new QTableWidgetItem( data.at(5ul) ));
            dt = QDateTime::fromMSecsSinceEpoch( aux_time );
            table->setItem( n_rows, 6, new QTableWidgetItem( dt.time().toString("hh:mm:ss") ));
            ++ n_rows;
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
    gradient.setCoordinateMode(QGradient::ObjectMode);

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
    axisY->setTickCount( ( max_value < 9 ) ? max_value+1 : 9 );
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



void Crapview::drawCount( QTableWidget* table, QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString field ) const
{
    std::optional<stats_count_items_t> result;

    try {

        this->dbQuery.getItemsCount( result, web_server, year, month, day, field );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // std::vector<std::tuple<QString, int>>
    // [ ( log_item, count ) ]
    const auto items{ std::move(*result) };

    // make the pie
    QPieSeries* pie{ new QPieSeries() };
    // cut off exdceeding elements for the chart
    const int max_items{ 15 };
    int count, oth_count{0}, n_rows{0};
    QString item;
    // bring items in reverse order
    stats_count_items_t::const_reverse_iterator iter = items.crbegin();
    while ( iter != items.crend() ) {
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
        ++ n_rows;
    }
    table->verticalHeader()->setVisible( false );

    if ( oth_count > 0 ) {
        pie->append( TR::tr( "Others" ), oth_count );
        QPieSlice* slice = pie->slices().at( pie->count()-1 );
        slice->setBrush( Qt::gray );
    }
    pie->setPieSize( 0.60 );
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



void Crapview::drawDay( QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString from_year, const QString from_month, const QString from_day, const QString to_year, const QString to_month, const QString to_day, const QString field_str, const LogField field, const QString filter ) const
{
    std::optional<stats_day_items_t> result;

    try {

        this->dbQuery.getDaytimeCounts( result, web_server, from_year, from_month, from_day, to_year, to_month, to_day, field, filter );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // std::unordered_map<int, std::unordered_map<int, int>>
    // { hour : { 10th_minutes : count } }
    const auto items{ std::move(*result) };

    // draw the chart
    QString date;
    QColor col{ Qt::GlobalColor::darkGreen };
    QBarSet* b_10{ new QBarSet("") };
    b_10->setColor( col );
    QBarSet* b_20{ new QBarSet("") };
    b_20->setColor( col );
    QBarSet* b_30{ new QBarSet("") };
    if ( to_year.isEmpty() || to_month.isEmpty() || to_day.isEmpty() ) {
        date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
    } else {
        date = QStringLiteral("%1 %2 %3 %4")
            .arg( TR::tr( "from" ),
                  PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
                  TR::tr( "to" ),
                  PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ) );
    }
    b_30->setColor( col );
    QBarSet* b_40{ new QBarSet("") };
    b_40->setColor( col );
    QBarSet* b_50{ new QBarSet("") };
    b_50->setColor( col );
    QBarSet* b_60{ new QBarSet("") };
    b_60->setColor( col );

    // build the bars upon data
    int count, max_count{0};
    for ( size_t h{0ul}; h<24ul; ++h ) {
        auto& data = items.at( h );
        count = data.at( 0ul );
        *b_10 << count;
        if ( count > max_count ) {
            max_count = count;
        }
        count = data.at( 10ul );
        *b_20 << count;
        if ( count > max_count ) {
            max_count = count;
        }
        count = data.at( 20ul );
        *b_30 << count;
        if ( count > max_count ) {
            max_count = count;
        }
        count = data.at( 30ul );
        *b_40 << count;
        if ( count > max_count ) {
            max_count = count;
        }
        count = data.at( 40ul );
        *b_50 << count;
        if ( count > max_count ) {
            max_count = count;
        }
        count = data.at( 50ul );
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
    b_chart->setTitle( QStringLiteral("%1: %2").arg( TR::tr( "Time of Day Count" ), field_str ) );
    b_chart->legend()->setVisible( false );
    /*b_chart->legend()->setAlignment( Qt::AlignBottom );
    b_chart->setAnimationOptions( QChart::SeriesAnimations );*/

    // craft the X-axis labels
    const QStringList categories{ this->getHours() };

    // set-up the time-of-day axis (X)
    QBarCategoryAxis* axisX{ new QBarCategoryAxis() };
    axisX->append( categories );
    axisX->setTitleText( date );
    b_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );

    // set-up the count values axis (Y)
    QValueAxis* axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setTickCount( ( max_count < 9 ) ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    b_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY) ;

    // apply the chart to the view
    chart->setChart( b_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawRelat( QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString from_year, const QString from_month, const QString from_day, const QString to_year, const QString to_month, const QString to_day, const QString field_1_str, const LogField field_1, const QString filter_1, const QString field_2_str, const LogField field_2, const QString filter_2 ) const
{
    const bool period{ from_day != to_day || from_month != to_month || from_year != to_year };

    std::optional<stats_relat_items_t> result;

    try {

        if ( ! period ) {
            this->dbQuery.getRelationalCountsDay( result, web_server, from_year, from_month, from_day, field_1, filter_1, field_2, filter_2 );
        } else {
            this->dbQuery.getRelationalCountsPeriod( result, web_server, from_year, from_month, from_day, to_year, to_month, to_day, field_1, filter_1, field_2, filter_2 );
        }
    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // std::vector<std::tuple<long long, int>>
    // [ ( epoch_ms, count ) ]
    const auto items{ std::move(*result) };

    // draw the relational chart
    QLineSeries* line{ new QLineSeries() };
    // build the line upon data
    int max_count{0};
    for ( const auto& item : items ) {
        const qint64 time{ std::get<0>(item) };
        const int count{ std::get<1>(item) };
        line->append( static_cast<qreal>(time), static_cast<qreal>(count) );
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
        date = QStringLiteral("%1 %2 %3 %4")
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
    a_chart->setTitle( QStringLiteral("%1: %2 -> %3").arg( TR::tr( "Time of Day Count" ), field_1_str, field_2_str) );
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
    axisY->setTickCount( ( max_count < 9 ) ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    a_chart->addAxis( axisY, Qt::AlignLeft );
    area->attachAxis( axisY) ;

    // add the chart to the view
    chart->setChart( a_chart );
    chart->setRenderHint(QPainter::Antialiasing);
}


// calculate global informations
bool Crapview::calcGlobals( std::vector<std::tuple<QString,QString>>& recur_list, std::vector<std::tuple<QString,QString>>& traffic_list, std::vector<std::tuple<QString,QString>>& perf_list, std::vector<QString>& work_list, const QString web_server ) const
{
    const auto& avl_dates{ this->dates.at( this->WebServer_s2i.value( web_server ) ) };
    if ( avl_dates.empty() ) {
        return false;
    }

    std::optional<GlobalsData> result;

    try {

        this->dbQuery.getGlobalCounts( result, web_server, avl_dates );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return false;

    } catch ( const LogDoctorException& ) {
        // dialog already shown
        return false;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return false;
    }

    // std::vector<std::tuple<long long, int>>
    // [ ( epoch_ms, count ) ]
    const GlobalsData data{ std::move(*result) };

    // compose the results
    const QString __dash{ QStringLiteral("-") };
    const QString __zero{ QStringLiteral("0") };

    // most recurrent request elements
    {
        const auto max_of{
            [&__dash,&recur_list](const auto& rec)
            {
                size_t max{ 0 };
                QStringView max_str{ __dash };
                std::for_each( rec.begin(), rec.end(),
                    [&max,&max_str](const auto sc)
                    { if (auto& [s,c]{sc}; c>max){ max=c; max_str=s; } });
                recur_list.push_back(
                    std::make_tuple( max_str.toString(), QString::number(max) ) );
            }
        };
        max_of( data.recurs.protocol.asKeyValueRange()   );
        max_of( data.recurs.method.asKeyValueRange()     );
        max_of( data.recurs.uri.asKeyValueRange()        );
        max_of( data.recurs.user_agent.asKeyValueRange() );
    }

    // most trafficked
    {
        traffic_list.push_back( std::move( data.traf.date ) );
        const auto max_of{
            [this,&__dash,&__zero,&traffic_list](const auto& traf, const bool op)
            {
                double max_c{ 0.0 };
                size_t max_i{ traf.size() };
                std::ranges::for_each( std::views::enumerate(traf),
                    [&max_c,&max_i](const auto ic)
                    { if (auto& [i,c]{ic}; c>max_c){ max_c=c; max_i=i; } });
                if ( max_i == traf.size() ) {
                    traffic_list.push_back( std::make_tuple( __dash, __zero ) );
                } else {
                    const size_t f{ static_cast<size_t>(max_c) };
                    const size_t d{ max_c<10.0 ? static_cast<size_t>(max_c*100.0)%100ul : static_cast<size_t>(max_c*10.0)%10ul };
                    QString count{ QString::number( f ) };
                    if ( d > 0 ) {
                        count += QString(".%1").arg( d );
                    }
                    QString value{ op ? TR::tr(this->dbQuery.DAYS.at(max_i+1).c_str())
                                      : QStringLiteral("%1").arg(max_i, 2, 10, QChar('0')) };
                    traffic_list.push_back( std::make_tuple( value, count ) );
                }
            }
        };
        max_of( data.traf.day, true );
        max_of( data.traf.hour, false );
    }

    // mean/max performances
    perf_list.push_back( std::make_tuple(
        QStringLiteral("%1 ms").arg( data.perf.time_taken.count > 0ul
            ? data.perf.time_taken.total / data.perf.time_taken.count
            : data.perf.time_taken.total ),
        QStringLiteral("%1 ms").arg( data.perf.time_taken.max ) ));
    perf_list.push_back( std::make_tuple(
        QStringLiteral("%1 B").arg( data.perf.bytes_sent.count > 0ul
            ? data.perf.bytes_sent.total / data.perf.bytes_sent.count
            : data.perf.bytes_sent.total ),
        QStringLiteral("%1 B").arg( data.perf.bytes_sent.max ) ));
    perf_list.push_back( std::make_tuple(
        QStringLiteral("%1 B").arg( data.perf.bytes_recv.count > 0ul
            ? data.perf.bytes_recv.total / data.perf.bytes_recv.count
            : data.perf.bytes_recv.total ),
        QStringLiteral("%1 B").arg( data.perf.bytes_recv.max ) ));

    // overall work list
    work_list.push_back( QString::number( data.req_count ) );

    float t{ static_cast<float>( data.perf.time_taken.total ) };
    if ( t < 0.f ) {
        work_list.push_back( __zero );
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
        work_list.push_back( QStringLiteral("%1 h %2 m %3.%4 s").arg(h).arg(m).arg(s).arg(ms) );
    }

    float b{ static_cast<float>( data.perf.bytes_sent.total ) };
    if ( b < 0.f ) {
        work_list.push_back( __zero );
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
        work_list.push_back( QStringLiteral("%1.%2 %3").arg(f).arg(d).arg(sfx) );
    }

    return true;
}

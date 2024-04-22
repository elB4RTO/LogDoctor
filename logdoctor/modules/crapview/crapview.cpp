
#include "crapview.h"

#include "globals/db_names.h"

#include "utilities/arrays.h"
#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/warnlists/modules/warnlist.h"

#include "modules/crapview/modules/lib.h"

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

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( result ) {
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
        const WebServer ws{ this->WebServer_s2e.value( web_server ) };
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
        const WebServer ws{ this->WebServer_s2e.value( web_server ) };
        if ( const auto& y{ this->dates.at( ws ) }; !y.empty() ) {
            const int year_{ year.toInt() };
            if ( const auto& m{ y.at( year_ ) }; !m.empty() ) {
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
        const WebServer ws{ this->WebServer_s2e.value( web_server ) };
        if ( const auto& y{ this->dates.at( ws ) }; !y.empty() ) {
            const int year_{ year.toInt() };
            if ( const auto& m{ y.at( year_ ) }; !m.empty() ) {
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

void Crapview::setSpeedTimeInterval( const qint64 interval ) noexcept
{
    this->speed_interval = interval;
}
qint64 Crapview::getSpeedTimeInterval() const noexcept
{
    return this->speed_interval;
}

void Crapview::setSpeedTimeFormat( const QString& format ) noexcept
{
    this->speed_time_format = format;
}
const QString& Crapview::getSpeedTimeFormat() const noexcept
{
    return this->speed_time_format;
}

void Crapview::setCountPieSize( const qreal size ) noexcept
{
    this->count_pie_size = size;
}
qreal Crapview::getCountPieSize() const noexcept
{
    return this->count_pie_size;
}

void Crapview::setCountMaxSlices( const int value ) noexcept
{
    this->count_max_slices = value;
}
int Crapview::getCountMaxSlices() const noexcept
{
    return this->count_max_slices;
}

void Crapview::setRelatTimeFormat( const QString& format ) noexcept
{
    this->relat_time_format = format;
}
const QString& Crapview::getRelatTimeFormat() const noexcept
{
    return this->relat_time_format;
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

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

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
                    is_warning |= true;
                    warning_cols.emplace_back( 5 );
                }
            }
            if ( warnlist.user_agent.used ) {
                const auto target{ line.at( 11ul ).toStdString() };
                const auto& list{ warnlist.user_agent.list };
                if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
                    is_warning |= true;
                    warning_cols.emplace_back( 8 );
                }
            }
            if ( warnlist.client.used ) {
                const auto target{ line.at( 12ul ).toStdString() };
                const auto& list{ warnlist.client.list };
                if ( std::any_of( list.cbegin(), list.cend(), [&target](std::string_view flag){ return StringOps::startsWith( target, flag ); } ) ) {
                    is_warning |= true;
                    warning_cols.emplace_back( 9 );
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
    const QColor cols[]{ QColor(127,127,127), QColor(237,80,61) };
    std::vector<QStackedBarSeries*> b_series;
    const size_t max{ sets.size() };
    for ( size_t i{0}; i<max; ++i ) {
        const auto& set{ sets.at( i ) };
        auto* series{ new QStackedBarSeries() };
        for ( size_t w{0}; w<2ul; ++w ) {
            QBarSet* b{ set.at( w ) };
            b->setColor( cols[ w ] );
            series->append( b );
        }
        series->setBarWidth( 1 );
        b_series.push_back( series );
    }

    // set-up the bars chart
    QChart* b_chart{ new QChart() };
    b_chart->setTheme( theme );
    for ( auto* bars : b_series ) {
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

    chart->setChart( b_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawSpeed( QTableWidget* table, QChartView* chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString protocol, const QString method, const QString uri, const QString query, const QString response ) const
{
    std::optional<stats_speed_items_t> result;

    try {

        this->dbQuery.getSpeedData( result, web_server, year, month, day, protocol, method, uri, query, response, this->speed_interval );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return;

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        chart->setChart( new QChart() );
        return;
    }

    // [ ( epoch_msec, [ log_data ] ) ]
    const auto items{ std::move(*result) };

    QLineSeries* line{ new QLineSeries() };

    // build the line upon data
    size_t i{ 0 };
    const size_t max_i{ items.size() };
    int value{0}, count{0}, aux_value, max_value{0}, n_rows{0};
    qint64 time{ -1 };
    const auto finalize_value{
        [&value,&count]()
        {
            if ( value == -1 ) {
                value = 0;
            } else {
                value /= count+1;
            }
        }
    };
    const auto push_to_line{
        [&max_value]( QLineSeries*const line, const qint64 time, const int value )
        {
            line->append( time, value );
            if ( value > max_value ) {
                max_value = value;
            }
        }
    };
    const auto set_values{
        [&time,&value,&count]( const qint64 t, const int v, const int c )
        {
            time = t; value = v; count = c;
        }
    };
    for ( const auto& item : items ) {
        ++i;
        const qint64 aux_time{ std::get<0>(item) };
        const std::array<QString,6>& data{ std::get<1>(item) };
        if ( const auto tt{data.at(0ul)}; tt.isEmpty() ) {
            if ( time != -1 ) {
                finalize_value();
                push_to_line( line, time, value );
            }
            set_values( aux_time, -1, 0 );
            push_to_line( line, time, 0 );
            continue;
        } else {
            aux_value = tt.toInt();
            if ( aux_time > time ) {
                finalize_value();
                push_to_line( line, time, value );
                set_values( aux_time, aux_value, 0 );
                if ( i == max_i ) {
                    push_to_line( line, time, value );
                }
            } else {
                if ( value == -1 ) {
                    value = 0;
                } else {
                    ++ count;
                }
                value += aux_value;
                if ( i == max_i ) {
                    value /= count+1;
                    push_to_line( line, aux_time, value );
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
            QDateTime dt{ QDateTime::fromMSecsSinceEpoch( aux_time ) };
            table->setItem( n_rows, 6, new QTableWidgetItem( dt.time().toString("hh:mm:ss") ));
            ++ n_rows;
        }
    }
    table->verticalHeader()->setVisible( false );

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

    // build the chart
    QChart* l_chart{ new QChart() };
    l_chart->setTheme( theme );
    l_chart->addSeries( line );
    l_chart->setTitle( TR::tr( "Time Taken to Serve Requests" ) );
    /*l_chart->legend()->setAlignment( Qt::AlignBottom );*/
    l_chart->legend()->setVisible( false );

    // set-up the date-time axis (X)
    QDateTimeAxis* axisX{ new QDateTimeAxis() };
    axisX->setFormat( this->speed_time_format );
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

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // [ ( log_item, count ) ]
    const auto items{ std::move(*result) };

    QPieSeries* pie{ new QPieSeries() };
    // cut off exdceeding elements for the chart
    int oth_count{0}, n_rows{0};
    // bring items in reverse order
    auto iter{ items.crbegin() };
    while ( iter != items.crend() ) {
        const QString& item{ iter->second };
        const unsigned count{ iter->first };
        if ( n_rows >= this->count_max_slices ) {
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
        ++ iter;
    }
    table->verticalHeader()->setVisible( false );

    if ( oth_count > 0 ) {
        pie->append( TR::tr( "Others" ), oth_count );
        QPieSlice* slice = pie->slices().at( pie->count()-1 );
        slice->setBrush( Qt::gray );
    }
    pie->setPieSize( this->count_pie_size );
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

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // { hour : { 10th_minutes : count } }
    const auto items{ std::move(*result) };

    QString date;
    if ( to_year.isEmpty() || to_month.isEmpty() || to_day.isEmpty() ) {
        date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
    } else {
        date = QStringLiteral("%1 %2 %3 %4").arg(
            TR::tr( "from" ),
            PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
            TR::tr( "to" ),
            PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ) );
    }

    QColor col{ Qt::GlobalColor::darkGreen };
    QBarSet* b_10{ new QBarSet("") };
    b_10->setColor( col );
    QBarSet* b_20{ new QBarSet("") };
    b_20->setColor( col );
    QBarSet* b_30{ new QBarSet("") };
    b_30->setColor( col );
    QBarSet* b_40{ new QBarSet("") };
    b_40->setColor( col );
    QBarSet* b_50{ new QBarSet("") };
    b_50->setColor( col );
    QBarSet* b_60{ new QBarSet("") };
    b_60->setColor( col );

    // build the bars upon data
    int count, max_count{0};
    const auto check_max_count{ [&count,&max_count](){
        if ( count > max_count ) max_count = count;
    }};
    for ( size_t h{0ul}; h<24ul; ++h ) {
        const auto& data{ items.at( h ) };
        count = data.at( 0ul );
        *b_10 << count;
        check_max_count();
        count = data.at( 10ul );
        *b_20 << count;
        check_max_count();
        count = data.at( 20ul );
        *b_30 << count;
        check_max_count();
        count = data.at( 30ul );
        *b_40 << count;
        check_max_count();
        count = data.at( 40ul );
        *b_50 << count;
        check_max_count();
        count = data.at( 50ul );
        *b_60 << count;
        check_max_count();
    }

    QBarSeries* bars{ new QBarSeries() };
    bars->append( b_10 ); bars->append( b_20 ); bars->append( b_30 );
    bars->append( b_40 ); bars->append( b_50 ); bars->append( b_60 );
    bars->setBarWidth( 1 );

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

    } catch ( const VoidException& ) {
        // dialog already shown
        return;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return;
    }

    // [ ( epoch_ms, count ) ]
    const auto items{ std::move(*result) };

    QString date;
    if ( ! period ) {
        date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
    } else {
        date = QStringLiteral("%1 %2 %3 %4").arg(
            TR::tr( "from" ),
            PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
            TR::tr( "to" ),
            PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ));
    }

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

    QAreaSeries* area{ new QAreaSeries( line ) };
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
    a_chart->setTitle( QStringLiteral("%1: %2 -> %3").arg( TR::tr( "Relational Count" ), field_1_str, field_2_str) );
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
        axisX->setFormat( this->relat_time_format );
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

    chart->setChart( a_chart );
    chart->setRenderHint(QPainter::Antialiasing);
}


// calculate global informations
bool Crapview::calcGlobals( std::vector<std::tuple<QString,QString>>& recur_list, std::vector<std::tuple<QString,QString>>& traffic_list, std::vector<std::tuple<QString,QString>>& perf_list, std::vector<QString>& work_list, const QString web_server ) const
{
    const auto& avl_dates{ this->dates.at( this->WebServer_s2e.value( web_server ) ) };
    if ( avl_dates.empty() ) {
        return false;
    }

    std::optional<GlobalsData> result;

    try {

        this->dbQuery.getGlobalCounts( result, web_server, avl_dates );

    } catch ( const DatabaseException& e ) {
        DialogSec::errProcessingStatsData( e.what() );
        return false;

    } catch ( const VoidException& ) {
        // dialog already shown
        return false;
    }

    if ( ! result ) {
        DialogSec::msgNoDataForStats();
        return false;
    }

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
                /*std::ranges::for_each( std::views::enumerate(traf),
                    [&max_c,&max_i](const auto ic)
                    { if (auto& [i,c]{ic}; c>max_c){ max_c=c; max_i=i; } });*/
                for( const auto [index,count] : ArrayOps::enumerate(traf) ) {
                    if ( count > max_c ) {
                        max_c = count;
                        max_i = index;
                    }
                };
                if ( max_i == traf.size() ) {
                    traffic_list.emplace_back( __dash, __zero );
                } else {
                    const size_t f{ static_cast<size_t>(max_c) };
                    const size_t d{ max_c<10.0 ? static_cast<size_t>(max_c*100.0)%100ul : static_cast<size_t>(max_c*10.0)%10ul };
                    QString count{ QString::number( f ) };
                    if ( d > 0 ) {
                        count += QString::number( d ).prepend(QLatin1Char('.'));
                    }
                    const QString value{ op ? TR::tr(this->dbQuery.DAYS.at(max_i+1).c_str())
                                            : QStringLiteral("%1").arg(max_i, 2, 10, QChar('0')) };
                    traffic_list.emplace_back( value, count );
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

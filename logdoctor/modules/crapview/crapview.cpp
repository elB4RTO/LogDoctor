
#include "crapview.h"

#include "globals/db_names.h"

#include "modules/crapview/utilities/charts.h"
#include "modules/crapview/utilities/globals.h"

#include "utilities/printables.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"

#include "modules/warnlists/modules/warnlist.h"

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
    std::optional<WarningData> result;

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
    WarningData data{ std::move(*result) };

    data.checkWarnings( warnlist );

    data.insertInTable( table, this->warning_color );
    table->verticalHeader()->setVisible( false );

    std::vector<std::vector<QBarSet*>> sets;
    const int max_count{ data.insertInChart( sets ) };

    std::vector<QStackedBarSeries*> b_series;
    ChartOps::Warnings::appendToSeries( b_series, sets );

    QChart*const b_chart{ new QChart() };
    ChartOps::Warnings::appendToChart( b_chart, b_series );

    ChartOps::Warnings::setupChart( b_chart, theme );

    const QStringList categories{
        hour.isEmpty() ? this->getHours() : QStringList{"00","10","20","30","40","50"}
    };
    const QString date{ hour.isEmpty()
        ? PrintSec::printableDate( year, this->getMonthNumber( month ), day )
        : PrintSec::printableDate( year, this->getMonthNumber( month ), day ) + ", h " + hour
    };

    ChartOps::Warnings::attachXAxis( b_chart, b_series, categories, date );

    ChartOps::Warnings::attachYAxis( b_chart, b_series, max_count );

    chart->setChart( b_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawSpeed( QTableWidget*const table, QChartView*const chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString protocol, const QString method, const QString uri, const QString query, const QString response ) const
{
    std::optional<SpeedData> result;

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
    const SpeedData items{ std::move(*result) };

    items.insertInTable( table );
    table->verticalHeader()->setVisible( false );

    QLineSeries*const line{ new QLineSeries() };
    const int max_value{ items.insertInChart( line ) };

    ChartOps::Speed::applyGradient( line );

    QChart*const l_chart{ new QChart() };
    ChartOps::Speed::appendToChart( l_chart, line );

    ChartOps::Speed::setupChart( l_chart, theme );

    const QString date{ PrintSec::printableDate( year, this->getMonthNumber(month), day ) };

    ChartOps::Speed::attachXAxis( l_chart, line, this->speed_time_format, date );

    ChartOps::Speed::attachYAxis( l_chart, line, max_value );

    chart->setChart( l_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawCount( QTableWidget*const table, QChartView*const chart, const QChart::ChartTheme& theme, const QString web_server, const QString year, const QString month, const QString day, const QString field ) const
{
    std::optional<CountData> result;

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
    const CountData data{ std::move(*result) };

    data.insertInTable( table );
    table->verticalHeader()->setVisible( false );

    QPieSeries*const pie{ new QPieSeries() };
    data.insertInChart( pie, this->count_max_slices, TR::tr( "Others" ) );

    ChartOps::Count::setupPie( pie, this->count_pie_size );
    connect( pie, &QPieSeries::clicked, this, &Crapview::sliceClicked );

    QChart*const p_chart{ new QChart() };
    ChartOps::Count::setupChart( p_chart, theme, field );

    chart->setChart( p_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawDay( QChartView*const chart, const QChart::ChartTheme& theme, const QString web_server, const QString from_year, const QString from_month, const QString from_day, const QString to_year, const QString to_month, const QString to_day, const QString field_str, const LogField field, const QString filter ) const
{
    std::optional<DaytimeData> result;

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
    const DaytimeData data{ std::move(*result) };

    const QColor col{ Qt::GlobalColor::darkGreen };

    QBarSet*const b_10{ new QBarSet("") };
    b_10->setColor( col );
    QBarSet*const b_20{ new QBarSet("") };
    b_20->setColor( col );
    QBarSet*const b_30{ new QBarSet("") };
    b_30->setColor( col );
    QBarSet*const b_40{ new QBarSet("") };
    b_40->setColor( col );
    QBarSet*const b_50{ new QBarSet("") };
    b_50->setColor( col );
    QBarSet*const b_60{ new QBarSet("") };
    b_60->setColor( col );
    QList<QBarSet*> sets{ b_10, b_20, b_30, b_40, b_50, b_60 };

    const qreal max_count{ data.insertInChart( sets ) };

    QBarSeries*const bars{ new QBarSeries() };
    ChartOps::Daytime::setupSeries( bars, sets );

    QChart*const b_chart{ new QChart() };
    ChartOps::Daytime::setupChart( b_chart, theme, field_str );

    const QStringList categories{ this->getHours() };
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

    ChartOps::Daytime::attachXAxis( b_chart, bars, categories, date );

    ChartOps::Daytime::attachYAxis( b_chart, bars, max_count );

    chart->setChart( b_chart );
    chart->setRenderHint( QPainter::Antialiasing );
}



void Crapview::drawRelat( QChartView*const chart, const QChart::ChartTheme& theme, const QString web_server, const QString from_year, const QString from_month, const QString from_day, const QString to_year, const QString to_month, const QString to_day, const QString field_1_str, const LogField field_1, const QString filter_1, const QString field_2_str, const LogField field_2, const QString filter_2 ) const
{
    const bool period{ from_day != to_day || from_month != to_month || from_year != to_year };

    std::optional<RelationalData> result;

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
    const RelationalData data{ std::move(*result) };

    QLineSeries*const line{ new QLineSeries() };
    const int max_count{ data.insertInChart( line ) };

    QAreaSeries*const area{ new QAreaSeries( line ) };
    ChartOps::Relational::applyGradient( area );

    // fictitious area, just to have another label
    QAreaSeries*const area_{ new QAreaSeries( ) };
    ChartOps::Relational::applyGradient( area_ );

    // build the area
    QChart*const a_chart{ new QChart() };
    ChartOps::Relational::appendToChart( a_chart, area  );
    ChartOps::Relational::appendToChart( a_chart, area_ );

    ChartOps::Relational::setupChart( a_chart, theme, field_1_str, field_2_str );

    int n_ticks;
    QString time_format, date;
    if ( period ) {
        time_format = "yyyy-MM";
        n_ticks = this->dbQuery.countMonths( from_year, from_month, to_year, to_month );
        if ( n_ticks == 1 ) {
            time_format = "yyyy-MM-dd";
            n_ticks = to_day.toInt() - from_day.toInt() + 2;
        }
        ++n_ticks;
        date = QStringLiteral("%1 %2 %3 %4").arg(
            TR::tr( "from" ),
            PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day ),
            TR::tr( "to" ),
            PrintSec::printableDate( to_year, this->getMonthNumber(to_month), to_day ));
    } else {
        time_format = this->relat_time_format;
        n_ticks = 25;
        date = PrintSec::printableDate( from_year, this->getMonthNumber(from_month), from_day );
    }

    ChartOps::Relational::attachXAxis( a_chart, area, n_ticks, time_format, date );

    ChartOps::Relational::attachYAxis( a_chart, area, max_count );

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

    const QString __dash{ QStringLiteral("-") };
    const QString __zero{ QStringLiteral("0") };

    GlobalsOps::appendMostRecurrent( recur_list, data.recurs.protocol.asKeyValueRange(), __dash );
    GlobalsOps::appendMostRecurrent( recur_list, data.recurs.method.asKeyValueRange(), __dash );
    GlobalsOps::appendMostRecurrent( recur_list, data.recurs.uri.asKeyValueRange(), __dash );
    GlobalsOps::appendMostRecurrent( recur_list, data.recurs.user_agent.asKeyValueRange(), __dash );

    traffic_list.push_back( std::move( data.traf.date ) );
    const auto [max_i_d,max_c_d] = GlobalsOps::getMaxIndexCount( data.traf.day );
    GlobalsOps::appendMostTrafficked( traffic_list, data.traf.day, max_i_d, max_c_d, TR::tr(this->dbQuery.DAYS.at(max_i_d+1).c_str()), __dash, __zero );
    const auto [max_i_h,max_c_h] = GlobalsOps::getMaxIndexCount( data.traf.hour );
    GlobalsOps::appendMostTrafficked( traffic_list, data.traf.hour, max_i_h, max_c_h, QStringLiteral("%1").arg(max_i_h, 2, 10, QChar('0')), __dash, __zero );

    GlobalsOps::appendMeanMaxPerformances( perf_list, data.perf.time_taken, QLatin1String("ms") );
    GlobalsOps::appendMeanMaxPerformances( perf_list, data.perf.bytes_sent, QLatin1String("B") );
    GlobalsOps::appendMeanMaxPerformances( perf_list, data.perf.bytes_recv, QLatin1String("B") );

    work_list.push_back( QString::number( data.req_count ) );
    GlobalsOps::appendTotalWorkTime( work_list, data.perf.time_taken, __zero );
    GlobalsOps::appendTotalDataTransfer( work_list, data.perf.bytes_sent, __zero );

    return true;
}

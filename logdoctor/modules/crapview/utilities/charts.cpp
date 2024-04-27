
#include "charts.h"

#include "modules/shared.h"

#include <QValueAxis>
#include <QDateTimeAxis>
#include <QBarCategoryAxis>

#include <QStackedBarSeries>
#include <QLineSeries>
#include <QAreaSeries>
#include <QPieSeries>
#include <QBarSeries>

#include <QBarSet>


namespace ChartOps
{

namespace Warnings
{

void appendToSeries( std::vector<QStackedBarSeries*>& b_series, std::vector<std::vector<QBarSet*>>& sets )
{
    b_series.reserve( sets.size() );
    const QColor cols[]{ QColor(127,127,127), QColor(237,80,61) };
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
}

void appendToChart( QChart*const b_chart, const std::vector<QStackedBarSeries*>& b_series )
{
    for ( auto* bars : b_series ) {
        b_chart->addSeries( bars );
    }
}

void setupChart( QChart*const b_chart, const QChart::ChartTheme& theme )
{
    b_chart->setTheme( theme );
    b_chart->setTitle( TR::tr( "Log Lines Marked as Warning" ) );
    b_chart->legend()->setVisible( false );
    /*b_chart->legend()->setAlignment( Qt::AlignBottom );
    b_chart->setAnimationOptions( QChart::SeriesAnimations );*/
}

void attachXAxis( QChart*const b_chart, std::vector<QStackedBarSeries*>& b_series, const QStringList categories, const QString date )
{
    QBarCategoryAxis*const axisX{ new QBarCategoryAxis() };
    axisX->append( categories );
    axisX->setTitleText( date );
    b_chart->addAxis( axisX, Qt::AlignBottom );
    for ( auto* s : b_series ) {
        s->attachAxis( axisX );
    }
}

void attachYAxis( QChart*const b_chart, std::vector<QStackedBarSeries*>& b_series, const int max_count )
{
    QValueAxis*const axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setTickCount( max_count < 9 ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    b_chart->addAxis( axisY, Qt::AlignLeft );
    for ( auto* s : b_series ) {
        s->attachAxis( axisY );
    }
}

} // namespace Warnings


namespace Speed
{

void applyGradient( QLineSeries*const line )
{
    const QColor col1{ Qt::GlobalColor::red   };
    const QColor col2{ Qt::GlobalColor::green };
    const QColor col3{ Qt::GlobalColor::blue  };
    //area->setColor( col );
    QLinearGradient gradient{ QPointF(0, 0), QPointF(0, 1) };
    gradient.setColorAt(0.3, col1.lighter( 90 ) );
    gradient.setColorAt(0.7, col2.lighter( 90 ) );
    gradient.setColorAt(1.0, col3.lighter( 90 ) );
    gradient.setCoordinateMode( QGradient::ObjectMode );

    QPen pen{ line->pen() };
    pen.setBrush( gradient );
    pen.setWidth( 1 );
    line->setPen(pen);
}

void appendToChart( QChart*const l_chart, QLineSeries*const line )
{
    l_chart->addSeries( line );
}

void setupChart( QChart*const l_chart, const QChart::ChartTheme& theme )
{
    l_chart->setTheme( theme );
    l_chart->setTitle( TR::tr( "Time Taken to Serve Requests" ) );
    /*l_chart->legend()->setAlignment( Qt::AlignBottom );*/
    l_chart->legend()->setVisible( false );
}

void attachXAxis( QChart*const l_chart, QLineSeries*const line, const QString& time_format, const QString date )
{
    QDateTimeAxis*const axisX{ new QDateTimeAxis() };
    axisX->setFormat( time_format );
    axisX->setTickCount( 25 );
    axisX->setTitleText( date );
    l_chart->addAxis( axisX, Qt::AlignBottom );
    line->attachAxis( axisX );
}

void attachYAxis( QChart*const l_chart, QLineSeries*const line, const int max_value )
{
    QValueAxis*const axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setTickCount( max_value < 9 ? max_value+1 : 9 );
    axisY->setMinorTickCount( 4 );
    axisY->setRange( 0, max_value > 1 ? max_value : 0 );
    l_chart->addAxis( axisY, Qt::AlignLeft );
    line->attachAxis( axisY) ;
}

} // namespace Speed


namespace Count
{

void setupPie( QPieSeries*const pie, const qreal pie_size )
{
    pie->setPieSize( pie_size );
    pie->setLabelsVisible( false );
}

void appendToChart( QChart*const p_chart, QPieSeries*const pie )
{
    p_chart->addSeries( pie );
}

void setupChart( QChart*const p_chart, const QChart::ChartTheme& theme, const QString& title )
{
    p_chart->setTheme( theme );
    p_chart->setTitle( title );
    p_chart->legend()->hide();
}

} // namespace Count


namespace Daytime
{

void setupSeries( QBarSeries*const bars, const QList<QBarSet*>& sets )
{
    bars->append( sets );
    bars->setBarWidth( 1 );
}

void appendToChart( QChart*const b_chart, QBarSeries*const bars )
{
    b_chart->addSeries( bars );
}

void setupChart( QChart*const b_chart, const QChart::ChartTheme& theme, const QString& field_str )
{
    b_chart->setTheme( theme );
    b_chart->setTitle( QStringLiteral("%1: %2").arg( TR::tr( "Time of Day Count" ), field_str ) );
    b_chart->legend()->setVisible( false );
    /*b_chart->legend()->setAlignment( Qt::AlignBottom );
    b_chart->setAnimationOptions( QChart::SeriesAnimations );*/
}

void attachXAxis( QChart*const b_chart, QBarSeries*const bars, const QStringList& categories, const QString date )
{
    QBarCategoryAxis*const axisX{ new QBarCategoryAxis() };
    axisX->append( categories );
    axisX->setTitleText( date );
    b_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );
}

void attachYAxis( QChart*const b_chart, QBarSeries*const bars, const int max_count )
{
    QValueAxis*const axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setTickCount( ( max_count < 9 ) ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    b_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY );
}

} // namespace Daytime


namespace Relational
{

void applyGradient( QAreaSeries*const area )
{
    const QColor col1{ Qt::GlobalColor::red     };
    const QColor col2{ Qt::GlobalColor::yellow  };
    const QColor col3{ Qt::GlobalColor::magenta };
    //area->setColor( col );
    QLinearGradient gradient{ QPointF(0, 0), QPointF(0, 1) };
    gradient.setColorAt(0.25, col3.lighter( 70 ) );
    gradient.setColorAt(0.5,  col1.lighter( 80 ) );
    gradient.setColorAt(1.0,  col2.lighter( 90 ) );
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    area->setBrush( gradient );
    area->setBorderColor( col1.lighter( 50 ) );
}

void appendToChart( QChart*const a_chart, QAreaSeries*const area )
{
    a_chart->addSeries( area );
}

void setupChart( QChart*const a_chart, const QChart::ChartTheme& theme, const QString& field_1_str, const QString& field_2_str )
{
    a_chart->setTheme( theme );
    a_chart->setTitle( QStringLiteral("%1: %2 -> %3").arg( TR::tr( "Relational Count" ), field_1_str, field_2_str) );
    a_chart->legend()->setVisible( false );
    /*a_chart->legend()->setFont( fonts.at( "main_small" ) );
    a_chart->legend()->setAlignment( Qt::AlignBottom );*/
}

void attachXAxis( QChart*const a_chart, QAreaSeries*const area, const int n_ticks, const QString& time_format, const QString& date )
{
    QDateTimeAxis*const axisX{ new QDateTimeAxis() };
    axisX->setFormat( time_format );
    axisX->setTickCount( n_ticks );
    axisX->setTitleText( date );
    a_chart->addAxis( axisX, Qt::AlignBottom );
    area->attachAxis( axisX );
}

void attachYAxis( QChart*const a_chart, QAreaSeries*const area, const int max_count )
{
    QValueAxis*const axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setTickCount( ( max_count < 9 ) ? max_count+1 : 9 );
    axisY->setRange( 0, max_count );
    a_chart->addAxis( axisY, Qt::AlignLeft );
    area->attachAxis( axisY) ;
}

} // namespace Relational


} // namespace ChartOps

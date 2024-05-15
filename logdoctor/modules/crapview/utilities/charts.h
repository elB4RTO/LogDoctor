#ifndef LOGDOCTOR__CRAPVIEW__UTILITIES__CHARTS_H
#define LOGDOCTOR__CRAPVIEW__UTILITIES__CHARTS_H


#include <QChart>

#include <vector>


class QBarSet;

class QBarSeries;
class QPieSeries;
class QLineSeries;
class QAreaSeries;
class QStackedBarSeries;


namespace ChartOps
{


namespace Warnings
{

void appendToSeries( std::vector<QStackedBarSeries*>& b_series, std::vector<std::vector<QBarSet*>>& sets );

void appendToChart( QChart*const b_chart, const std::vector<QStackedBarSeries*>& b_series );

void setupChart( QChart*const b_chart, const QChart::ChartTheme& theme );

void attachXAxis( QChart*const b_chart, std::vector<QStackedBarSeries*>& b_series, const QStringList categories, const QString date );

void attachYAxis( QChart*const b_chart, std::vector<QStackedBarSeries*>& b_series, const int max_count );

} // namespace Warnings


namespace Speed
{

void applyGradient( QLineSeries*const line );

void appendToChart( QChart*const l_chart, QLineSeries*const line );

void setupChart( QChart*const l_chart, const QChart::ChartTheme& theme );

void attachXAxis( QChart*const l_chart, QLineSeries*const line, const QString& time_format, const QString date );

void attachYAxis( QChart*const l_chart, QLineSeries*const line, const int max_value );

} // namespace Speed


namespace Count
{

void setupPie( QPieSeries*const pie, const qreal pie_size );

void appendToChart( QChart*const p_chart, QPieSeries*const pie );

void setupChart( QChart*const p_chart, const QChart::ChartTheme& theme, const QString& title );

} // namespace Count


namespace Daytime
{

void setupSeries( QBarSeries*const bars, const QList<QBarSet*>& sets );

void appendToChart( QChart*const b_chart, QBarSeries*const bars );

void setupChart( QChart*const b_chart, const QChart::ChartTheme& theme, const QString& field_str );

void attachXAxis( QChart*const b_chart, QBarSeries*const bars, const QStringList& categories, const QString date );

void attachYAxis( QChart*const b_chart, QBarSeries*const bars, const int max_count );

} // namespace Daytime


namespace Relational
{

void applyGradient( QAreaSeries*const area );

void appendToChart( QChart*const a_chart, QAreaSeries*const area );

void setupChart( QChart*const a_chart, const QChart::ChartTheme& theme, const QString& field_1_str, const QString& field_2_str );

void attachXAxis( QChart*const a_chart, QAreaSeries*const area, const int n_ticks, const QString& time_format, const QString& date );

void attachYAxis( QChart*const a_chart, QAreaSeries*const area, const int max_count );

} // namespace Relational


} // namespace ChartOps


#endif // LOGDOCTOR__CRAPVIEW__UTILITIES__CHARTS_H


#include "colors.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QPalette>
#include <QColor>
#include <QString>


namespace ColorSec
{

const std::unordered_map<std::string, QColor> getColors()
{
    return {
        // greyscale
        {"black",      QColor(   0,   0,   0, 255 )},
        {"dark_grey",  QColor(  63,  63,  63, 255 )},
        {"grey",       QColor( 127, 127, 127, 255 )},
        {"light_grey", QColor( 191, 191, 191, 255 )},
        {"white",      QColor( 255, 255, 255, 255 )},
        // spectrum
        {"red",    QColor( 255,  44,   0, 255 )},
        {"orange", QColor( 255, 140,   0, 255 )},
        {"yellow", QColor( 255, 200,   0, 255 )},
    };
}


const std::unordered_map<int, std::unordered_map<std::string, QString>> getColorSchemes()
{
    return {
        // none
        {0,{{"background",""},
            {"text",""},
            {"x",""},
            {"ip",""},
            {"byte",""},
            {"time",""},
            {"ua",""},
            {"req",""},
            {"res",""} }},
        // breeze
        {1,{{"background","#ffffff"},
            {"text","#9c9c9b"},
            {"x","#1f1c1b"},
            {"ip","#644a9b"},
            {"byte","#d5bc79"},
            {"time","#d685d6"},
            {"ua","#006e28"},
            {"req","#54b8ff"},
            {"res","#d24f4f"} }},
        // monokai
        {2,{{"background","#272822"},
            {"text","#706c5a"},
            {"x","#a6a6a0"},
            {"ip","#57adbc"},
            {"byte","#c1b864"},
            {"time","#9773db"},
            {"ua","#a6e22e"},
            {"req","#d1d1cb"},
            {"res","#f92672"} }},
        // radical
        {3,{{"background","#141322"},
            {"text","#749295"},
            {"x","#7c9c9e"},
            {"ip","#fda8bc"},
            {"byte","#ff85a1"},
            {"time","#a8c0c2"},
            {"ua","#42a784"},
            {"req","#d5358f"},
            {"res","#56e8e4"} }}
    };
}


void applyChartTheme( const std::unordered_map<std::string, QFont>& fonts, QtCharts::QChartView* chart_view )
{
    QPen axis_pen;
    QPen grid_pen;
    QColor label_color;
    QBrush title_brush{ chart_view->foregroundBrush() };
    QBrush background_brush{ chart_view->backgroundBrush() };
    const QFont& small_font{ fonts.at("main_small") };
    const QFont& font{ fonts.at("main") };
    switch ( GlobalConfigs::charts_theme ) {
        case ChartsTheme::Light:
            label_color.setRgb( 16, 16, 16 );
            axis_pen.setColor( QColor( 56, 56, 56 ) );
            grid_pen.setColor( QColor( 160, 160, 160 ) );
            background_brush.setColor( QColor( 248, 248, 248 ) );
            break;
        case ChartsTheme::Dark:
            label_color.setRgb( 248, 248, 248 );
            axis_pen.setColor( QColor( 200, 200, 200 ) );
            grid_pen.setColor( QColor( 96, 96, 96 ) );
            background_brush.setColor( QColor( 24, 24, 24 ) );
            break;
        default:
            // shouldn't be here
            throw GenericException( "Unexpeced ChartsTheme ID: "+std::to_string(static_cast<themes_t>(GlobalConfigs::charts_theme)), true );
            break;
    }
    axis_pen.setWidthF( 1.1 );
    grid_pen.setWidthF( 0.8 );
    title_brush.setColor( label_color );
    background_brush.setStyle( Qt::SolidPattern );
    // apply to the chart
    QChart* chart{ chart_view->chart() };
    // title
    chart->setTitleFont( font );
    chart->setTitleBrush( title_brush );
    // legend
    chart->legend()->setFont( font );
    chart->legend()->setLabelColor( label_color );
    // axes
    QList<QAbstractAxis*> axes{ chart->axes() };
    for ( auto* axis : axes ) {
        axis->setLinePen( axis_pen );
        axis->setGridLinePen( grid_pen );
        axis->setLabelsColor( label_color );
        axis->setLabelsFont( small_font );
        axis->setTitleBrush( title_brush );
        axis->setTitleFont( small_font );
    }
    // background
    chart->setBackgroundBrush( background_brush );
    /*chart->setPlotAreaBackgroundBrush( background_brush );
    chart_view->setBackgroundBrush( background_brush );*/
}

} // namespace ColorSec

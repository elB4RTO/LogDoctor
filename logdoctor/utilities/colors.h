#ifndef COLORS_H
#define COLORS_H

#include <QtCharts>

#include <string>
#include <unordered_map>

class QString;
class QColor;
class QFont;


//! ColorSec
/*!
    Utilities for the colors
*/
namespace ColorSec
{

//! Provides a map with pre-made colors
const std::unordered_map<std::string, QColor> getColors();

//! Provides a map with pre-made color-schemes for the TextBrowser
const std::unordered_map<int, std::unordered_map<std::string, QString>> getColorSchemes();

//! Applies the choosen theme to the given chart
/*!
  \param fonts The fonts set
  \param chart_view The chart on which the theme will be applied
*/
void applyChartTheme( const std::unordered_map<std::string, QFont>& fonts, QtCharts::QChartView* chart_view );

} // namespace ColorSec

#endif // COLORS_H

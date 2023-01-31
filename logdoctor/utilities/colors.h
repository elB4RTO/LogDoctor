#ifndef COLORS_H
#define COLORS_H

#include <QPalette>
#include <QColor>
#include <QString>

#include <string>
#include <unordered_map>


//! ColorSec
/*!
    Utilities for the colors
*/
class ColorSec
{
public:
    explicit ColorSec();

    //! Provides a map with pre-made colors
    static const std::unordered_map<std::string, QColor> getColors();

    //! Provides a map with pre-made color-schemes for the TextBrowser
    static const std::unordered_map<int, std::unordered_map<std::string, QString>> getColorSchemes();
};

#endif // COLORS_H

#ifndef COLORS_H
#define COLORS_H

#include <QColor>
#include <QString>

#include <string>
#include <unordered_map>


class ColorSec
{
public:
    ColorSec();

    static std::unordered_map<std::string, QColor> getColors();
    static std::unordered_map<int, std::unordered_map<std::string, QString>> getColorSchemes();
};

#endif // COLORS_H

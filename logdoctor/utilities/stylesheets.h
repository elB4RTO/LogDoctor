#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>

#include <unordered_map>


class StyleSec
{
public:
    StyleSec();

    static void getStyleSheet( QString& stylesheet, const QString& icons_theme, const int& theme_id );

private:

    static void makeStyleSheet( QString& ss, const std::unordered_map<std::string, QString> &style, const QString& theme );
};

#endif // STYLESHEETS_H

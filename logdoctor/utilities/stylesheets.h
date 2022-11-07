#ifndef STYLESEC_H
#define STYLESEC_H

#include <QString>


class StyleSec
{
public:
    StyleSec();

    static void getStyleSheet( QString& stylesheet, const QString& icons_theme, const int& theme_id );

private:

    static void styleAsh( QString& ss, const QString& theme );
    void styleCandy( QString& ss, const QString& theme );
    void styleHerb( QString& ss, const QString& theme );
    void stylePowder( QString& ss, const QString& theme );
};

#endif // STYLESEC_H

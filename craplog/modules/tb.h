#ifndef LOGFILESTB_H
#define LOGFILESTB_H


#include "qfont.h"


class TextBrowser
{
public:
    TextBrowser();

    // getters
    bool getWideLinesUsage();
    int  getColorSchemeID();
    std::unordered_map<std::string, QString> getColorScheme();
    int  getFontSize();
    QString getFontFamily();
    QFont   getFont();

    // setters
    void setWideLinesUsage( bool use_wide_lines );
    void setColorScheme( int color_scheme_id, std::unordered_map<std::string, QString> color_scheme );
    void setFontSize( int font_size );
    void setFontFamily( QString font_family );
    void setFont( QFont font );

private:
    bool wide_lines = false;
    int  color_scheme_id = 1;
    std::unordered_map<std::string, QString> color_scheme;
    int  font_size = 13;
    QString font_family;
    QFont   font;
};

#endif // LOGFILESTB_H

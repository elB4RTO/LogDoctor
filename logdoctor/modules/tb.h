#ifndef LOGFILESTB_H
#define LOGFILESTB_H

#include <QFont>

#include <string>
#include <unordered_map>


class TextBrowser
{
public:
    TextBrowser();

    // getters
    const bool& getWideLinesUsage();
    const int&  getColorSchemeID();
    const std::unordered_map<std::string, QString>& getColorScheme();
    const int& getFontSize();
    const QString& getFontFamily();
    const QFont&   getFont();

    // setters
    void setWideLinesUsage( const bool& use_wide_lines );
    void setColorScheme( const int& color_scheme_id, const std::unordered_map<std::string, QString>& color_scheme );
    void setFontSize( const int& font_size );
    void setFontFamily( const QString& font_family );
    void setFont( const QFont& font );

    // preview
    void makePreview( QString& content );

private:
    bool wide_lines = false;
    int  color_scheme_id = 1;
    std::unordered_map<std::string, QString> color_scheme;
    int  font_size = 13;
    QString font_family;
    QFont   font;
};

#endif // LOGFILESTB_H

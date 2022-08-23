
#include "tb.h"


TextBrowser::TextBrowser()
{

}


// getters
const bool& TextBrowser::getWideLinesUsage()
{
    return this->wide_lines;
}

const int& TextBrowser::getColorSchemeID()
{
    return this->color_scheme_id;
}

const std::unordered_map<std::string, QString>& TextBrowser::getColorScheme()
{
    return this->color_scheme;
}

const int& TextBrowser::getFontSize()
{
    return this->font_size;
}

const QString& TextBrowser::getFontFamily()
{
    return this->font_family;
}

const QFont& TextBrowser::getFont()
{
    return this->font;
}


// setters
void TextBrowser::setWideLinesUsage( const bool& use_wide_lines )
{
    this->wide_lines = use_wide_lines;
}

void TextBrowser::setColorScheme( const int& color_scheme_id, const std::unordered_map<std::string, QString>& color_scheme )
{
    this->color_scheme_id = color_scheme_id;
    this->color_scheme = color_scheme;
}

void TextBrowser::setFontSize( const int& font_size )
{
    this->font_size = font_size;
}

void TextBrowser::setFontFamily( const QString& font_family )
{
    this->font_family = font_family;
}

void TextBrowser::setFont( const QFont& font )
{
    this->font = font;
}


// preview
void TextBrowser::makePreview( QString& content )
{
    content += QString("<!DOCTYPE html><html><head></head><body");
    if ( this->color_scheme_id > 0 ) {
        content += QString(" style=\"background:%1; color:%2\"")
            .arg( this->color_scheme.at("background"),
                  this->color_scheme.at("text") );
    }
    content += ">";
    if ( wide_lines ) {
        content += "<br/>";
    }
    for ( int i=0; i<32; i++ ) {
        content += "<p>";

        content += "<b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("time") );
        }
        content += "2000-01-01 23:59:59";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("req") );
        }
        content += "HTTP/1.1 GET /index.php query=x";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("res") );
        }
        content += "404</b>";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("x") );
        }
        content += "123 1234 1000";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("x") );
        }
        content += "http://www.referrer.site";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("x") );
        }
        content += "aCookie=abc123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("ua") );
        }
        content += "UserAgent/3.0 (Details stuff) Info/123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += QString("<span style=\"color:%1\">").arg( this->color_scheme.at("ip") );
        }
        content += "192.168.1.123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += "</p>";
        if ( this->wide_lines ) {
            content += "<br/>";
        }
    }
    content += "</body></html>";
}


#include "tb.h"


// getters
bool TextBrowser::getWideLinesUsage() const noexcept
{
    return this->wide_lines;
}

int TextBrowser::getColorSchemeID() const noexcept
{
    return this->color_scheme_id;
}

const std::unordered_map<std::string, QString>& TextBrowser::getColorScheme() const noexcept
{
    return this->color_scheme;
}

/*const int& TextBrowser::getFontSize() noexcept
{
    return this->font_size;
}*/

const QString& TextBrowser::getFontFamily() const noexcept
{
    return this->font_family;
}

const QFont& TextBrowser::getFont() const noexcept
{
    return this->font;
}


// setters
void TextBrowser::setWideLinesUsage( const bool& use_wide_lines ) noexcept
{
    this->wide_lines = use_wide_lines;
}

void TextBrowser::setColorScheme( const int& color_scheme_id, const std::unordered_map<std::string, QString>& color_scheme ) noexcept
{
    this->color_scheme_id = color_scheme_id;
    this->color_scheme = color_scheme;
}

/*void TextBrowser::setFontSize( const int& font_size ) noexcept
{
    this->font_size = font_size;
}

void TextBrowser::setFontFamily( const QString& font_family ) noexcept
{
    this->font_family = font_family;
}*/

void TextBrowser::setFont( const QFont& font ) noexcept
{
    this->font = font;
    this->font_family = font.family();
}


// preview
void TextBrowser::makePreview( QString& content ) const noexcept
{
    const QString span_template{ QStringLiteral("<span style=\"color:%1\">") };

    content += "<!DOCTYPE html><html><head></head><body";
    if ( this->color_scheme_id > 0 ) {
        content += QStringLiteral(" style=\"background:%1; color:%2\"")
            .arg( this->color_scheme.at("background"),
                  this->color_scheme.at("text") );
    }
    content += ">";
    /*if ( this->wide_lines ) {
        content += "<br/>";
    }*/
    for ( int i{0}; i<32; ++i ) {
        content += "<p>";

        content += "<b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("time") );
        }
        content += "2000-01-01 23:59:59";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("req") );
        }
        content += "HTTP/1.1 GET /index.php query=x";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("res") );
        }
        content += "404</b>";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("x") );
        }
        content += "123 1234 1000";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("x") );
        }
        content += "http://www.referrer.site";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("x") );
        }
        content += "aCookie=abc123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " \"<b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("ua") );
        }
        content += "UserAgent/3.0 (Details stuff) Info/123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>\"";

        content += " <b>";
        if ( this->color_scheme_id > 0 ) {
            content += span_template.arg( this->color_scheme.at("ip") );
        }
        content += "192.168.1.123";
        if ( this->color_scheme_id > 0 ) {
            content += "</span>";
        }
        content += "</b>";

        content += "</p>";
        if ( this->wide_lines && i < 31 ) {
            content += "<br/>";
        }
    }
    content += "</body></html>";
}

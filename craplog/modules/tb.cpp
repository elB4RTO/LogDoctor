
#include "tb.h"


TextBrowser::TextBrowser()
{

}


bool TextBrowser::getWideLinesUsage()
{
    return this->wide_lines;
}

int TextBrowser::getColorSchemeID()
{
    return this->color_scheme_id;
}

std::unordered_map<std::string, QString> TextBrowser::getColorScheme()
{
    return this->color_scheme;
}

int TextBrowser::getFontSize()
{
    return this->font_size;
}

QString TextBrowser::getFontFamily()
{
    return this->font_family;
}

QFont TextBrowser::getFont()
{
    return this->font;
}


// setters
void TextBrowser::setWideLinesUsage( bool use_wide_lines )
{
    this->wide_lines = use_wide_lines;
}

void TextBrowser::setColorScheme( int color_scheme_id, std::unordered_map<std::string, QString> color_scheme )
{
    this->color_scheme_id = color_scheme_id;
    this->color_scheme = color_scheme;
}

void TextBrowser::setFontSize( int font_size )
{
    this->font_size = font_size;
}

void TextBrowser::setFontFamily( QString font_family )
{
    this->font_family = font_family;
}

void TextBrowser::setFont( QFont font )
{
    this->font = font;
}

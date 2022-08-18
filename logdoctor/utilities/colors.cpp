
#include "colors.h"


ColorSec::ColorSec()
{

}

const std::unordered_map<std::string, QColor> ColorSec::getColors()
{
    const std::unordered_map<std::string, QColor> colors = {
        // greyscale
        {"black",      QColor(   0,   0,   0, 255 )},
        {"dark_grey",  QColor(  63,  63,  63, 255 )},
        {"grey",       QColor( 127, 127, 127, 255 )},
        {"light_grey", QColor( 191, 191, 191, 255 )},
        {"white",      QColor( 255, 255, 255, 255 )},
        // spectrum
        {"red",    QColor( 255,  44,   0, 255 )},
        {"orange", QColor( 255, 140,   0, 255 )},
        {"yellow", QColor( 255, 200,   0, 255 )},
    };
    return colors;
}


const std::unordered_map<int, std::unordered_map<std::string, QString>> ColorSec::getColorSchemes()
{
    const std::unordered_map<int, std::unordered_map<std::string, QString>> scheme = {
        // none
        {0,{{"background",""},
            {"text",""},
            {"x",""},
            {"ip",""},
            {"pt",""},
            {"time",""},
            {"ua",""},
            {"req",""},
            {"res",""} }},
        // breeze
        {1,{{"background","#ffffff"},
            {"text","#9c9c9b"},
            {"x","#1f1c1b"},
            {"ip","#644a9b"},
            {"pt","#d5bc79"},
            {"time","#d685d6"},
            {"ua","#006e28"},
            {"req","#54b8ff"},
            {"res","#d24f4f"} }},
        // monokai
        {2,{{"background","#272822"},
            {"text","#706c5a"},
            {"x","#a6a6a0"},
            {"ip","#57adbc"},
            {"pt","#c1b864"},
            {"time","#9773db"},
            {"ua","#a6e22e"},
            {"req","#d1d1cb"},
            {"res","#f92672"} }},
        // radical
        {3,{{"background","#141322"},
            {"text","#749295"},
            {"x","#7c9c9e"},
            {"ip","#fda8bc"},
            {"pt","#ff85a1"},
            {"time","#a8c0c2"},
            {"ua","#42a784"},
            {"req","#d5358f"},
            {"res","#56e8e4"} }}
    };
    return scheme;
}



const QPalette ColorSec::getPalette( const int&theme )
{
    QPalette p;
    switch ( theme ) {
        case 0:
            p = QPalette();
            break;
        case 1:
            // light
            p = QPalette( QBrush(QColor(  14,  20,   0 )), /* window text */
                          QBrush(QColor( 193, 175, 129 )), /* button */
                          QBrush(QColor( 164, 178, 140 )), /* light */
                          QBrush(QColor(  24,  38,   0 )), /* dark */
                          QBrush(QColor(  94, 108,  70 )), /* mid */
                          QBrush(QColor(  14,  20,   0 )), /* text */
                          QBrush(QColor(  54,  60,  40 )), /* bright text */
                          QBrush(QColor( 220, 211, 187 )), /* base */
                          QBrush(QColor( 220, 211, 187 ))  /* window */ );
            p.setBrush( QPalette::ColorRole::Highlight, QBrush(QColor(94,108,70)) );
            break;
        case 2:
            // dark
            p = QPalette( QBrush(QColor( 220, 211, 187 )), /* window text */
                          QBrush(QColor(  44,  58,  20 )), /* button */
                          QBrush(QColor(  24,  38,   0 )), /* light */
                          QBrush(QColor( 164, 178, 140 )), /* dark */
                          QBrush(QColor(  94, 108,  70 )), /* mid */
                          QBrush(QColor( 220, 211, 187 )), /* text */
                          QBrush(QColor( 193, 175, 129 )), /* bright text */
                          QBrush(QColor(  14,  20,   0 )), /* base */
                          QBrush(QColor(  14,  20,   0 ))  /* window */ );
            p.setBrush( QPalette::ColorRole::Highlight, QBrush(QColor(94,108,70)) );
            break;
    }

    return p;
}

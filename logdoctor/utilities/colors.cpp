
#include "colors.h"


ColorSec::ColorSec()
{

}

const std::unordered_map<std::string, QColor> ColorSec::getColors()
{
    std::unordered_map<std::string, QColor> colors;
                  // greyscale
         colors["black"] = QColor(   0,   0,   0, 255 );
     colors["dark_grey"] = QColor(  63,  63,  63, 255 );
          colors["grey"] = QColor( 127, 127, 127, 255 );
    colors["light_grey"] = QColor( 191, 191, 191, 255 );
         colors["white"] = QColor( 255, 255, 255, 255 );
                   // spectrum
           colors["red"] = QColor( 255,  44,   0, 255 );
        colors["orange"] = QColor( 255, 140,   0, 255 );
        colors["yellow"] = QColor( 255, 200,   0, 255 );

    return colors;
}


const std::unordered_map<int, std::unordered_map<std::string, QString> > ColorSec::getColorSchemes()
{
    std::unordered_map<int, std::unordered_map<std::string, QString>> scheme;
    scheme.emplace( 1, std::unordered_map<std::string, QString>() );
    scheme.emplace( 2, std::unordered_map<std::string, QString>() );
    scheme.emplace( 3, std::unordered_map<std::string, QString>() );

    // breeze
    scheme[1].emplace("background","#ffffff");
    scheme[1].emplace("text","#9c9c9b");
    scheme[1].emplace("x","#1f1c1b");
    scheme[1].emplace("ip","#644a9b");
    scheme[1].emplace("pt","#d5bc79");
    scheme[1].emplace("time","#d685d6");
    scheme[1].emplace("ua_src","#006e28");
    scheme[1].emplace("req_err","#54b8ff");
    scheme[1].emplace("res_lev","#d24f4f");

    // monokai
    scheme[2].emplace("background","#272822");
    scheme[2].emplace("text","#706c5a");
    scheme[2].emplace("x","#a6a6a0");
    scheme[2].emplace("ip","#57adbc");
    scheme[2].emplace("pt","#c1b864");
    scheme[2].emplace("time","#9773db");
    scheme[2].emplace("ua_src","#a6e22e");
    scheme[2].emplace("req_err","#d1d1cb");
    scheme[2].emplace("res_lev","#f92672");

    // radical
    scheme[3].emplace("background","#141322");
    scheme[3].emplace("text","#749295");
    scheme[3].emplace("x","#7c9c9e");
    scheme[3].emplace("ip","#fda8bc");
    scheme[3].emplace("pt","#ff85a1");
    scheme[3].emplace("time","#a8c0c2");
    scheme[3].emplace("ua_src","#42a784");
    scheme[3].emplace("req_err","#d5358f");
    scheme[3].emplace("res_lev","#56e8e4");

    return scheme;
}

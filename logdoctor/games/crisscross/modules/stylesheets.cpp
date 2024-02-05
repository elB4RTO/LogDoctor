
#include "stylesheets.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QString>
#include <QtCore/QStringBuilder>

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    WINDOW_BASE,
    TILES_BASE_HOVER,
    TILES_BORDER_HOVER,
    LINES_BASE,
    LINES_BORDER
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Light:
            return {
                {WINDOW_BASE,
                    "rgb( 250, 250, 255 )"},
                {TILES_BASE_HOVER,
                    "rgb( 230, 230, 230 )"},
                {TILES_BORDER_HOVER,
                    "rgb( 124, 119, 119 )"},
                {LINES_BASE,
                    "rgb( 88, 80, 80 )"},
                {LINES_BORDER,
                    "rgb( 230, 230, 230 )"}
            };
        case WindowTheme::Dark:
            return {
                {WINDOW_BASE,
                    "rgb( 13, 14, 15 )"},
                {TILES_BASE_HOVER,
                    "rgb( 27, 30, 33 )"},
                {TILES_BORDER_HOVER,
                    "rgb( 40, 45, 49 )"},
                {LINES_BASE,
                    "rgb( 96, 96, 96 )"},
                {LINES_BORDER,
                    "rgb( 27, 30, 33 )"}
            };
        default:
            throw GenericException( "Unexpected WindowTheme: "+std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)), true );
    }
}

} // namespace (private)


namespace StyleSec::Games::CrissCross
{

QString getStyleSheet()
{
    if ( GlobalConfigs::window_theme == WindowTheme::Native ) {
        return "";
    }
    const StyleMap style{ makeStyleMap() };
    return
        "QWidget#CrissCross {"
        "   background-color: "% style.at(WINDOW_BASE) %";"
        "}"
        "QPushButton#button_NE,"
        "QPushButton#button_N,"
        "QPushButton#button_NW,"
        "QPushButton#button_E,"
        "QPushButton#button_C,"
        "QPushButton#button_W,"
        "QPushButton#button_SE,"
        "QPushButton#button_S,"
        "QPushButton#button_SW {"
        "   border-radius: 4px;"
        "   border: 1px solid "% style.at(WINDOW_BASE) %";"
        "   background-color: "% style.at(WINDOW_BASE) %";"
        "}"
        "QPushButton#button_NE:hover,"
        "QPushButton#button_N:hover,"
        "QPushButton#button_NW:hover,"
        "QPushButton#button_E:hover,"
        "QPushButton#button_C:hover,"
        "QPushButton#button_W:hover,"
        "QPushButton#button_SE:hover,"
        "QPushButton#button_S:hover,"
        "QPushButton#button_SW:hover {"
        "   border-color: "% style.at(TILES_BORDER_HOVER) %";"
        "   background-color: "% style.at(TILES_BASE_HOVER) %";"
        "}"
        "QPushButton#button_NE::flat,"
        "QPushButton#button_N::flat,"
        "QPushButton#button_NW::flat,"
        "QPushButton#button_E::flat,"
        "QPushButton#button_C::flat,"
        "QPushButton#button_W::flat,"
        "QPushButton#button_SE::flat,"
        "QPushButton#button_S::flat,"
        "QPushButton#button_SW::flat {"
        "   border-color: "% style.at(WINDOW_BASE) %";"
        "   background-color: "% style.at(WINDOW_BASE) %";"
        "}"
        "QFrame#line_1,"
        "QFrame#line_2,"
        "QFrame#line_3,"
        "QFrame#line_4,"
        "QFrame#line_5,"
        "QFrame#line_6,"
        "QFrame#line_7,"
        "QFrame#line_8,"
        "QFrame#line_9,"
        "QFrame#line_10,"
        "QFrame#line_11,"
        "QFrame#line_12 {"
        "   border: 1px solid "% style.at(LINES_BORDER) %";"
        "   background-color: "% style.at(LINES_BASE) %";"
        "}";
}

} // namespace StyleSec::Games::CrissCross

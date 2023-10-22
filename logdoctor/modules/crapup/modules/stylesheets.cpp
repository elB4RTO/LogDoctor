
#include "stylesheets.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QString>

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    WINDOW_PRIMARY,
    WINDOW_SECONDARY
};

using StyleMap = std::unordered_map<StyleId, QString>;

const StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Light:
            return {
                {TEXT,
                    "rgb( 22, 11, 0 )"},
                {WINDOW_PRIMARY,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_SECONDARY,
                    "rgb( 230, 230, 230 )"}
            };
            break;
        case WindowTheme::Dark:
            return {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {WINDOW_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_SECONDARY,
                    "rgb( 27, 30, 33 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme: "+std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)), true );
            break;
    }
}

} // namespacce (private)


namespace StyleSec::Crapup
{

void getStyleSheet( QString& stylesheet )
{
    if ( GlobalConfigs::window_theme != WindowTheme::Native ) {
        const StyleMap style{ makeStyleMap() };
        stylesheet =
            "* {"
            "   color: "+style.at(TEXT)+";"
            "}"
            "QWidget#Crapup {"
            "   background-color: "+style.at(WINDOW_SECONDARY)+";"
            "}"
            "QFrame {"
            "   background-color: "+style.at(WINDOW_PRIMARY)+";"
            "}";
    }
}

} // namespacce StyleSec::Crapup

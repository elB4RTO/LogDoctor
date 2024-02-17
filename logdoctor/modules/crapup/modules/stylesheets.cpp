
#include "stylesheets.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QString>
#include <QtCore/QStringBuilder>

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    WINDOW_PRIMARY,
    WINDOW_SECONDARY
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
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
        case WindowTheme::Dark:
            return {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {WINDOW_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_SECONDARY,
                    "rgb( 27, 30, 33 )"}
            };
        default:
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }
}

} //namespace (private)


namespace StyleSec::Crapup
{

QString getStyleSheet()
{
    if ( GlobalConfigs::window_theme == WindowTheme::Native ) {
        return "";
    }
    const StyleMap style{ makeStyleMap() };
    return
        "* {"
        "   color: "% style.at(TEXT) %";"
        "}"
        "QWidget#Crapup {"
        "   background-color: "% style.at(WINDOW_SECONDARY) %";"
        "}"
        "QFrame {"
        "   background-color: "% style.at(WINDOW_PRIMARY) %";"
        "}";
}

} // namespacce StyleSec::Crapup


#include "stylesheets.h"

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

const StyleMap makeStyleMap( const int theme_id )
{
    switch ( theme_id ) {
        case 1: // light
            return {
                {TEXT,
                    "rgb( 22, 11, 0 )"},
                {WINDOW_PRIMARY,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_SECONDARY,
                    "rgb( 230, 230, 230 )"}
            };
            break;
        case 2: // dark
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
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
}

} // namespacce (private)


namespace StyleSec::Crapup
{

void getStyleSheet( QString& stylesheet, const int theme_id )
{
    if ( theme_id != 0 ) {
        const StyleMap style{ makeStyleMap( theme_id ) };
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


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
    WINDOW,
    BORDER_PRIMARY,
    BORDER_SECONDARY,
    SPINBOX_TEXT,
    SPINBOX_BASE,
    SPINBOX_BASE_FOCUS,
    SPINBOX_BASE_SELECTION
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
    case WindowTheme::Light:
        return {
            {TEXT,
                "rgb( 22, 11, 0 )"},
            {WINDOW,
                "rgb( 230, 230, 230 )"},
            {BORDER_PRIMARY,
                "rgb( 205, 200, 200 )"},
            {BORDER_SECONDARY,
                "rgb( 124, 119, 119 )"},
            {SPINBOX_TEXT,
                "rgb( 88, 80, 80 )"},
            {SPINBOX_BASE,
                "rgb( 216, 216, 216 )"},
            {SPINBOX_BASE_FOCUS,
                "rgb( 209, 209, 209 )"},
            {SPINBOX_BASE_SELECTION,
                "rgb( 153, 211, 255 )"}
        };
    case WindowTheme::Dark:
        return {
            {TEXT,
                "rgb( 248, 248, 248 )"},
            {WINDOW,
                "rgb( 27, 30, 33 )"},
            {BORDER_PRIMARY,
                "rgb( 50, 55, 59 )"},
            {BORDER_SECONDARY,
                "rgb( 107, 107, 107 )"},
            {SPINBOX_TEXT,
                "rgb( 210, 210, 210 )"},
            {SPINBOX_BASE,
                "rgb( 41, 44, 44 )"},
            {SPINBOX_BASE_FOCUS,
                "rgb( 57, 60, 60 )"},
            {SPINBOX_BASE_SELECTION,
                "rgb( 47, 99, 47 )"}
        };
    default:
        throw GenericException( "Unexpected WindowTheme: "+std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)), true );
    }
}

} // namespacce (private)


namespace StyleSec::Crapnote
{

QString getStyleSheet( const int color_scheme )
{
    QString icons_theme;
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Native:
            return "";
        case WindowTheme::Light:
            icons_theme = "dark";
            break;
        case WindowTheme::Dark:
            icons_theme = "light";
            break;
        default:
            throw GenericException( "Unexpected WindowTheme: "+std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)), true );
    }
    QString note_bg, note_txt;
    // update the colors palette
    switch ( color_scheme ) {
        case 0:
            if ( GlobalConfigs::window_theme == WindowTheme::Light ) {
                note_bg  = "rgb( 255, 255, 255 )";
                note_txt = "rgb(   0,   0,   0 )";
            } else {
                note_bg  = "rgb(   0,   0,   0 )";
                note_txt = "rgb( 255, 255, 255 )";
            }
            break;
        case 1:
            // breeze
            note_bg  = "rgb( 255, 198, 102 )";
            note_txt = "rgb(  31,  28,  27 )";
            break;
        case 2:
            // monokai
            note_bg  = "rgb( 166, 226,  46 )";
            note_txt = "rgb(  39,  40,  34 )";
            break;
        case 3:
            // radical
            note_bg  = "rgb(  20,  19,  34 )";
            note_txt = "rgb( 213,  53, 143 )";
            break;
        default:
            // wrong
            throw GenericException( "Unexpected ColorScheme ID for Crapnote: "+std::to_string( color_scheme ), true ); // leave un-catched
    }

    const StyleMap style{ makeStyleMap() };
    return
        "* {"
        "   color: "% style.at(TEXT) %";"
        "}"
        "QWidget#Crapnote {"
        "   background-color: "% style.at(WINDOW) %";"
        "}"
        "QFrame {"
        "   background-color: transparent;"
        "}"
        "QTextEdit {"
        "   color: "% note_txt %";"
        "   background-color: "% note_bg %";"
        "}"
        "QPushButton {"
        "   border: 1px solid "% style.at(BORDER_PRIMARY) %";"
        "   border-radius: 16px;"
        "   background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "   border: 1px solid "% style.at(BORDER_SECONDARY) %";"
        "}"
        "QSpinBox {"
        "   border: 0px solid;"
        "   border-radius: 4px;"
        "   color: "% style.at(SPINBOX_TEXT) %";"
        "   background-color: "% style.at(SPINBOX_BASE) %";"
        "   selection-color: "% style.at(SPINBOX_TEXT) %";"
        "   selection-background-color: "% style.at(SPINBOX_BASE_SELECTION) %";"
        "}"
        "QSpinBox::focus {"
        "   border: 1px solid "% style.at(BORDER_SECONDARY) %";"
        "   background-color: "% style.at(SPINBOX_BASE_FOCUS) %";"
        "}"
        "QPushButton#button_FontSize_Minus {"
        "   image: url(:/icons/icons/"% icons_theme %"/list_rem.png);"
        "}"
        "QPushButton#button_FontSize_Plus {"
        "   image: url(:/icons/icons/"% icons_theme %"/list_add.png);"
        "}";
}

} // namespacce StyleSec::Crapnote

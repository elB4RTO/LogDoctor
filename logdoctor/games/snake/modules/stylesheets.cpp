
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
    WINDOW_BASE_PRIMARY,
    WINDOW_BASE_SECONDARY,
    WINDOW_BORDER,
    BOXES_BASE_PRIMARY,
    BOXES_BASE_SECONDARY,
    BOXES_BASE_SELECTION,
    BOXES_DECO,
    PLAY_BUTTON_BASE,
    PLAY_BUTTON_BASE_HOVER,
    SCORE_FRAME_BASE,
    SCORE_FRAME_BORDER,
    SCORE_TEXT,
    SCORE_BASE,
    SCORE_BORDER
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Light:
            return {
                {TEXT,
                    "rgb( 22, 11, 0 )"},
                {WINDOW_BASE_PRIMARY,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_BASE_SECONDARY,
                    "rgb( 240, 240, 244 )"},
                {WINDOW_BORDER,
                    "transparent"},
                {BOXES_BASE_PRIMARY,
                    "rgb( 230, 230, 230 )"},
                {BOXES_BASE_SECONDARY,
                    "rgb( 220, 220, 220 )"},
                {BOXES_BASE_SELECTION,
                    "rgb( 123, 201, 255 )"},
                {BOXES_DECO,
                    "rgb( 99, 188, 255 )"},
                {PLAY_BUTTON_BASE,
                    "rgb( 99, 188, 255 )"},
                {PLAY_BUTTON_BASE_HOVER,
                    "rgb( 123, 201, 255 )"},
                {SCORE_FRAME_BASE,
                    "rgb( 230, 230, 230 )"},
                {SCORE_FRAME_BORDER,
                    "transparent"},
                {SCORE_TEXT,
                    "rgb( 22, 11, 0 )"},
                {SCORE_BASE,
                    "rgb( 240, 240, 240 )"},
                {SCORE_BORDER,
                    "transparent"}
            };
        case WindowTheme::Dark:
            return {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {WINDOW_BASE_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_BASE_SECONDARY,
                    "rgb( 27, 30, 33 )"},
                {WINDOW_BORDER,
                    "transparent"},
                {BOXES_BASE_PRIMARY,
                    "rgb( 39, 42, 45 )"},
                {BOXES_BASE_SECONDARY,
                    "rgb( 45, 48, 51 )"},
                {BOXES_BASE_SELECTION,
                    "rgb( 33, 162, 33 )"},
                {BOXES_DECO,
                    "rgb( 10, 155, 10 )"},
                {PLAY_BUTTON_BASE,
                    "rgb( 10, 155, 10 )"},
                {PLAY_BUTTON_BASE_HOVER,
                    "rgb( 33, 162, 33 )"},
                {SCORE_FRAME_BASE,
                    "rgb( 39, 42, 45 )"},
                {SCORE_FRAME_BORDER,
                    "transparent"},
                {SCORE_TEXT,
                    "rgb( 248, 248, 248 )"},
                {SCORE_BASE,
                    "rgb( 30, 33, 36 )"},
                {SCORE_BORDER,
                    "transparent"}
            };
        default:
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }
}

} //namespace (private)


namespace StyleSec::Games::Snake
{

QString getStyleSheet()
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
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }
    const StyleMap style{ makeStyleMap() };
    return
        "* {"
        "   color: "% style.at(TEXT) %";"
        "}"
        "QWidget#SnakeGame {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "}"
        "QWidget#stackedWidget_GameDisplay {"
        "   border: 1px solid "% style.at(WINDOW_BORDER) %";"
        "}"
        "QWidget#stackedPage_GameMenu {"
        "   border-radius: 16px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QWidget#stackedPage_GameBoard {"
        "   border-radius: 6px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QComboBox {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "}"
        "QComboBox:on {"
        "   border-bottom-left-radius: 0px;"
        "   border-bottom: 2px solid "% style.at(BOXES_DECO) %";"
        "   background-color: "% style.at(BOXES_BASE_SECONDARY) %";"
        "}"
        "QComboBox::drop-down {"
        "   border-top-right-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   border-left: 3px solid"
        "                QLinearGradient("
        "                   x0:0, y0:0, x1:0, y1:1,"
        "                   stop:0 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:0.1 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:0.5 "% style.at(BOXES_DECO) %","
        "                   stop:0.9 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:1 "% style.at(BOXES_BASE_PRIMARY) %");"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "   selection-background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "}"
        "QComboBox::drop-down:on {"
        "   border-bottom-right-radius: 0px;"
        "   border-left-color: transparent;"
        "   background-color: "% style.at(BOXES_BASE_SECONDARY) %";"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/icons/"% icons_theme %"/combobox_arrow.png);"
        "}"
        "QComboBox::down-arrow:on {"
        "   image: url();"
        "}"
        "QComboBox QAbstractItemView {"
        "   border-bottom-left-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "   selection-background-color: "% style.at(BOXES_BASE_SELECTION) %";"
        "}"
        "QPushButton#button_Play {"
        "   border-radius: 8px;"
        "   border: 0px;"
        "   background-color: "% style.at(PLAY_BUTTON_BASE) %";"
        "}"
        "QPushButton#button_Play:hover {"
        "   background-color: "% style.at(PLAY_BUTTON_BASE_HOVER) %";"
        "}"
        "QFrame#frame_Score {"
        "   border-radius: 4px;"
        "   border: 1px solid "% style.at(SCORE_FRAME_BORDER) %";"
        "   background-color: "% style.at(SCORE_FRAME_BASE) %";"
        "}"
        "QLCDNumber#lcd_Score {"
        "   border: 1px solid "% style.at(SCORE_BORDER) %";"
        "   color: "% style.at(SCORE_TEXT) %";"
        "   background-color: "% style.at(SCORE_BASE) %";"
        "}";
}

} // namespace StyleSec::Games::Snake

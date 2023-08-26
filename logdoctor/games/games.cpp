
#include "games.h"

#include "modules/exceptions.h"

#include <unordered_map>

#include <QString>


namespace GameSec
{

void crisscrossStyleSheet( QString& stylesheet, const int theme_id )
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0: // native
            break;
        case 1: // light
            style = {
                {"window_base",
                    "rgb( 250, 250, 255 )"},
                {"tiles_base_hover",
                    "rgb( 230, 230, 230 )"},
                {"tiles_border_hover",
                    "rgb( 124, 119, 119 )"},
                {"lines_base",
                    "rgb( 88, 80, 80 )"},
                {"lines_border",
                    "rgb( 230, 230, 230 )"}
            };
            break;
        case 2: // dark
            style = {
                {"window_base",
                    "rgb( 13, 14, 15 )"},
                {"tiles_base_hover",
                    "rgb( 27, 30, 33 )"},
                {"tiles_border_hover",
                    "rgb( 40, 45, 49 )"},
                {"lines_base",
                    "rgb( 96, 96, 96 )"},
                {"lines_border",
                    "rgb( 27, 30, 33 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "QWidget#CrissCross {"
            "   background-color: "+style.at("window_base")+";"
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
            "   border: 1px solid "+style.at("window_base")+";"
            "   background-color: "+style.at("window_base")+";"
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
            "   border-color: "+style.at("tiles_border_hover")+";"
            "   background-color: "+style.at("tiles_base_hover")+";"
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
            "   border-color: "+style.at("window_base")+";"
            "   background-color: "+style.at("window_base")+";"
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
            "   border: 1px solid "+style.at("lines_border")+";"
            "   background-color: "+style.at("lines_base")+";"
            "}";
    }
}


void snakeStyleSheet( QString& stylesheet, const int theme_id )
{
    QString icons_theme;
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:  // native
            break;
        case 1: // light
            icons_theme = "dark";
            style = {
                {"text",
                    "rgb( 22, 11, 0 )"},
                {"window_base_primary",
                    "rgb( 250, 250, 255 )"},
                {"window_base_secondary",
                    "rgb( 240, 240, 244 )"},
                {"window_border",
                    "transparent"},
                {"boxes_base_primary",
                    "rgb( 230, 230, 230 )"},
                {"boxes_base_secondary",
                    "rgb( 220, 220, 220 )"},
                {"boxes_base_selection",
                    "rgb( 123, 201, 255 )"},
                {"boxes_deco",
                    "rgb( 99, 188, 255 )"},
                {"play_button_base",
                    "rgb( 99, 188, 255 )"},
                {"play_button_base_hover",
                    "rgb( 123, 201, 255 )"},
                {"score_frame_base",
                    "rgb( 230, 230, 230 )"},
                {"score_frame_border",
                    "transparent"},
                {"score_text",
                    "rgb( 22, 11, 0 )"},
                {"score_base",
                    "rgb( 240, 240, 240 )"},
                {"score_border",
                    "transparent"}
            };
            break;
        case 2: // dark
            icons_theme = "light";
            style = {
                {"text",
                    "rgb( 248, 248, 248 )"},
                {"window_base_primary",
                    "rgb( 13, 14, 15 )"},
                {"window_base_secondary",
                    "rgb( 27, 30, 33 )"},
                {"window_border",
                    "transparent"},
                {"boxes_base_primary",
                    "rgb( 39, 42, 45 )"},
                {"boxes_base_secondary",
                    "rgb( 45, 48, 51 )"},
                {"boxes_base_selection",
                    "rgb( 33, 162, 33 )"},
                {"boxes_deco",
                    "rgb( 10, 155, 10 )"},
                {"play_button_base",
                    "rgb( 10, 155, 10 )"},
                {"play_button_base_hover",
                    "rgb( 33, 162, 33 )"},
                {"score_frame_base",
                    "rgb( 39, 42, 45 )"},
                {"score_frame_border",
                    "transparent"},
                {"score_text",
                    "rgb( 248, 248, 248 )"},
                {"score_base",
                    "rgb( 30, 33, 36 )"},
                {"score_border",
                    "transparent"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "* {"
            "   color: "+style.at("text")+";"
            "}"
            "QWidget#SnakeGame {"
            "   background-color: "+style.at("window_base_secondary")+";"
            "}"
            "QWidget#stackedWidget_GameDisplay {"
            "   border: 1px solid "+style.at("window_border")+";"
            "}"
            "QWidget#stackedPage_GameMenu {"
            "   border-radius: 16px;"
            "   background-color: "+style.at("window_base_primary")+";"
            "}"
            "QWidget#stackedPage_GameBoard {"
            "   border-radius: 6px;"
            "   background-color: "+style.at("window_base_primary")+";"
            "}"
            "QComboBox {"
            "   border-radius: 8px;"
            "   background-color: "+style.at("boxes_base_primary")+";"
            "}"
            "QComboBox:on {"
            "   border-bottom-left-radius: 0px;"
            "   border-bottom: 2px solid "+style.at("boxes_deco")+";"
            "   background-color: "+style.at("boxes_base_secondary")+";"
            "}"
            "QComboBox::drop-down {"
            "   border-top-right-radius: 8px;"
            "   border-bottom-right-radius: 8px;"
            "   border-left: 3px solid"
            "                QLinearGradient("
            "                   x0:0, y0:0, x1:0, y1:1,"
            "                   stop:0 "+style.at("boxes_base_primary")+","
            "                   stop:0.1 "+style.at("boxes_base_primary")+","
            "                   stop:0.5 "+style.at("boxes_deco")+","
            "                   stop:0.9 "+style.at("boxes_base_primary")+","
            "                   stop:1 "+style.at("boxes_base_primary")+");"
            "   background-color: "+style.at("boxes_base_primary")+";"
            "   selection-background-color: "+style.at("boxes_base_primary")+";"
            "}"
            "QComboBox::drop-down:on {"
            "   border-bottom-right-radius: 0px;"
            "   border-left-color: transparent;"
            "   background-color: "+style.at("boxes_base_secondary")+";"
            "}"
            "QComboBox::down-arrow {"
            "   image: url(:/icons/icons/"+icons_theme+"/combobox_arrow.png);"
            "}"
            "QComboBox::down-arrow:on {"
            "   image: url();"
            "}"
            "QComboBox QAbstractItemView {"
            "   border-bottom-left-radius: 8px;"
            "   border-bottom-right-radius: 8px;"
            "   background-color: "+style.at("boxes_base_primary")+";"
            "   selection-background-color: "+style.at("boxes_base_selection")+";"
            "}"
            "QPushButton#button_Play {"
            "   border-radius: 8px;"
            "   border: 0px;"
            "   background-color: "+style.at("play_button_base")+";"
            "}"
            "QPushButton#button_Play:hover {"
            "   background-color: "+style.at("play_button_base_hover")+";"
            "}"
            "QFrame#frame_Score {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("score_frame_border")+";"
            "   background-color: "+style.at("score_frame_base")+";"
            "}"
            "QLCDNumber#lcd_Score {"
            "   border: 1px solid "+style.at("score_border")+";"
            "   color: "+style.at("score_text")+";"
            "   background-color: "+style.at("score_base")+";"
            "}";
    }
}

} // namespace GameSec

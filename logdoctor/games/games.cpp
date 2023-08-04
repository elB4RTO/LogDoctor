
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
        case 0:
            break;
        case 1:
            style = {
                {"window_base",
                 "rgb( 170, 161, 137 )"},
                {"tiles_base_hover",
                 "rgb( 130, 121, 97 )"},
                {"tiles_border_hover",
                 "rgb( 140, 131, 107 )"},
                {"lines_base",
                 "rgb( 60, 51, 27 )"},
                {"lines_border",
                 "rgb( 170, 161, 137 )"}
            };
            break;
        case 2:
            style = {
                {"window_base",
                    "rgb( 32, 32, 32 )"},
                {"tiles_base_hover",
                    "rgb( 40, 40, 40 )"},
                {"tiles_border_hover",
                    "rgb( 64, 64, 64 )"},
                {"lines_base",
                    "rgb( 192, 192, 192 )"},
                {"lines_border",
                    "rgb( 16, 16, 16 )"}
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
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            style = {
                {"text",
                 "rgb( 30, 21, 0 )"},
                {"window_base_primary",
                 "rgb( 170, 161, 137 )"},
                {"window_base_secondary",
                 "rgb( 230, 221, 197 )"},
                {"window_border",
                 "rgb( 80, 71, 47 )"},
                {"gamemode_box_base",
                 "rgb( 130, 121, 97 )"},
                {"gamemode_box_base_selection",
                 "rgb( 140, 131, 107 )"},
                {"gamemode_box_border",
                 "rgb( 90, 81, 57 )"},
                {"play_button_base",
                 "rgb( 130, 121, 97 )"},
                {"play_button_base_hover",
                 "rgb( 140, 131, 107 )"},
                {"play_button_border",
                 "rgb( 90, 81, 57 )"},
                {"play_button_border_hover",
                 "rgb( 80, 71, 47 )"},
                {"score_frame_base",
                 "rgb( 210, 201, 177 )"},
                {"score_frame_border",
                 "rgb( 170, 161, 137 )"},
                {"score_text",
                 "rgb( 30, 21, 0 )"},
                {"score_base",
                 "rgb( 230, 221, 197 )"}
            };
            break;
        case 2:
            style = {
                {"text",
                    "rgb( 248, 248, 248 )"},
                {"window_base_primary",
                    "rgb( 16, 16, 16 )"},
                {"window_base_secondary",
                    "rgb( 32, 32, 32 )"},
                {"window_border",
                    "rgb( 128, 128, 128 )"},
                {"gamemode_box_base",
                    "rgb( 64, 64, 64 )"},
                {"gamemode_box_base_selection",
                    "rgb( 96, 96, 96 )"},
                {"gamemode_box_border",
                    "rgb( 82, 82, 82 )"},
                {"play_button_base",
                    "rgb( 64, 64, 64 )"},
                {"play_button_base_hover",
                    "rgb( 96, 96, 96 )"},
                {"play_button_border",
                    "rgb( 82, 82, 82 )"},
                {"play_button_border_hover",
                    "rgb( 128, 128, 128 )"},
                {"score_frame_base",
                    "rgb( 64, 64, 64 )"},
                {"score_frame_border",
                    "rgb( 82, 82, 82 )"},
                {"score_text",
                    "rgb( 248, 248, 248 )"},
                {"score_base",
                    "rgb( 0, 0, 0 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "QWidget#SnakeGame {"
            "   background-color: "+style.at("window_base_primary")+";"
            "}"
            "QWidget#stackedPage_GameMenu,"
            "QWidget#stackedPage_GameBoard {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("window_border")+";"
            "   background-color: "+style.at("window_base_secondary")+";"
            "}"
            "QComboBox {"
            "   color: "+style.at("text")+";"
            "   background-color: "+style.at("gamemode_box_base")+";"
            "   selection-background-color: "+style.at("gamemode_box_base_selection")+";"
            "}"
            "QComboBox QAbstractItemView {"
            "   border: 1px solid "+style.at("gamemode_box_border")+";"
            "   background-color: "+style.at("gamemode_box_base")+";"
            "}"
            "QPushButton#button_Play {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("play_button_border")+";"
            "   color: "+style.at("text")+";"
            "   background-color: "+style.at("play_button_base")+";"
            "}"
            "QPushButton#button_Play:hover {"
            "   border-color: "+style.at("play_button_border_hover")+";"
            "   background-color: "+style.at("play_button_base_hover")+";"
            "}"
            "QFrame#frame_Score {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("score_frame_border")+";"
            "   background-color: "+style.at("score_frame_base")+";"
            "}"
            "QLCDNumber#lcd_Score {"
            "   color: "+style.at("score_text")+";"
            "   background-color: "+style.at("score_base")+";"
            "}";
    }
}

} // namespace GameSec

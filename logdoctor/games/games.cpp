
#include "games.h"

#include "modules/exceptions.h"

#include <unordered_map>


GameSec::GameSec()
{

}


void GameSec::crisscrossStyleSheet( QString& stylesheet, const int& theme_id )
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
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
        case 2:
            break;
        case 3:
            break;
        case 4:
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


void GameSec::snakeStyleSheet( QString& stylesheet, const int& theme_id )
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            style = {
                {"window_base_primary",
                    "rgb( 16, 16, 16 )"},
                {"window_base_secondary",
                    "rgb( 32, 32, 32 )"},
                {"window_border",
                    "rgb( 128, 128, 128 )"},
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
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "QWidget#Snake {"
            "   background-color: "+style.at("window_base_primary")+";"
            "}"
            "QWidget#stackedPage_GameMenu,"
            "QWidget#stackedPage_GameBoard {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("window_border")+";"
            "   background-color: "+style.at("window_base_secondary")+";"
            "}"
            "QPushButton#button_Play {"
            "   border-radius: 4px;"
            "   border: 1px solid "+style.at("play_button_border")+";"
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

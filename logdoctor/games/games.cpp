
#include "games.h"

#include "modules/exceptions.h"


GameSec::GameSec()
{

}


void GameSec::crisscrossStyleSheet( QString& stylesheet, const int& theme_id )
{
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            stylesheet =
                "QWidget#CrissCross {"
                "   background-color: rgb( 32, 32, 32 );"
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
                "   border: 1px solid rgb( 32, 32, 32 );"
                "   background-color: rgb( 32, 32, 32 );"
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
                "   border-color: rgb( 64, 64, 64 );"
                "   background-color: rgb( 40, 40, 40 );"
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
                "   border-color: rgb( 32, 32, 32 );"
                "   background-color: rgb( 32, 32, 32 );"
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
                "   border: 1px solid rgb( 16, 16, 16 );"
                "   background-color: rgb( 192, 192, 192 );"
                "}";
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
}


void GameSec::snakeStyleSheet( QString& stylesheet, const int& theme_id )
{
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            stylesheet =
                "QWidget#Snake {"
                "   background-color: rgb( 16, 16, 16 );"
                "}"
                "QWidget#stackedPage_GameMenu,"
                "QWidget#stackedPage_GameBoard {"
                "   border-radius: 4px;"
                "   border: 1px solid rgb( 128, 128, 128 );"
                "   background-color: rgb( 32, 32, 32 );"
                "}"
                "QPushButton#button_Play {"
                "   border-radius: 4px;"
                "   border: 1px solid rgb( 82, 82, 82 );"
                "   background-color: rgb( 64, 64, 64 );"
                "}"
                "QPushButton#button_Play:hover {"
                "   border-color: rgb( 128, 128, 128 );"
                "   background-color: rgb( 96, 96, 96 );"
                "}"
                "QFrame#frame_Score {"
                "   border-radius: 4px;"
                "   border: 1px solid rgb( 82, 82, 82 );"
                "   background-color: rgb( 64, 64, 64 );"
                "}"
                "QLCDNumber#lcd_Score {"
                "   color: rgb( 248, 248, 248 );"
                "   background-color: rgb( 0, 0, 0 );"
                "}";
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
}

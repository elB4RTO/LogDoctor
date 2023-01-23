
#include "crapinfo.h"
#include "ui_crapinfo.h"

#include "modules/exceptions.h"

#include <unordered_map> // leave this here for OSX

#include <QFontDatabase>


Crapinfo::Crapinfo( const int& window_theme_id, const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::Crapinfo)
{
    ui->setupUi(this);
    QString stylesheet = "";
    this->getStyleSheet( stylesheet, window_theme_id );
    this->setStyleSheet( stylesheet );

    // fonts
    const QString ff = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0);
    const QFont font( ff, 13 );
    const QString ff_s = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Hack")).at(0);
    const QFont font_script( ff_s, 13 );

    // apply the fonts
    this->ui->toolBox_Infos->setFont( font );
    // version
    this->ui->label_Verion_Number->setText( QString(version).replace(",",".") );
    this->ui->label_Verion_Number->setFont( font_script );
    // paths
    this->ui->inLine_ExecPath->setText( exec_path );
    this->ui->inLine_ExecPath->setCursorPosition( 0 );
    this->ui->inLine_ConfPath->setText( conf_path );
    this->ui->inLine_ConfPath->setCursorPosition( 0 );
    this->ui->inLine_DataPath->setText( logdoc_path );
    this->ui->inLine_DataPath->setCursorPosition( 0 );

}

Crapinfo::~Crapinfo()
{
    delete ui;
}


void Crapinfo::getStyleSheet( QString& stylesheet, const int& theme_id ) const
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            style = {
                {"text",
                    "rgb( 248, 248, 248 )"},
                {"text_unselected",
                    "rgb( 192, 192, 192 )"},
                {"window_primary",
                    "rgb( 32, 32, 32 )"},
                {"window_secondary",
                    "rgb( 16, 16, 16 )"},
                {"border",
                    "rgb( 96, 96, 96 )"},
                {"border_hover",
                    "rgb( 128, 128, 128 )"},
                {"border_unselected",
                    "rgb( 64, 64, 64 )"},
                {"toolbox_tab_base",
                    "rgb( 24, 24, 24 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 48, 48, 48 )"},
                {"scrollbar_base",
                    "rgb( 96, 96, 96 )"},
                {"scrollbar_handler",
                    "rgb( 192, 192, 192 )"},
                {"linedit_text",
                    "rgb( 16, 16, 16 )"},
                {"linedit_base",
                    "rgb( 216, 216, 216 )"},
                {"linedit_text_selection",
                    "rgb( 16, 16, 16 )"},
                {"linedit_base_selection",
                    "rgb( 192, 192, 192 )"},
                {"paths_frame_base",
                    "rgb( 48, 48, 48 )"},
                {"lines",
                    "rgb( 192, 192, 192 )"}
            };
            break;
        case 2:
            style = {
                {"text",
                    "rgb( 45, 0, 30 )"},
                {"text_unselected",
                    "rgb( 115, 30, 70 )"},
                {"window_primary",
                    "rgb( 255, 140, 141 )"},
                {"window_secondary",
                    "rgb( 255, 204, 143 )"},
                {"border",
                    "rgb( 195, 80, 81 )"},
                {"border_hover",
                    "rgb( 215, 100, 101 )"},
                {"border_unselected",
                    "rgb( 175, 60, 61 )"},
                {"toolbox_tab_base",
                    "rgb( 245, 172, 142 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 245, 130, 131 )"},
                {"scrollbar_base",
                    "rgb( 145, 100, 130 )"},
                {"scrollbar_handler",
                    "rgb( 45, 0, 30 )"},
                {"linedit_text",
                    "rgb( 45, 0, 30 )"},
                {"linedit_base",
                    "rgb( 113, 154, 225 )"},
                {"linedit_text_selection",
                    "rgb( 45, 0, 30 )"},
                {"linedit_base_selection",
                    "rgb( 145, 100, 130 )"},
                {"paths_frame_base",
                    "rgb( 245, 172, 142 )"},
                {"lines",
                    "rgb( 195, 80, 81 )"}
            };
            break;
        case 3:
            style = {
                {"text",
                    "rgb( 220, 211, 187 )"},
                {"text_unselected",
                    "rgb( 163, 145, 99 )"},
                {"window_primary",
                    "rgb( 34, 58, 10 )"},
                {"window_secondary",
                    "rgb( 14, 28, 0 )"},
                {"border",
                    "rgb( 163, 145, 99 )"},
                {"border_hover",
                    "rgb( 193, 175, 129 )"},
                {"border_unselected",
                    "rgb( 133, 115, 69 )"},
                {"toolbox_tab_base",
                    "rgb( 24, 48, 0 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 54, 78, 30 )"},
                {"scrollbar_base",
                    "rgb( 94, 118, 70 )"},
                {"scrollbar_handler",
                    "rgb( 174, 198, 150 )"},
                {"linedit_text",
                    "rgb( 4, 18, 0 )"},
                {"linedit_base",
                    "rgb( 193, 175, 129 )"},
                {"linedit_text_selection",
                    "rgb( 4, 18, 0 )"},
                {"linedit_base_selection",
                    "rgb( 173, 155, 109 )"},
                {"paths_frame_base",
                    "rgb( 54, 78, 30 )"},
                {"lines",
                    "rgb( 124, 148, 100 )"}
            };
            break;
        case 4:
            style = {
                {"text",
                    "rgb( 30, 21, 0 )"},
                {"text_unselected",
                    "rgb( 60, 51, 27 )"},
                {"window_primary",
                    "rgb( 170, 161, 137 )"},
                {"window_secondary",
                    "rgb( 230, 221, 197 )"},
                {"border",
                    "rgb( 80, 71, 47 )"},
                {"border_hover",
                    "rgb( 60, 51, 27 )"},
                {"border_unselected",
                    "rgb( 100, 91, 67 )"},
                {"toolbox_tab_base",
                    "rgb( 190, 181, 157 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 150, 141, 117 )"},
                {"scrollbar_base",
                    "rgb( 100, 91, 67 )"},
                {"scrollbar_handler",
                    "rgb( 210, 201, 177 )"},
                {"linedit_text",
                    "rgb( 30, 21, 0 )"},
                {"linedit_base",
                    "rgb( 210, 201, 177 )"},
                {"linedit_text_selection",
                    "rgb( 30, 21, 0 )"},
                {"linedit_base_selection",
                    "rgb( 170, 161, 137 )"},
                {"paths_frame_base",
                    "rgb( 190, 181, 157 )"},
                {"lines",
                    "rgb( 60, 51, 27 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "QWidget#Crapinfo {"
            "   color: "+style.at("text")+";"
            "   background-color: "+style.at("window_secondary")+";"
            "}"
            "QFrame {"
            "   background-color: transparent;"
            "}"
            "QLabel {"
            "   color: "+style.at("text")+";"
            "}"
            "QPushButton:pressed {"
            "   background-color: "+style.at("window_primary")+";"
            "}"
            "QWidget#page_LogDoc,"
            "QWidget#page_Paths,"
            "QWidget#scrollAreaContents_LogDoc {"
            "   border: 1px solid "+style.at("border")+";"
            "   border-top: 0px;"
            "   border-radius: 4px;"
            "   border-top-left-radius: 0px;"
            "   border-top-right-radius: 0px;"
            "   background-color: "+style.at("window_primary")+";"
            "}"
            "QToolBox::tab {"
            "   border: 1px solid "+style.at("border_unselected")+";"
            "   border-radius: 4px;"
            "   color: "+style.at("text_unselected")+";"
            "   background-color: "+style.at("toolbox_tab_base")+";"
            "}"
            "QToolBox::tab:!selected:hover {"
            "   border-color: "+style.at("border_hover")+";"
            "   color: "+style.at("text")+";"
            "   background-color: "+style.at("toolbox_tab_base")+";"
            "}"
            "QToolBox::tab:selected {"
            "   border-color: "+style.at("border")+";"
            "   border-top: 4px solid "+style.at("border")+";"
            "   border-bottom: 0px;"
            "   border-bottom-left-radius: 0px;"
            "   border-bottom-right-radius: 0px;"
            "   color: "+style.at("text")+";"
            "   background-color: "+style.at("toolbox_tab_base_selected")+";"
            "}"
            "QScrollBar:horizontal {"
            "   height: 12px;"
            "   background-color: "+style.at("scrollbar_base")+";"
            "}"
            "QScrollBar::handle:horizontal {"
            "   min-width: 16px;"
            "   margin: 5px 12px 5px 12px;"
            "   background-color: "+style.at("scrollbar_handler")+";"
            "}"
            "QScrollBar::handle:horizontal:hover {"
            "   margin: 4px 12px 4px 12px;"
            "}"
            "QLineEdit {"
            "   border-radius: 4px;"
            "   color: "+style.at("linedit_text")+";"
            "   selection-color: "+style.at("linedit_text_selection")+";"
            "   background-color: "+style.at("linedit_base")+";"
            "   selection-background-color: "+style.at("linedit_base_selection")+";"
            "}"
            "QWidget#scrollAreaContents_Paths_Executable,"
            "QWidget#scrollAreaContents_Paths_ConfigFile,"
            "QWidget#scrollAreaContents_Paths_AppData {"
            "   border-radius: 4px;"
            "   background-color: "+style.at("paths_frame_base")+";"
            "}"
            "QFrame#gline_Version,"
            "QFrame#gline_StatsCount {"
            "   border: 1px solid "+style.at("window_secondary")+";"
            "   margin: 2px 0px 3px 0px;"
            "   background-color: "+style.at("lines")+";"
            "}"
            "";
    }
}

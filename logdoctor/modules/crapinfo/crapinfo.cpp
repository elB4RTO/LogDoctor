
#include "crapinfo.h"
#include "ui_crapinfo.h"

#include "modules/exceptions.h"

#include <unordered_map> // leave this here for OSX

#include <QFontDatabase>


Crapinfo::Crapinfo( const int window_theme_id, const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent )
    : QWidget{ parent }
    , ui{ new Ui::Crapinfo }
{
    ui->setupUi(this);
    QString stylesheet{ "" };
    this->getStyleSheet( stylesheet, window_theme_id );
    this->setStyleSheet( stylesheet );

    // fonts
    const QString& ff{ QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0) };
    const QFont font{ ff, 13 };
    const QString& ff_s{ QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Hack")).at(0) };
    const QFont font_script{ ff_s, 13 };

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


void Crapinfo::getStyleSheet( QString& stylesheet, const int theme_id ) const
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0: // native
            break;
        case 1: // light
            style = {
                {"text",
                    "rgb( 22, 11, 0 )"},
                {"text_unselected",
                    "rgb( 64, 60, 60 )"},
                {"window_primary",
                    "rgb( 250, 250, 255 )"},
                {"window_secondary",
                    "rgb( 230, 230, 230 )"},
                {"border",
                    "rgb( 210, 210, 210 )"},
                {"border_hover",
                    "rgb( 124, 119, 119 )"},
                {"border_unselected",
                    "transparent"},
                {"toolbox_tab_base",
                    "rgb( 210, 210, 210 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 240, 240, 245 )"},
                {"scrollbar_base",
                    "rgb( 250, 250, 255 )"},
                {"scrollbar_handler",
                    "rgb( 40, 185, 40 )"},
                {"scrollbar_controls",
                    "transparent"},
                {"linedit_text",
                    "rgb( 88, 80, 80 )"},
                {"linedit_base",
                    "rgb( 236, 236, 236 )"},
                {"linedit_text_selection",
                    "rgb( 10, 10, 10 )"}, //!!!
                {"linedit_base_selection",
                    "rgb( 197, 191, 185 )"},
                {"paths_frame_base",
                    "rgb( 240, 240, 245 )"},
                {"separators",
                    "rgb( 88, 80, 80 )"}
            };
            break;
        case 2: // dark
            style = {
                {"text",
                    "rgb( 248, 248, 248 )"},
                {"text_unselected",
                    "rgb( 192, 192, 192 )"},
                {"window_primary",
                    "rgb( 13, 14, 15 )"},
                {"window_secondary",
                    "rgb( 27, 30, 33 )"},
                {"border",
                    "rgb( 40, 45, 49 )"},
                {"border_hover",
                    "rgb( 64, 64, 64 )"},
                {"border_unselected",
                    "transparent"},
                {"toolbox_tab_base",
                    "rgb( 21, 23, 25 )"},
                {"toolbox_tab_base_selected",
                    "rgb( 17, 18, 19 )"},
                {"scrollbar_base",
                    "transparent"},
                {"scrollbar_handler",
                    "rgb( 69, 177, 255 )"},
                {"scrollbar_controls",
                    "transparent"},
                {"linedit_text",
                    "rgb( 210, 210, 210 )"},
                {"linedit_base",
                    "rgb( 41, 44, 44 )"},
                {"linedit_text_selection",
                    "rgb( 245, 245, 245 )"},
                {"linedit_base_selection",
                    "rgb( 47, 99, 47 )"},
                {"paths_frame_base",
                    "rgb( 32, 35, 37 )"},
                {"separators",
                    "rgb( 96, 96, 96 )"}
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
            "   border: 0px;"
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
            "   border-radius: 8px;"
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
            "QScrollBar {"
            "   border: 0px solid transparent;"
            "}"
            "QScrollBar:horizontal {"
            "   height: 12px;"
            "   background-color: "+style.at("scrollbar_base")+";"
            "}"
            "QScrollBar::handle:horizontal {"
            "   min-width: 16px;"
            "   margin: 5px 0px 5px 0px;"
            "   background-color: "+style.at("scrollbar_handler")+";"
            "}"
            "QScrollBar::handle:horizontal:hover {"
            "   margin: 4px 0px 4px 0px;"
            "   border-radius: 2px;"
            "}"
            "QScrollBar:vertical {"
            "   width: 12px;"
            "   background-color: "+style.at("scrollbar_base")+";"
            "}"
            "QScrollBar::handle:vertical {"
            "   min-width: 16px;"
            "   margin: 0px 5px 0px 5px;"
            "   background-color: "+style.at("scrollbar_handler")+";"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "   margin: 0px 4px 0px 4px;"
            "   border-radius: 2px;"
            "}"
            "QScrollBar::add-line,"
            "QScrollBar::sub-line,"
            "QScrollBar::add-pae,"
            "QScrollBar::sub-pae,"
            "QScrollBar::up-arrow,"
            "QScrollBar::down-arrow,"
            "QScrollBar::left-arrow,"
            "QScrollBar::right-arrow {"
            "   border: 0px;"
            "   background-color: "+style.at("scrollbar_controls")+";"
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
            "   border-radius: 8px;"
            "   background-color: "+style.at("paths_frame_base")+";"
            "}"
            "QFrame#gline_Version {"
            "   border: 1px solid "+style.at("window_primary")+";"
            "   margin: 2px 0px 3px 0px;"
            "   background-color: "+style.at("separators")+";"
            "}";
    }
}

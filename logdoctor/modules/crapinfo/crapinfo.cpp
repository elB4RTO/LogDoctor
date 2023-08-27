
#include "crapinfo.h"
#include "ui_crapinfo.h"

#include "modules/exceptions.h"

#include <unordered_map> // leave this here for OSX

#include <QFontDatabase>


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    TEXT_UNSELECTED,
    WINDOW_PRIMARY,
    WINDOW_SECONDARY,
    BORDER,
    BORDER_HOVER,
    BORDER_UNSELECTED,
    TOOLBOX_TAB_BASE,
    TOOLBOX_TAB_BASE_SELECTED,
    SCROLLBAR_BASE,
    SCROLLBAR_HANDLER,
    SCROLLBAR_CONTROLS,
    LINEDIT_TEXT,
    LINEDIT_BASE,
    LINEDIT_TEXT_SELECTION,
    LINEDIT_BASE_SELECTION,
    PATHS_FRAME_BASE,
    SEPARATORS
};

} // namespace (private)


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
    std::unordered_map<StyleId, QString> style;
    switch ( theme_id ) {
        case 0: // native
            break;
        case 1: // light
            style = {
                {TEXT,
                    "rgb( 22, 11, 0 )"},
                {TEXT_UNSELECTED,
                    "rgb( 64, 60, 60 )"},
                {WINDOW_PRIMARY,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_SECONDARY,
                    "rgb( 230, 230, 230 )"},
                {BORDER,
                    "rgb( 210, 210, 210 )"},
                {BORDER_HOVER,
                    "rgb( 124, 119, 119 )"},
                {BORDER_UNSELECTED,
                    "transparent"},
                {TOOLBOX_TAB_BASE,
                    "rgb( 210, 210, 210 )"},
                {TOOLBOX_TAB_BASE_SELECTED,
                    "rgb( 240, 240, 245 )"},
                {SCROLLBAR_BASE,
                    "rgb( 250, 250, 255 )"},
                {SCROLLBAR_HANDLER,
                    "rgb( 40, 185, 40 )"},
                {SCROLLBAR_CONTROLS,
                    "transparent"},
                {LINEDIT_TEXT,
                    "rgb( 88, 80, 80 )"},
                {LINEDIT_BASE,
                    "rgb( 236, 236, 236 )"},
                {LINEDIT_TEXT_SELECTION,
                    "rgb( 10, 10, 10 )"},
                {LINEDIT_BASE_SELECTION,
                    "rgb( 197, 191, 185 )"},
                {PATHS_FRAME_BASE,
                    "rgb( 240, 240, 245 )"},
                {SEPARATORS,
                    "rgb( 88, 80, 80 )"}
            };
            break;
        case 2: // dark
            style = {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {TEXT_UNSELECTED,
                    "rgb( 192, 192, 192 )"},
                {WINDOW_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_SECONDARY,
                    "rgb( 27, 30, 33 )"},
                {BORDER,
                    "rgb( 40, 45, 49 )"},
                {BORDER_HOVER,
                    "rgb( 64, 64, 64 )"},
                {BORDER_UNSELECTED,
                    "transparent"},
                {TOOLBOX_TAB_BASE,
                    "rgb( 21, 23, 25 )"},
                {TOOLBOX_TAB_BASE_SELECTED,
                    "rgb( 17, 18, 19 )"},
                {SCROLLBAR_BASE,
                    "transparent"},
                {SCROLLBAR_HANDLER,
                    "rgb( 69, 177, 255 )"},
                {SCROLLBAR_CONTROLS,
                    "transparent"},
                {LINEDIT_TEXT,
                    "rgb( 210, 210, 210 )"},
                {LINEDIT_BASE,
                    "rgb( 41, 44, 44 )"},
                {LINEDIT_TEXT_SELECTION,
                    "rgb( 245, 245, 245 )"},
                {LINEDIT_BASE_SELECTION,
                    "rgb( 47, 99, 47 )"},
                {PATHS_FRAME_BASE,
                    "rgb( 32, 35, 37 )"},
                {SEPARATORS,
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
            "   color: "+style.at(TEXT)+";"
            "   background-color: "+style.at(WINDOW_SECONDARY)+";"
            "}"
            "QFrame {"
            "   border: 0px;"
            "   background-color: transparent;"
            "}"
            "QLabel {"
            "   color: "+style.at(TEXT)+";"
            "}"
            "QPushButton:pressed {"
            "   background-color: "+style.at(WINDOW_PRIMARY)+";"
            "}"
            "QWidget#page_LogDoc,"
            "QWidget#page_Paths,"
            "QWidget#scrollAreaContents_LogDoc {"
            "   border: 1px solid "+style.at(BORDER)+";"
            "   border-top: 0px;"
            "   border-radius: 4px;"
            "   border-top-left-radius: 0px;"
            "   border-top-right-radius: 0px;"
            "   background-color: "+style.at(WINDOW_PRIMARY)+";"
            "}"
            "QToolBox::tab {"
            "   border: 1px solid "+style.at(BORDER_UNSELECTED)+";"
            "   border-radius: 8px;"
            "   color: "+style.at(TEXT_UNSELECTED)+";"
            "   background-color: "+style.at(TOOLBOX_TAB_BASE)+";"
            "}"
            "QToolBox::tab:!selected:hover {"
            "   border-color: "+style.at(BORDER_HOVER)+";"
            "   color: "+style.at(TEXT)+";"
            "   background-color: "+style.at(TOOLBOX_TAB_BASE)+";"
            "}"
            "QToolBox::tab:selected {"
            "   border-color: "+style.at(BORDER)+";"
            "   border-top: 4px solid "+style.at(BORDER)+";"
            "   border-bottom: 0px;"
            "   border-bottom-left-radius: 0px;"
            "   border-bottom-right-radius: 0px;"
            "   color: "+style.at(TEXT)+";"
            "   background-color: "+style.at(TOOLBOX_TAB_BASE_SELECTED)+";"
            "}"
            "QScrollBar {"
            "   border: 0px solid transparent;"
            "}"
            "QScrollBar:horizontal {"
            "   height: 12px;"
            "   background-color: "+style.at(SCROLLBAR_BASE)+";"
            "}"
            "QScrollBar::handle:horizontal {"
            "   min-width: 16px;"
            "   margin: 5px 0px 5px 0px;"
            "   background-color: "+style.at(SCROLLBAR_HANDLER)+";"
            "}"
            "QScrollBar::handle:horizontal:hover {"
            "   margin: 4px 0px 4px 0px;"
            "   border-radius: 2px;"
            "}"
            "QScrollBar:vertical {"
            "   width: 12px;"
            "   background-color: "+style.at(SCROLLBAR_BASE)+";"
            "}"
            "QScrollBar::handle:vertical {"
            "   min-width: 16px;"
            "   margin: 0px 5px 0px 5px;"
            "   background-color: "+style.at(SCROLLBAR_HANDLER)+";"
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
            "   background-color: "+style.at(SCROLLBAR_CONTROLS)+";"
            "}"
            "QLineEdit {"
            "   border-radius: 4px;"
            "   color: "+style.at(LINEDIT_TEXT)+";"
            "   selection-color: "+style.at(LINEDIT_TEXT_SELECTION)+";"
            "   background-color: "+style.at(LINEDIT_BASE)+";"
            "   selection-background-color: "+style.at(LINEDIT_BASE_SELECTION)+";"
            "}"
            "QWidget#scrollAreaContents_Paths_Executable,"
            "QWidget#scrollAreaContents_Paths_ConfigFile,"
            "QWidget#scrollAreaContents_Paths_AppData {"
            "   border-radius: 8px;"
            "   background-color: "+style.at(PATHS_FRAME_BASE)+";"
            "}"
            "QFrame#gline_Version {"
            "   border: 1px solid "+style.at(WINDOW_PRIMARY)+";"
            "   margin: 2px 0px 3px 0px;"
            "   background-color: "+style.at(SEPARATORS)+";"
            "}";
    }
}

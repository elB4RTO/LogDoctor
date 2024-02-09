
#include "message_dialog.h"
#include "ui_message_dialog.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QSize>
#include <QIcon>
#include <QPixmap>

#include <unordered_map> // leave this here for clang


enum StyleId : uint32_t {
    TEXT,
    WINDOW,
    BUTTONS_BASE,
    BUTTONS_BASE_HOVER,
    TEXTBROWSER_BASE,
    TEXTBROWSER_BORDER,
    TEXTBROWSER_TEXT_SELECTION,
    TEXTBROWSER_BASE_SELECTION,
    SCROLLBAR_BASE,
    SCROLLBAR_HANDLER,
    SCROLLBAR_CONTROLS
};

QString getStylesheet()
{
    std::unordered_map<StyleId, QString> style;
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Light:
            style = {
                {TEXT,
                    "rgb( 22, 11, 0 )"},
                {WINDOW,
                    "rgb( 250, 250, 255 )"},
                {BUTTONS_BASE,
                    "rgb( 99, 188, 255 )"},
                {BUTTONS_BASE_HOVER,
                    "rgb( 123, 201, 255 )"},
                {TEXTBROWSER_BASE,
                    "rgb( 236, 236, 236 )"},
                {TEXTBROWSER_BORDER,
                    "rgb( 236, 236, 236 )"},
                {TEXTBROWSER_TEXT_SELECTION,
                    "rgb( 68, 60, 60 )"},
                {TEXTBROWSER_BASE_SELECTION,
                    "rgb( 207, 201, 195 )"},
                {SCROLLBAR_BASE,
                    "rgb( 236, 236, 236 )"},
                {SCROLLBAR_HANDLER,
                    "rgb( 40, 185, 40 )"},
                {SCROLLBAR_CONTROLS,
                    "transparent"}
            };
            break;
        case WindowTheme::Dark:
            style = {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {WINDOW,
                    "rgb( 13, 14, 15 )"},
                {BUTTONS_BASE,
                    "rgb( 10, 155, 10 )"},
                {BUTTONS_BASE_HOVER,
                    "rgb( 33, 162, 33 )"},
                {TEXTBROWSER_BASE,
                    "rgb( 32, 32, 32 )"},
                {TEXTBROWSER_BORDER,
                    "rgb( 32, 32, 32 )"},
                {TEXTBROWSER_TEXT_SELECTION,
                    "rgb( 248, 248, 248 )"},
                {TEXTBROWSER_BASE_SELECTION,
                    "rgb( 64, 64, 64 )"},
                {SCROLLBAR_BASE,
                    "rgb( 32, 32, 32 )"},
                {SCROLLBAR_HANDLER,
                    "rgb( 69, 177, 255 )"},
                {SCROLLBAR_CONTROLS,
                    "transparent"}
            };
            break;
        default:
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }

    return
        "* {"
        "   color: "+style.at(TEXT)+";"
        "   background-color: "+style.at(WINDOW)+";"
        "}"
        "QPushButton#button_Ok {"
        "   border-radius: 8px;"
        "   background-color: "+style.at(BUTTONS_BASE)+";"
        "}"
        "QPushButton#button_Ok:hover {"
        "   background-color: "+style.at(BUTTONS_BASE_HOVER)+";"
        "}"
        "QPushButton#button_ShowAdditional {"
        "   border-radius: 4px;"
        "}"
        "QPushButton#button_ShowAdditional:hover {"
        "   background-color: "+style.at(TEXTBROWSER_BASE)+";"
        "}"
        "QTextBrowser {"
        "   border: 1px solid "+style.at(TEXTBROWSER_BORDER)+";"
        "   border-radius: 8px;"
        "   color: "+style.at(TEXT)+";"
        "   background-color: "+style.at(TEXTBROWSER_BASE)+";"
        "   selection-color: "+style.at(TEXTBROWSER_TEXT_SELECTION)+";"
        "   selection-background-color: "+style.at(TEXTBROWSER_BASE_SELECTION)+";"
        "}"
        "QTextBrowser QScrollBar:vertical {"
        "   width: 12px;"
        "   background-color: "+style.at(SCROLLBAR_BASE)+";"
        "}"
        "QTextBrowser QScrollBar::handle:vertical {"
        "   min-height: 16px;"
        "   margin: 0px 5px 0px 5px;"
        "   background-color: "+style.at(SCROLLBAR_HANDLER)+";"
        "}"
        "QTextBrowser QScrollBar::handle:vertical:hover {"
        "   margin: 0px 4px 0px 4px;"
        "}"
        "QTextBrowser QScrollBar::add-line,"
        "QTextBrowser QScrollBar::sub-line,"
        "QTextBrowser QScrollBar::add-pae,"
        "QTextBrowser QScrollBar::sub-pae,"
        "QTextBrowser QScrollBar::up-arrow,"
        "QTextBrowser QScrollBar::down-arrow,"
        "QTextBrowser QScrollBar::left-arrow,"
        "QTextBrowser QScrollBar::right-arrow {"
        "   border: 0px;"
        "   background-color: "+style.at(SCROLLBAR_CONTROLS)+";"
        "}";
}


DialogMsg::DialogMsg(const QString& title, const QString& text, const QString& additional, const MsgType type, QWidget* parent )
    : QDialog{ parent }
    , ui{ new Ui::DialogMsg }
{
    ui->setupUi(this);

    if ( GlobalConfigs::window_theme != WindowTheme::Native ) {
        this->setStyleSheet( getStylesheet() );
    }

    // icon
    switch (type) {
        case MsgType::Info:
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialogs/info.png") );
            break;
        case MsgType::Warning:
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialogs/warn.png") );
            break;
        case MsgType::Error:
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialogs/err.png") );
            break;
        default:
            // shouldn't be here
            throw GenericException("Unexpected MsgType: "+ std::to_string(static_cast<int>(type)));
    }

    // insert the given text
    this->ui->label_Title->setText( title );
    this->ui->label_Message->setText( text );

    // additional info, hide by default
    this->ui->frame_Additional->setVisible( false );
    if ( additional.isEmpty() ) {
        this->ui->button_ShowAdditional->setEnabled( false );
        this->ui->button_ShowAdditional->setVisible( false );
    } else {
        this->ui->text_Additional->setText( additional );
        this->ui->button_ShowAdditional->setIcon(
            QIcon(":/icons/icons/"+GlobalConfigs::icons_set+"/list_down.png") );
    }

    // adjust the initial size
    this->adjustSize();
}


void DialogMsg::on_button_ShowAdditional_clicked()
{
    this->additional_shown = ! this->additional_shown;
    // set additional info visibility
    this->ui->frame_Additional->setVisible( this->additional_shown );
    // set the icon
    QIcon icon;
    if ( this->additional_shown ) {
        icon = QIcon(":/icons/icons/"+GlobalConfigs::icons_set+"/list_up.png");
        // resize
        this->initial_height = this->height();
        if ( this->additional_height > 0 ) {
            this->resize( this->width(), this->additional_height );
        } else {
            this->resize( this->width(), this->height()+100 );
        }
    } else {
        icon = QIcon(":/icons/icons/"+GlobalConfigs::icons_set+"/list_down.png");
        this->additional_height = this->height();
        this->resize( this->width(), this->initial_height );
    }
    this->ui->button_ShowAdditional->setIcon( icon );
}


void DialogMsg::on_button_Ok_clicked()
{
    this->done( 1 );
}

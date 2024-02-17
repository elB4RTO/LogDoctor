
#include "ida_dialog.h"
#include "ui_ida_dialog.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <unordered_map> // leave this here for clang


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    WINDOW,
    BUTTONS_BASE,
    BUTTONS_BASE_HOVER
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
                    "rgb( 123, 201, 255 )"}
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
                    "rgb( 33, 162, 33 )"}
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
        "QPushButton {"
        "   border-radius: 8px;"
        "   background-color: "+style.at(BUTTONS_BASE)+";"
        "}"
        "QPushButton:hover {"
        "   background-color: "+style.at(BUTTONS_BASE_HOVER)+";"
        "}";
}

} //namespace (private)


DialogIda::DialogIda( const QString& title, const QString& text, const bool ignore, const bool discard, const bool abort, QWidget* parent )
    : QDialog{ parent }
    , ui{ new Ui::DialogIda }
{
    ui->setupUi(this);

    if ( GlobalConfigs::window_theme != WindowTheme::Native ) {
        this->setStyleSheet( getStylesheet() );
    }

    // (un)set the button if needed
    if ( !ignore ) {
        this->ui->button_IGNORE->setEnabled( false );
        this->ui->button_IGNORE->setVisible( false );
    }
    if ( !discard ) {
        this->ui->button_DISCARD->setEnabled( false );
        this->ui->button_DISCARD->setVisible( false );
    }
    if ( !abort ) {
        this->ui->button_ABORT->setEnabled( false );
        this->ui->button_ABORT->setVisible( false );
    }

    // insert the given title and text
    this->ui->label_Title->setText( title );
    this->ui->label_Message->setText( text );

    // adjust the initial size
    this->adjustSize();
}

void DialogIda::on_button_IGNORE_clicked()
{
    this->done( 2 );
}

void DialogIda::on_button_DISCARD_clicked()
{
    this->done( 1 );
}

void DialogIda::on_button_ABORT_clicked()
{
    this->done( 0 );
}

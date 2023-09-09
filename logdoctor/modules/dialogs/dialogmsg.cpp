
#include "dialogmsg.h"
#include "ui_dialogmsg.h"

#include "modules/exceptions.h"

#include <QSize>
#include <QIcon>
#include <QPixmap>


DialogMsg::DialogMsg(const QString& title, const QString& text, const QString& additional, const MsgType type, QWidget* parent )
    : QDialog{ parent }
    , ui{ new Ui::DialogMsg }
{
    ui->setupUi(this);

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
        case MsgType::Game:
            this->ui->label_Icon->setPixmap( QPixmap(":/logo/logo/logdoctor.svg").scaled( QSize(64,64) ) );
            break;
        default:
            // shouldn't be here
            throw GenericException("Unexpected dialog type: "+ std::to_string(static_cast<int>(type)));
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
        icon = QIcon(":/icons/icons/up.png");
        // resize
        this->initial_height = this->height();
        if ( this->additional_height > 0 ) {
            this->resize( this->width(), this->additional_height );
        } else {
            this->resize( this->width(), this->height()+100 );
        }
    } else {
        icon = QIcon(":/icons/icons/down.png");
        this->additional_height = this->height();
        this->resize( this->width(), this->initial_height );
    }
    this->ui->button_ShowAdditional->setIcon( icon );
}


void DialogMsg::on_button_Ok_clicked()
{
    this->done( 1 );
}

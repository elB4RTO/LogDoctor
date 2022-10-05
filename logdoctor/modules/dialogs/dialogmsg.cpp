
#include "dialogmsg.h"
#include "ui_dialogmsg.h"

#include "modules/exceptions.h"

#include <QSize>
#include <QIcon>
#include <QPixmap>


DialogMsg::DialogMsg(const QString& title, const QString& text, const QString& additional, const int& type, QWidget* parent ) :
    QDialog(parent),
    ui(new Ui::DialogMsg)
{
    ui->setupUi(this);

    // icon
    switch (type) {
        case 0:
            // info message
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialog_info.png") );
            break;
        case 1:
            // warning message
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialog_warn.png") );
            break;
        case 2:
            // error message, change the icon
            this->ui->label_Icon->setPixmap( QPixmap(":/icons/icons/dialog_err.png") );
            break;
        default:
            // shouldn't be here
            throw GenericException("Unexpected dialog type: "+ type);
    }

    // insert the given text
    this->ui->label_Title->setText( title );
    this->ui->label_Message->setText( text );

    // additional info, hide by default
    this->ui->frame_Additional->setVisible( false );
    if ( additional.size() == 0 ) {
        this->ui->button_ShowAdditional->setEnabled( false );
        this->ui->button_ShowAdditional->setVisible( false );
    } else {
        this->ui->text_Additional->setText( additional );
    }

    // adjust the initial size
    this->adjustSize();
}

DialogMsg::~DialogMsg()
{
    delete ui;
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


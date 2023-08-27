
#include "dialogbool.h"
#include "ui_dialogbool.h"


DialogBool::DialogBool( const QString& title, const QString& text, QWidget *parent )
    : QDialog{ parent }
    , ui{ new Ui::DialogBool }
{
    ui->setupUi(this);

    // insert the given title and text
    this->ui->label_Title->setText( title );
    this->ui->label_Message->setText( text );

    // adjust the initial size
    this->adjustSize();
}


void DialogBool::on_button_NO_clicked()
{
    this->done( 0 );
}

void DialogBool::on_button_YES_clicked()
{
    this->done( 1 );
}

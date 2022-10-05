#include "dialogdia.h"
#include "ui_dialogdia.h"

DialogDia::DialogDia( const QString& title, const QString& text, const bool& ignore, const bool& discard, const bool& abort, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::DialogDia)
{
    ui->setupUi(this);

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

DialogDia::~DialogDia()
{
    delete ui;
}

void DialogDia::on_button_IGNORE_clicked()
{
    this->done( 2 );
}


void DialogDia::on_button_DISCARD_clicked()
{
    this->done( 1 );
}


void DialogDia::on_button_ABORT_clicked()
{
    this->done( 0 );
}



#include "game_dialog.h"
#include "ui_game_dialog.h"


GameDialog::GameDialog(const QString& title, const QString& text, QWidget* parent )
    : QDialog{ parent }
    , ui{ new Ui::GameDialog }
{
    ui->setupUi(this);

    // insert the given text
    this->ui->label_Title->setText( title );
    this->ui->label_Message->setText( text );

    // adjust the initial size
    this->adjustSize();
}

void GameDialog::on_button_Ok_clicked()
{
    this->done( 1 );
}

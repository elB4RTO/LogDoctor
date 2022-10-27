
#include "crapinfo.h"
#include "ui_crapinfo.h"

#include <QFontDatabase>


Crapinfo::Crapinfo( const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::Crapinfo)
{
    ui->setupUi(this);

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


#include "crapinfo.h"
#include "ui_crapinfo.h"

#include "modules/stylesheets.h"

#include <QFontDatabase>


Crapinfo::Crapinfo( const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent )
    : QWidget{ parent }
    , ui{ new Ui::Crapinfo }
{
    ui->setupUi(this);

    QString stylesheet;
    StyleSec::Crapinfo::getStyleSheet( stylesheet );
    this->setStyleSheet( stylesheet );

    // fonts
    const QString& ff{ QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0) };
    const QFont font{ ff, 13 };
    const QFont font_big{ ff, 15 };
    const QFont font_bigger{ ff, 17 };
    const QString& ff_s{ QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Hack")).at(0) };
    const QFont font_script{ ff_s, 12 };
    const QFont font_script_small{ ff_s, 10 };

    // apply the fonts
    this->ui->toolBox_Infos->setFont( font );
    this->ui->label_Version->setFont( font_bigger );
    this->ui->label_Links->setFont( font_big );
    this->ui->inLine_Links_Github->setFont( font_script_small );
    this->ui->inLine_Links_Disroot->setFont( font_script_small );
    this->ui->inLine_Links_Gitlab->setFont( font_script_small );
    this->ui->inLine_Links_Bitbucket->setFont( font_script_small );
    this->ui->frame_LogDoc_Paths->setFont( font );
    this->ui->inLine_ExecPath->setFont( font_script );
    this->ui->inLine_ConfPath->setFont( font_script );
    this->ui->inLine_DataPath->setFont( font_script );
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

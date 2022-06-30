
#include "dialogs.h"
#include "mainwindow.h"


Dialogs::Dialogs()
{

}


bool Dialogs::choiceSelectedFileNotFound(QWidget *parent, const std::string& file )
{
    return choiceSelectedFileNotFound( parent, QString::fromStdString( file ) );
}
bool Dialogs::choiceSelectedFileNotFound(QWidget *parent, const QString& file )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        t_FILE_NOT_FOUND,
        QString("%1:\n%2\n\n%3?")
            .arg( m_SELECTED_FILE_NOT_FOUND, file, q_CONTINUE ),
        QMessageBox::Abort | QMessageBox::Ignore );
    if ( response == QMessageBox::Ignore ) {
        proceed = true;
    }
    return proceed;
}



void Dialogs::msgDirNotExists(QWidget *parent, const std::string& dir )
{
    msgDirNotExists( parent, QString::fromStdString( dir ) );
}
void Dialogs::msgDirNotExists(QWidget *parent, const QString& dir )
{
    QMessageBox::warning(parent,
        t_DIR_NOT_FOUND,
        dir,
        QMessageBox::Ok );
}




void Dialogs::msgGenericError(QWidget *parent, const std::string& msg )
{
    msgGenericError( parent, QString::fromStdString( msg ) );
}
void Dialogs::msgGenericError(QWidget *parent, const QString& msg )
{
    QMessageBox::critical(parent,
        t_ERROR_OCCURED,
        QString("%1\n\n%2")
            .arg( msg, r_REPORT_ISSUE ),
        QMessageBox::Ok );
}

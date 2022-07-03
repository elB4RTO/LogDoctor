
#include "dialogs.h"
#include "mainwindow.h"


DialogSec::DialogSec()
{

}


void DialogSec::errFailedDefineLogType( QWidget *parent, const std::string& file )
{
    errFailedDefineLogType( parent, QString::fromStdString(file) );
}
void DialogSec::errFailedDefineLogType( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGTYPE_FAILED,
        QString("%1:\n%2")
            .arg( m_LOGTYPE_FAILED, file ),
        QMessageBox::Ok );
}


void DialogSec::errUndefinedLogType( QWidget *parent, const std::string& file )
{
    errUndefinedLogType( parent, QString::fromStdString(file) );
}
void DialogSec::errUndefinedLogType( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_ERROR_OCCURED,
        QString("%1:\n%2\n\n%3")
            .arg( m_LOGTYPE_UNDEFINED, file, f_SKIPPING ),
        QMessageBox::Ok );
}

bool DialogSec::choiceUndefinedLogType( QWidget *parent, const std::string& file )
{
    return choiceUndefinedLogType( parent, QString::fromStdString(file) );
}
bool DialogSec::choiceUndefinedLogType( QWidget *parent, const QString& file )
{
    int choice;
    auto response = QMessageBox::critical(parent,
        t_ERROR_OCCURED,
        QString("%1:\n%2\n\n%3")
            .arg( m_LOGTYPE_UNDEFINED, file, q_DA ),
        QMessageBox::Discard | QMessageBox::Abort );
    if ( response == QMessageBox::Discard ) {
        choice = true;
    } else {
        choice = false;
    }
    return choice;
}


int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const std::string& file )
{
    return choiceFileAlreadyUsed( parent, QString::fromStdString(file) );
}
int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const QString& file )
{
    int choice;
    auto response = QMessageBox::warning(parent,
        t_FILE_ALREADY_USED,
        QString("%1:\n%2\n\n%3?")
            .arg( m_FILE_ALREADY_USED, file, q_DIA ),
        QMessageBox::Discard | QMessageBox::Ignore | QMessageBox::Abort );
    if ( response == QMessageBox::Ignore ) {
        choice = 0;
    } else if ( response == QMessageBox::Discard ) {
        choice = 1;
    } else {
        choice = -1;
    }
    return choice;
}


void DialogSec::errFailedReadFile(QWidget *parent, const std::string& file , const bool skipping)
{
    errFailedReadFile( parent, QString::fromStdString( file ), skipping );
}
void DialogSec::errFailedReadFile(QWidget *parent, const QString& file , const bool skipping )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_FAILED_READ,
        QString("%1:\n%2")
            .arg( m_FILE_FAILED_READ, file + ((skipping) ? f_SKIPPING : "") ),
        QMessageBox::Ok );
}


void DialogSec::warnEmptyFile( QWidget *parent, const std::string& file )
{
    warnEmptyFile( parent, QString::fromStdString( file ) );
}

void DialogSec::warnEmptyFile( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_FILE_EMPTY,
        QString("%1:\n%2\n\n%3")
            .arg( m_FILE_EMPTY, file, f_SKIPPING ),
        QMessageBox::Ok );
}



bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const std::string& file )
{
    return choiceSelectedFileNotFound( parent, QString::fromStdString( file ) );
}
bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const QString& file )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        t_FILE_NOT_FOUND,
        QString("%1:\n%2\n\n%3?")
            .arg( m_SELECTED_FILE_NOT_FOUND, file, q_DA ),
        QMessageBox::Abort | QMessageBox::Discard );
    if ( response == QMessageBox::Discard ) {
        proceed = true;
    }
    return proceed;
}



void DialogSec::errDirNotExists(QWidget *parent, const std::string& dir )
{
    errDirNotExists( parent, QString::fromStdString( dir ) );
}
void DialogSec::errDirNotExists(QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_FOUND,
        QString("%1:\n%2")
            .arg( m_DIR_NOT_EXISTS, dir ),
        QMessageBox::Ok );
}


bool DialogSec::choiceDirNotExists(QWidget *parent, const std::string& dir )
{
    return choiceDirNotExists( parent, QString::fromStdString( dir ) );
}
bool DialogSec::choiceDirNotExists(QWidget *parent, const QString& dir )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        t_DIR_NOT_FOUND,
        QString("%1:\n%2\n\n%3?")
            .arg( m_DIR_NOT_EXISTS, dir, q_CONTINUE ),
        QMessageBox::Abort | QMessageBox::Ignore );
    if ( response == QMessageBox::Ignore ) {
        proceed = true;
    }
    return proceed;
}




void DialogSec::warnGeneric(QWidget *parent, const std::string& msg, const bool report_msg )
{
    warnGeneric( parent, QString::fromStdString( msg ), report_msg );
}
void DialogSec::warnGeneric(QWidget *parent, const QString& msg, const bool report_msg )
{
    QString footer = "";
    if ( report_msg == true ) {
        footer += "\n\n" + r_REPORT_ISSUE;
    }
    std::ignore=
    QMessageBox::warning(parent,
        t_ERROR_OCCURED,
        QString("%1%2")
            .arg( msg, footer ),
        QMessageBox::Ok );
}



void DialogSec::errGeneric(QWidget *parent, const std::string& msg, const bool report_msg )
{
    errGeneric( parent, QString::fromStdString( msg ), report_msg );
}
void DialogSec::errGeneric(QWidget *parent, const QString& msg, const bool report_msg )
{
    QString footer = "";
    if ( report_msg == true ) {
        footer += "\n\n" + r_REPORT_ISSUE;
    }
    std::ignore=
    QMessageBox::critical(parent,
        t_ERROR_OCCURED,
        QString("%1%2")
            .arg( msg, footer ),
        QMessageBox::Ok );
}

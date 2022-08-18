
#include "dialogs.h"
#include "mainwindow.h"


DialogSec::DialogSec()
{

}



//////////////////
//// LANGUAGE ////
//////////////////
void DialogSec::errLangLocaleInvalid( QWidget *parent, const QString& locale )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LANG_INVALID,
        QString("%1:\n%2\n\n%3")
            .arg( m_LANG_LOCALE_INVALID, locale, r_CONF_DONT_EDIT ),
        QMessageBox::Ok );
}
void DialogSec::errLangNotAccepted( QWidget *parent, const QString& locale )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LANG_INVALID,
        QString("%1:\n%2\n\n%3")
            .arg( m_LANG_NOT_ACCEPTED, locale, r_LANG_NOT_ACCEPTED ),
        QMessageBox::Ok );
}



///////////////////////
//// CONFIGURATION ////
///////////////////////
void DialogSec::warnConfFileNotFound( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_CONF_FILE_NOT_FOUND,
        QString("%1%2\n\n%3")
            .arg( m_CONF_FILE_NOT_FOUND, (file=="") ? file : ":\n"+file, f_SKIPPING ),
        QMessageBox::Ok );
}

void DialogSec::errConfFailedWriting( QWidget *parent, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_CONF_FILE_FAILED_W,
        QString("%1%2\n\n%3")
            .arg( m_CONF_FILE_ERROR, (msg=="") ? msg : ":\n"+msg, m_CONF_FILE_FAILED_W ),
        QMessageBox::Ok );
}

void DialogSec::errConfFileNotReadable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_READABLE,
        QString("%1\n\n%2%3")
            .arg( m_CONF_FILE_ERROR, m_FILE_NOT_READABLE, (file=="") ? file : ":\n"+file ),
        QMessageBox::Ok );
}
void DialogSec::errConfFileNotWritable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_WRITABLE,
        QString("%1\n\n%2%3")
            .arg( m_CONF_FILE_ERROR, m_FILE_NOT_WRITABLE, (file=="") ? file : ":\n"+file ),
        QMessageBox::Ok );
}


void DialogSec::errConfDirNotWritable( QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_WRITABLE,
        QString("%1\n\n%2%3")
            .arg( m_CONF_FILE_ERROR, m_DIR_NOT_WRITABLE, (dir=="") ? dir : ":\n"+dir ),
        QMessageBox::Ok );
}



//////////////////
//// LANGUAGE ////
//////////////////
void DialogSec::errVersionCheckFailed( QWidget *parent, const int& err_code )
{
    QString msg;
    switch (err_code) {
        case 1:
            msg = QString("%1\n\n%2").arg( m_CONNECTIONS_FAILED, r_UPSTREAM_FAILURE );
            break;
        case 10:
            msg = m_VERSION_NOT_FOUND;
            break;
        case 11:
            msg = m_VERSION_MALFORMED;
            break;
    }

    std::ignore=
    QMessageBox::critical(parent,
        t_VERSION_CHECK_FAILED,
        QString("%1\n\n%2")
            .arg( m_VERSION_CHECK_FAILED, msg ),
        QMessageBox::Ok );
}

void DialogSec::warnConnectionFailed( QWidget *parent, const QString& link, const QString& err_msg )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_CONNECTION_FAILED,
        QString("%1:\n%2\n\n%3")
            .arg( m_CONNECTION_FAILED, link, err_msg ),
        QMessageBox::Ok );
}

void DialogSec::msgNewVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::information(parent,
        t_VERSION_CHECK,
        m_NEW_VERSION_AVAILABLE,
        QMessageBox::Ok );
}
void DialogSec::msgSameVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::information(parent,
        t_VERSION_CHECK,
        m_SAME_VERSION,
        QMessageBox::Ok );
}
void DialogSec::warnFutureVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_VERSION_CHECK,
        m_FUTURE_VERSION_RUNNING,
        QMessageBox::Ok );
}



//////////////
//// HELP ////
//////////////
void DialogSec::errHelpFailed( QWidget *parent, const QString& link, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_HELP_FAILED_LOADING,
        QString("%1%2\n\n%3%4")
            .arg( m_HELP_FAILED_LOADING,
                  (msg=="") ? msg : ":\n"+msg,
                  f_ADD_RESOURCES,
                  (link=="") ? link : ":\n"+link ),
        QMessageBox::Ok );
}

void DialogSec::errHelpNotFound( QWidget *parent, const QString& link )
{
    DialogSec::errHelpFailed( parent, link, m_FILE_NOT_FOUND.toLower() );
}

void DialogSec::errHelpNotReadable( QWidget *parent, const QString& link )
{
    DialogSec::errHelpFailed( parent, link, m_FILE_NOT_READABLE.toLower() );
}




//////////////////
//// DATABASE ////
//////////////////
void DialogSec::errSqlDriverNotFound( QWidget *parent, const QString& driver )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DB_DRIVER_NOT_FOUND,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_DRIVER_NOT_FOUND, driver, f_ABORTING ),
        QMessageBox::Ok );
}

const bool DialogSec::choiceDatabaseNotFound( QWidget *parent, const QString& db_name )
{
    bool choice = false;
    auto response = QMessageBox::question(parent,
        t_FILE_NOT_FOUND,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_NOT_FOUND, db_name, q_DB_CREATE ),
        QMessageBox::Yes | QMessageBox::No );
    if ( response == QMessageBox::Yes ) {
        choice = true;
    }
    return choice;
}

const bool DialogSec::choiceDatabaseWrongTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg( m_DB_WRONG_TABLE_NAME, db_name, table_name );
    return choiceDatabaseRenew( parent, t_DB_WRONG_TABLE_NAME, msg );
}
const bool DialogSec::choiceDatabaseMissingTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg( m_DB_MISSING_TABLE, db_name, table_name );
    return choiceDatabaseRenew( parent, t_DB_MISSING_TABLE, msg );
}
const bool DialogSec::choiceDatabaseWrongColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg( m_DB_WRONG_COLUMN_NAME, db_name, table_name, column_name );
    return choiceDatabaseRenew( parent, t_DB_WRONG_COLUMN_NAME, msg );
}
const bool DialogSec::choiceDatabaseMissingColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg( m_DB_MISSING_COLUMN, db_name, table_name, column_name );
    return choiceDatabaseRenew( parent, t_DB_MISSING_COLUMN, msg );
}
const bool DialogSec::choiceDatabaseWrongDataType(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4 -> %5").arg( m_DB_WRONG_COLUMN_TYPE, db_name, table_name, column_name, data_type );
    return choiceDatabaseRenew( parent, t_DB_WRONG_COLUMN_TYPE, msg );
}
const bool DialogSec::choiceDatabaseRenew(QWidget *parent, const QString& title, const QString& msg )
{
    bool choice = false;
    auto response = QMessageBox::warning(parent,
        title,
        QString("%1\n\n%2")
            .arg( msg, q_DB_RENEW ),
        QMessageBox::Yes | QMessageBox::No );
    if ( response == QMessageBox::Yes ) {
        choice = true;
    }
    return choice;
}
void DialogSec::msgDatabaseCreated( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::information(parent,
        t_DB_CREATED,
        QString("%1:\n%2")
            .arg( m_DB_CREATED, db_name ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotFile( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_FILE,
        QString("%1\n\n%2:\n%3\n\n%4")
            .arg( m_DB_NOT_FOUND, m_FILE_NOT_FILE, db_name, r_REMOVE_ENTRY ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotReadable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_READABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_NOT_READABLE, db_name, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotWritable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_WRITABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_NOT_WRITABLE, db_name, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedCreating(QWidget *parent, const QString& db_name, const QString &err_msg )
{
    QString msg = db_name;
    if ( err_msg != "" ) {
        msg += "\n\n" + m_DB_FAILED_ERRMSG + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        t_DB_FAILED_CREATING,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_FAILED_CREATING, msg, f_ABORTING ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedOpening(QWidget *parent, const QString& db_name, const QString &err_msg )
{
    QString msg = db_name;
    if ( err_msg != "" ) {
        msg += "\n\n" + m_DB_FAILED_ERRMSG + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        t_DB_FAILED_OPENING,
        QString("%1:\n%2\n\n%e")
            .arg( m_DB_FAILED_OPENING, msg, f_ABORTING ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedExecuting(QWidget *parent, const QString& db_name, const QString& statement, const QString& err_msg )
{
    QString msg = db_name;
    if ( statement != "" ) {
        msg += "\n\n" + m_DB_FAILED_STATEMENT + ":\n" + statement;
    }
    if ( err_msg != "" ) {
        msg += "\n\n" + m_DB_FAILED_ERRMSG + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        t_DB_FAILED_EXECUTING,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_FAILED_EXECUTING, msg, f_ABORTING ),
        QMessageBox::Ok );
}



/////////////////////
//// LOGS FORMAT ////
/////////////////////
void DialogSec::errInvalidLogFormatString( QWidget *parent, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGFORMAT_INVALID,
        QString("%1\n\n%2")
            .arg( msg, r_CHECK_FORMAT ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNotSet( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGFORMAT_ERROR,
        QString("%1")
            .arg( m_LOGFORMAT_NOT_SET ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNoFields( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGFORMAT_MISCONFIGURED,
        QString("%1\n\n%2")
            .arg( m_LOGFORMAT_NO_FIELDS, r_SET_FORMAT_FIELDS ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNoSeparators( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGFORMAT_MISCONFIGURED,
        QString("%1\n\n%2")
            .arg( m_LOGFORMAT_NO_SEPS, r_SET_FORMAT_SEPS ),
        QMessageBox::Ok );
}



///////////////////
//// LOG FILES ////
///////////////////
void DialogSec::errFailedDefiningLogType( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGTYPE_FAILED,
        QString("%1:\n%2")
            .arg( m_LOGTYPE_FAILED, file ),
        QMessageBox::Ok );
}


const int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const QString& msg )
{
    int choice;
    auto response = QMessageBox::warning(parent,
        t_FILE_ALREADY_USED,
        QString("%1:\n%2\n\n%3")
            .arg( m_FILE_ALREADY_USED, msg, q_DIA ),
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


const int DialogSec::choiceFileSizeWarning( QWidget *parent, const QString& msg )
{
    int choice;
    auto response = QMessageBox::warning(parent,
        t_FILE_SIZE_WARNING,
        QString("%1:\n%2\n\n%3")
            .arg( m_FILE_SIZE_WARNING, msg, q_DIA ),
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


void DialogSec::errFailedReadFile( QWidget *parent, const QString& file , const bool& skipping )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_FAILED_READ,
        QString("%1:\n%2")
            .arg( m_FILE_FAILED_READ, file + ((skipping) ? f_SKIPPING : "") ),
        QMessageBox::Ok );
}

void DialogSec::errFailedWriteFile( QWidget *parent, const QString& file , const bool& skipping )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_FAILED_WRITE,
        QString("%1:\n%2")
            .arg( m_FILE_FAILED_WRITE, file + ((skipping) ? f_SKIPPING : "") ),
        QMessageBox::Ok );
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

void DialogSec::warnFileNotReadable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_FILE_NOT_READABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_FILE_NOT_READABLE, file, f_SKIPPING ),
        QMessageBox::Ok );
}



const bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const QString& file )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        t_FILE_NOT_FOUND,
        QString("%1:\n%2\n\n%3")
            .arg( m_SELECTED_FILE_NOT_FOUND, file, q_DA ),
        QMessageBox::Abort | QMessageBox::Discard );
    if ( response == QMessageBox::Discard ) {
        proceed = true;
    }
    return proceed;
}



/////////////////////
//// PERMISSIONS ////
/////////////////////
void DialogSec::errDirNotExists(QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_FOUND,
        QString("%1:\n%2")
            .arg( m_DIR_NOT_EXISTS, dir ),
        QMessageBox::Ok );
}

const bool DialogSec::choiceDirNotExists(QWidget *parent, const QString& dir )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        t_DIR_NOT_FOUND,
        QString("%1:\n%2\n\n%3")
            .arg( m_DIR_NOT_EXISTS, dir, q_CONTINUE ),
        QMessageBox::Abort | QMessageBox::Ignore );
    if ( response == QMessageBox::Ignore ) {
        proceed = true;
    }
    return proceed;
}


void DialogSec::errDirNotReadable( QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_READABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DIR_NOT_READABLE, dir, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}
void DialogSec::warnDirNotReadable( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_DIR_NOT_READABLE,
        QString("%1\n\n%2")
            .arg( m_DIR_NOT_READABLE, r_SET_PERMS_BEFORE ),
        QMessageBox::Ok );
}

void DialogSec::errDirNotWritable( QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_WRITABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DIR_NOT_WRITABLE, dir, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}
void DialogSec::warnDirNotWritable( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        t_DIR_NOT_WRITABLE,
        QString("%1\n\n%2")
            .arg( m_DIR_NOT_WRITABLE, r_SET_PERMS_BEFORE ),
        QMessageBox::Ok );
}


//////////////////
//// GENERICS ////
//////////////////
const bool DialogSec::choiceDirNotDir( QWidget *parent, const QString& path )
{
    bool choice = false;
    auto response = QMessageBox::question(parent,
        t_DIR_NOT_DIR,
        QString("%1:\n%2\n\n%3")
            .arg( m_DIR_NOT_DIR, path, q_ENTRY_RENEW ),
        QMessageBox::Yes | QMessageBox::No );
    if ( response == QMessageBox::Yes ) {
        choice = true;
    }
    return choice;
}
const bool DialogSec::choiceFileNotFile( QWidget *parent, const QString& path )
{
    bool choice = false;
    auto response = QMessageBox::question(parent,
        t_FILE_NOT_FILE,
        QString("%1:\n%2\n\n%3")
            .arg( m_FILE_NOT_FILE, path, q_ENTRY_RENEW ),
        QMessageBox::Yes | QMessageBox::No );
    if ( response == QMessageBox::Yes ) {
        choice = true;
    }
    return choice;
}



void DialogSec::warnGeneric( QWidget *parent, const QString& msg, const bool& report_msg )
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



void DialogSec::errGeneric( QWidget *parent, const QString& msg, const bool& report_msg )
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



void DialogSec::errRenaming( QWidget *parent, const QString& path )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_ERROR_RENAMING,
        QString("%1:\n%2\n\n%3")
            .arg( m_ERROR_RENAMING, path, f_ABORTING ),
        QMessageBox::Ok );
}

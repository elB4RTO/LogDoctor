
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
        DialogSec::tr( t_LANG_INVALID.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_LANG_LOCALE_INVALID.c_str() ),
            locale,
            DialogSec::tr( r_CONF_DONT_EDIT.c_str() ) ),
        QMessageBox::Ok );
}
void DialogSec::errLangNotAccepted( QWidget *parent, const QString& locale )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LANG_INVALID.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_LANG_NOT_ACCEPTED.c_str() ),
            locale,
            DialogSec::tr( r_LANG_NOT_ACCEPTED.c_str() ) ),
        QMessageBox::Ok );
}



///////////////////////
//// CONFIGURATION ////
///////////////////////
void DialogSec::warnConfFileNotFound( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_CONF_FILE_NOT_FOUND.c_str() ),
        QString("%1%2\n\n%3").arg(
            DialogSec::tr( m_CONF_FILE_NOT_FOUND.c_str() ),
            (file=="") ? file : ":\n"+file,DialogSec::tr( f_SKIPPING.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errConfFailedWriting( QWidget *parent, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_CONF_FILE_FAILED_W.c_str() ),
        QString("%1%2\n\n%3").arg(
            DialogSec::tr( m_CONF_FILE_ERROR.c_str() ),
            (msg=="") ? msg : ":\n"+msg,
            DialogSec::tr( m_CONF_FILE_FAILED_W.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errConfFileNotReadable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_NOT_READABLE.c_str() ),
        QString("%1\n\n%2%3").arg(
            DialogSec::tr( m_CONF_FILE_ERROR.c_str() ),
            DialogSec::tr( m_FILE_NOT_READABLE.c_str() ),
            (file=="") ? file : ":\n"+file ),
        QMessageBox::Ok );
}
void DialogSec::errConfFileNotWritable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_NOT_WRITABLE.c_str() ),
        QString("%1\n\n%2%3").arg(
            DialogSec::tr( m_CONF_FILE_ERROR.c_str() ),
            DialogSec::tr( m_FILE_NOT_WRITABLE.c_str() ),
            (file=="") ? file : ":\n"+file ),
        QMessageBox::Ok );
}


void DialogSec::errConfDirNotWritable( QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DIR_NOT_WRITABLE.c_str() ),
        QString("%1\n\n%2%3").arg(
            DialogSec::tr( m_CONF_FILE_ERROR.c_str() ),
            DialogSec::tr( m_DIR_NOT_WRITABLE.c_str() ),
            (dir=="") ? dir : ":\n"+dir ),
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
            msg = QString("%1\n\n%2").arg(
                DialogSec::tr( m_CONNECTIONS_FAILED.c_str() ),
                DialogSec::tr( r_UPSTREAM_FAILURE.c_str() ) );
            break;
        case 10:
            msg = DialogSec::tr( m_VERSION_NOT_FOUND.c_str() );
            break;
        case 11:
            msg = DialogSec::tr( m_VERSION_MALFORMED.c_str() );
            break;
    }

    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_VERSION_CHECK_FAILED.c_str() ),
        QString("%1\n\n%2").arg(
            DialogSec::tr( m_VERSION_CHECK_FAILED.c_str() ),
            msg ),
        QMessageBox::Ok );
}

void DialogSec::warnConnectionFailed( QWidget *parent, const QString& link, const QString& err_msg )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_CONNECTION_FAILED.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_CONNECTION_FAILED.c_str() ),
            link,
            err_msg ),
        QMessageBox::Ok );
}

void DialogSec::msgNewVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::information(parent,
        DialogSec::tr( t_VERSION_CHECK.c_str() ),
        DialogSec::tr( m_NEW_VERSION_AVAILABLE.c_str() ),
        QMessageBox::Ok );
}
void DialogSec::msgSameVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::information(parent,
        DialogSec::tr( t_VERSION_CHECK.c_str() ),
        DialogSec::tr( m_SAME_VERSION.c_str() ),
        QMessageBox::Ok );
}
void DialogSec::warnFutureVersion( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_VERSION_CHECK.c_str() ),
        DialogSec::tr( m_FUTURE_VERSION_RUNNING.c_str() ),
        QMessageBox::Ok );
}



//////////////
//// HELP ////
//////////////
void DialogSec::errHelpFailed( QWidget *parent, const QString& link, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_HELP_FAILED_LOADING.c_str() ),
        QString("%1%2\n\n%3%4").arg(
            DialogSec::tr( m_HELP_FAILED_LOADING.c_str() ),
            (msg=="") ? msg : ":\n"+msg,
            DialogSec::tr( f_ADD_RESOURCES.c_str() ),
            (link=="") ? link : ":\n"+link ),
        QMessageBox::Ok );
}

void DialogSec::errHelpNotFound( QWidget *parent, const QString& link )
{
    DialogSec::errHelpFailed( parent, link,
            DialogSec::tr( m_FILE_NOT_FOUND.c_str() ).toLower() );
}

void DialogSec::errHelpNotReadable( QWidget *parent, const QString& link )
{
    DialogSec::errHelpFailed( parent, link,
            DialogSec::tr( m_FILE_NOT_READABLE.c_str() ).toLower() );
}




//////////////////
//// DATABASE ////
//////////////////
void DialogSec::errSqlDriverNotFound( QWidget *parent, const QString& driver )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DB_DRIVER_NOT_FOUND.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_DRIVER_NOT_FOUND.c_str() ),
            driver,
            DialogSec::tr( f_ABORTING.c_str() ) ),
        QMessageBox::Ok );
}

const bool DialogSec::choiceDatabaseNotFound( QWidget *parent, const QString& db_name )
{
    bool choice = false;
    auto response = QMessageBox::question(parent,
        DialogSec::tr( t_FILE_NOT_FOUND.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_NOT_FOUND.c_str() ),
            db_name,
           DialogSec::tr( q_DB_CREATE.c_str() ) ),
        QMessageBox::Yes | QMessageBox::No );
    if ( response == QMessageBox::Yes ) {
        choice = true;
    }
    return choice;
}

const bool DialogSec::choiceDatabaseWrongTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg(
        DialogSec::tr( m_DB_WRONG_TABLE_NAME.c_str() ),
        db_name, table_name );
    return choiceDatabaseRenew( parent,
            DialogSec::tr( t_DB_WRONG_TABLE_NAME.c_str() ), msg );
}
const bool DialogSec::choiceDatabaseMissingTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg(
        DialogSec::tr( m_DB_MISSING_TABLE.c_str() ),
        db_name, table_name );
    return choiceDatabaseRenew( parent,
            DialogSec::tr( t_DB_MISSING_TABLE.c_str() ), msg );
}
const bool DialogSec::choiceDatabaseWrongColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg(
        DialogSec::tr( m_DB_WRONG_COLUMN_NAME.c_str() ),
        db_name, table_name, column_name );
    return choiceDatabaseRenew( parent,
            DialogSec::tr( t_DB_WRONG_COLUMN_NAME.c_str() ), msg );
}
const bool DialogSec::choiceDatabaseMissingColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg(
        DialogSec::tr( m_DB_MISSING_COLUMN.c_str() ),
        db_name, table_name, column_name );
    return choiceDatabaseRenew( parent,
            DialogSec::tr( t_DB_MISSING_COLUMN.c_str() ), msg );
}
const bool DialogSec::choiceDatabaseWrongDataType(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4 -> %5").arg(
        DialogSec::tr( m_DB_WRONG_COLUMN_TYPE.c_str() ),
        db_name, table_name, column_name, data_type );
    return choiceDatabaseRenew( parent,
            DialogSec::tr( t_DB_WRONG_COLUMN_TYPE.c_str() ), msg );
}
const bool DialogSec::choiceDatabaseRenew(QWidget *parent, const QString& title, const QString& msg )
{
    bool choice = false;
    auto response = QMessageBox::warning(parent,
        title,
        QString("%1\n\n%2").arg(
            msg,
            DialogSec::tr( q_DB_RENEW.c_str() ) ),
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
        DialogSec::tr( t_DB_CREATED.c_str() ),
        QString("%1:\n%2").arg(
            DialogSec::tr( m_DB_CREATED.c_str() ),
            db_name ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotFile( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_NOT_FILE.c_str() ),
        QString("%1\n\n%2:\n%3\n\n%4").arg(
            DialogSec::tr( m_DB_NOT_FOUND.c_str() ),
            DialogSec::tr( m_FILE_NOT_FILE.c_str() ),
            db_name,
            DialogSec::tr( r_REMOVE_ENTRY.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotReadable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_NOT_READABLE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_NOT_READABLE.c_str() ),
            db_name,
            DialogSec::tr( r_SET_PERMISSIONS.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseNotWritable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_NOT_WRITABLE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_NOT_WRITABLE.c_str() ),
            db_name,
            DialogSec::tr( r_SET_PERMISSIONS.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedCreating(QWidget *parent, const QString& db_name, const QString &err_msg )
{
    QString msg = db_name;
    if ( err_msg != "" ) {
        msg += "\n\n" + DialogSec::tr( m_DB_FAILED_ERRMSG.c_str() ) + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DB_FAILED_CREATING.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_FAILED_CREATING.c_str() ),
            msg,
            DialogSec::tr( f_ABORTING.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedOpening(QWidget *parent, const QString& db_name, const QString &err_msg )
{
    QString msg = db_name;
    if ( err_msg != "" ) {
        msg += "\n\n" + DialogSec::tr( m_DB_FAILED_ERRMSG.c_str() ) + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DB_FAILED_OPENING.c_str() ),
        QString("%1:\n%2\n\n%e").arg(
            DialogSec::tr( m_DB_FAILED_OPENING.c_str() ),
            msg,
            DialogSec::tr( f_ABORTING.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDatabaseFailedExecuting(QWidget *parent, const QString& db_name, const QString& statement, const QString& err_msg )
{
    QString msg = db_name;
    if ( statement != "" ) {
        msg += "\n\n" + DialogSec::tr( m_DB_FAILED_STATEMENT.c_str() ) + ":\n" + statement;
    }
    if ( err_msg != "" ) {
        msg += "\n\n" + DialogSec::tr( m_DB_FAILED_ERRMSG.c_str() ) + ":\n" + err_msg;
    }
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DB_FAILED_EXECUTING.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DB_FAILED_EXECUTING.c_str() ),
            msg,
            DialogSec::tr( f_ABORTING.c_str() ) ),
        QMessageBox::Ok );
}



/////////////////////
//// LOGS FORMAT ////
/////////////////////
void DialogSec::errInvalidLogFormatString( QWidget *parent, const QString& msg )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LOGFORMAT_INVALID.c_str() ),
        QString("%1\n\n%2").arg(
            msg,
            DialogSec::tr( r_CHECK_FORMAT.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNotSet( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LOGFORMAT_ERROR.c_str() ),
        QString("%1").arg(
            DialogSec::tr( m_LOGFORMAT_NOT_SET.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNoFields( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LOGFORMAT_MISCONFIGURED.c_str() ),
        QString("%1\n\n%2").arg(
            DialogSec::tr( m_LOGFORMAT_NO_FIELDS.c_str() ),
            DialogSec::tr( r_SET_FORMAT_FIELDS.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errLogFormatNoSeparators( QWidget *parent )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LOGFORMAT_MISCONFIGURED.c_str() ),
        QString("%1\n\n%2").arg(
            DialogSec::tr( m_LOGFORMAT_NO_SEPS.c_str() ),
            DialogSec::tr( r_SET_FORMAT_SEPS.c_str() ) ),
        QMessageBox::Ok );
}



///////////////////
//// LOG FILES ////
///////////////////
void DialogSec::errFailedDefiningLogType( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_LOGTYPE_FAILED.c_str() ),
        QString("%1:\n%2").arg(
            DialogSec::tr( m_LOGTYPE_FAILED.c_str() ),
            file ),
        QMessageBox::Ok );
}


const int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const QString& msg )
{
    int choice;
    auto response = QMessageBox::warning(parent,
        DialogSec::tr( t_FILE_ALREADY_USED.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_FILE_ALREADY_USED.c_str() ),
            msg,
            DialogSec::tr( q_DIA.c_str() ) ),
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
        DialogSec::tr( t_FILE_SIZE_WARNING.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_FILE_SIZE_WARNING.c_str() ),
            msg,
            DialogSec::tr( q_DIA.c_str() ) ),
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
        DialogSec::tr( t_FILE_FAILED_READ.c_str() ),
        QString("%1:\n%2").arg(
            DialogSec::tr( m_FILE_FAILED_READ.c_str() ),
            file + ((skipping) ? DialogSec::tr( f_SKIPPING.c_str() ) : "") ),
        QMessageBox::Ok );
}

void DialogSec::errFailedWriteFile( QWidget *parent, const QString& file , const bool& skipping )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_FILE_FAILED_WRITE.c_str() ),
        QString("%1:\n%2").arg(
            DialogSec::tr( m_FILE_FAILED_WRITE.c_str() ),
            file + ((skipping) ? DialogSec::tr( f_SKIPPING.c_str() ) : "") ),
        QMessageBox::Ok );
}


void DialogSec::warnEmptyFile( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_FILE_EMPTY.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_FILE_EMPTY.c_str() ),
            file,
            DialogSec::tr( f_SKIPPING.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::warnFileNotReadable( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_FILE_NOT_READABLE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_FILE_NOT_READABLE.c_str() ),
            file,
            DialogSec::tr( f_SKIPPING.c_str() ) ),
        QMessageBox::Ok );
}



const bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const QString& file )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        DialogSec::tr( t_FILE_NOT_FOUND.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_SELECTED_FILE_NOT_FOUND.c_str() ),
            file,
            DialogSec::tr( q_DA.c_str() ) ),
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
        DialogSec::tr( t_DIR_NOT_FOUND.c_str() ),
        QString("%1:\n%2").arg(
            DialogSec::tr( m_DIR_NOT_EXISTS.c_str() ),
            dir ),
        QMessageBox::Ok );
}

const bool DialogSec::choiceDirNotExists(QWidget *parent, const QString& dir )
{
    bool proceed = false;
    auto response = QMessageBox::warning(parent,
        DialogSec::tr( t_DIR_NOT_FOUND.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DIR_NOT_EXISTS.c_str() ),
            dir,
            DialogSec::tr( q_CONTINUE.c_str() ) ),
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
        DialogSec::tr( t_DIR_NOT_READABLE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DIR_NOT_READABLE.c_str() ),
            dir,
            DialogSec::tr( r_SET_PERMISSIONS.c_str() ) ),
        QMessageBox::Ok );
}
void DialogSec::warnDirNotReadable( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_DIR_NOT_READABLE.c_str() ),
        QString("%1\n\n%2").arg(
            DialogSec::tr( m_DIR_NOT_READABLE.c_str() ),
            DialogSec::tr( r_SET_PERMS_BEFORE.c_str() ) ),
        QMessageBox::Ok );
}

void DialogSec::errDirNotWritable( QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_DIR_NOT_WRITABLE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DIR_NOT_WRITABLE.c_str() ),
            dir,
            DialogSec::tr( r_SET_PERMISSIONS.c_str() ) ),
        QMessageBox::Ok );
}
void DialogSec::warnDirNotWritable( QWidget *parent )
{
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_DIR_NOT_WRITABLE.c_str() ),
        QString("%1\n\n%2").arg(
            DialogSec::tr( m_DIR_NOT_WRITABLE.c_str() ),
            DialogSec::tr( r_SET_PERMS_BEFORE.c_str() ) ),
        QMessageBox::Ok );
}


//////////////////
//// GENERICS ////
//////////////////
const bool DialogSec::choiceDirNotDir( QWidget *parent, const QString& path )
{
    bool choice = false;
    auto response = QMessageBox::question(parent,
        DialogSec::tr( t_DIR_NOT_DIR.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_DIR_NOT_DIR.c_str() ),
            path,
            DialogSec::tr( q_ENTRY_RENEW.c_str() ) ),
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
        DialogSec::tr( t_FILE_NOT_FILE.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_FILE_NOT_FILE.c_str() ),
            path,
            DialogSec::tr( q_ENTRY_RENEW.c_str() ) ),
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
        footer += "\n\n" + DialogSec::tr( r_REPORT_ISSUE.c_str() );
    }
    std::ignore=
    QMessageBox::warning(parent,
        DialogSec::tr( t_ERROR_OCCURED.c_str() ),
        QString("%1%2").arg(
            msg, footer ),
        QMessageBox::Ok );
}



void DialogSec::errGeneric( QWidget *parent, const QString& msg, const bool& report_msg )
{
    QString footer = "";
    if ( report_msg == true ) {
        footer += "\n\n" + DialogSec::tr( r_REPORT_ISSUE.c_str() );
    }
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_ERROR_OCCURED.c_str() ),
        QString("%1%2").arg(
            msg, footer ),
        QMessageBox::Ok );
}



void DialogSec::errRenaming( QWidget *parent, const QString& path )
{
    std::ignore=
    QMessageBox::critical(parent,
        DialogSec::tr( t_ERROR_RENAMING.c_str() ),
        QString("%1:\n%2\n\n%3").arg(
            DialogSec::tr( m_ERROR_RENAMING.c_str() ),
            path,
            DialogSec::tr( f_ABORTING.c_str() ) ),
        QMessageBox::Ok );
}

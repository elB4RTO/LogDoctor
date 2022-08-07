
#include "dialogs.h"
#include "mainwindow.h"


DialogSec::DialogSec()
{

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

/*bool DialogSec::choiceDatabaseNotFound( QWidget *parent, const std::string& db_name )
{
    return choiceDatabaseNotFound( parent,QString::fromStdString( db_name ) );
}*/
bool DialogSec::choiceDatabaseNotFound( QWidget *parent, const QString& db_name )
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

bool DialogSec::choiceDatabaseWrongTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg( m_DB_WRONG_TABLE_NAME, db_name, table_name );
    return choiceDatabaseRenew( parent, t_DB_WRONG_TABLE_NAME, msg );
}
bool DialogSec::choiceDatabaseMissingTable(QWidget *parent, const QString& db_name, const QString& table_name )
{
    QString msg = QString("%1:\n%2 -> %3").arg( m_DB_MISSING_TABLE, db_name, table_name );
    return choiceDatabaseRenew( parent, t_DB_MISSING_TABLE, msg );
}
bool DialogSec::choiceDatabaseWrongColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg( m_DB_WRONG_COLUMN_NAME, db_name, table_name, column_name );
    return choiceDatabaseRenew( parent, t_DB_WRONG_COLUMN_NAME, msg );
}
bool DialogSec::choiceDatabaseMissingColumn(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4").arg( m_DB_MISSING_COLUMN, db_name, table_name, column_name );
    return choiceDatabaseRenew( parent, t_DB_MISSING_COLUMN, msg );
}
bool DialogSec::choiceDatabaseWrongDataType(QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type )
{
    QString msg = QString("%1:\n%2 -> %3 -> %4 -> %5").arg( m_DB_WRONG_COLUMN_TYPE, db_name, table_name, column_name, data_type );
    return choiceDatabaseRenew( parent, t_DB_WRONG_COLUMN_TYPE, msg );
}
bool DialogSec::choiceDatabaseRenew(QWidget *parent, const QString& title, const QString& msg )
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

/*void DialogSec::errDatabaseNotReadable( QWidget *parent, const std::string& db_name )
{
    errDatabaseNotReadable( parent,QString::fromStdString( db_name ) );
}*/
void DialogSec::errDatabaseNotReadable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_READABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_NOT_READABLE, db_name, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}

/*void DialogSec::errDatabaseNotWritable( QWidget *parent, const std::string& db_name )
{
    errDatabaseNotWritable( parent,QString::fromStdString( db_name ) );
}*/
void DialogSec::errDatabaseNotWritable( QWidget *parent, const QString& db_name )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_NOT_WRITABLE,
        QString("%1:\n%2\n\n%3")
            .arg( m_DB_NOT_WRITABLE, db_name, r_SET_PERMISSIONS ),
        QMessageBox::Ok );
}

/*void DialogSec::errDatabaseFailedCreating( QWidget *parent, const std::string& db_name, const std::string& err_msg )
{
    errDatabaseFailedCreating( parent, QString::fromStdString(db_name), QString::fromStdString(err_msg) );
}*/
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

/*void DialogSec::errDatabaseFailedOpening( QWidget *parent, const std::string& db_name, const std::string& err_msg )
{
    errDatabaseFailedOpening( parent, QString::fromStdString(db_name), QString::fromStdString(err_msg) );
}*/
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

/*void DialogSec::errDatabaseFailedExecuting( QWidget *parent, const std::string& db_name, const std::string& statement, const std::string& err_msg )
{
    errDatabaseFailedExecuting( parent, QString::fromStdString(db_name), QString::fromStdString(statement), QString::fromStdString(err_msg) );
}*/
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



///////////////////
//// LOG FILES ////
///////////////////
/*void DialogSec::errFailedDefiningLogType( QWidget *parent, const std::string& file )
{
    errFailedDefiningLogType( parent, QString::fromStdString(file) );
}*/
void DialogSec::errFailedDefiningLogType( QWidget *parent, const QString& file )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_LOGTYPE_FAILED,
        QString("%1:\n%2")
            .arg( m_LOGTYPE_FAILED, file ),
        QMessageBox::Ok );
}


/*int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const std::string& msg )
{
    return choiceFileAlreadyUsed( parent, QString::fromStdString(msg) );
}*/
int DialogSec::choiceFileAlreadyUsed( QWidget *parent, const QString& msg )
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


/*int DialogSec::choiceFileSizeWarning( QWidget *parent, const std::string& msg )
{
    return choiceFileSizeWarning( parent, QString::fromStdString(msg) );
}*/
int DialogSec::choiceFileSizeWarning( QWidget *parent, const QString& msg )
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


/*void DialogSec::errFailedReadFile(QWidget *parent, const std::string& file , const bool skipping)
{
    errFailedReadFile( parent, QString::fromStdString( file ), skipping );
}*/
void DialogSec::errFailedReadFile(QWidget *parent, const QString& file , const bool& skipping )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_FILE_FAILED_READ,
        QString("%1:\n%2")
            .arg( m_FILE_FAILED_READ, file + ((skipping) ? f_SKIPPING : "") ),
        QMessageBox::Ok );
}


/*void DialogSec::warnEmptyFile( QWidget *parent, const std::string& file )
{
    warnEmptyFile( parent, QString::fromStdString( file ) );
}*/
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



/*bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const std::string& file )
{
    return choiceSelectedFileNotFound( parent, QString::fromStdString( file ) );
}*/
bool DialogSec::choiceSelectedFileNotFound(QWidget *parent, const QString& file )
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
/*void DialogSec::errDirNotExists(QWidget *parent, const std::string& dir )
{
    errDirNotExists( parent, QString::fromStdString( dir ) );
}*/
void DialogSec::errDirNotExists(QWidget *parent, const QString& dir )
{
    std::ignore=
    QMessageBox::critical(parent,
        t_DIR_NOT_FOUND,
        QString("%1:\n%2")
            .arg( m_DIR_NOT_EXISTS, dir ),
        QMessageBox::Ok );
}


/*bool DialogSec::choiceDirNotExists(QWidget *parent, const std::string& dir )
{
    return choiceDirNotExists( parent, QString::fromStdString( dir ) );
}*/
bool DialogSec::choiceDirNotExists(QWidget *parent, const QString& dir )
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



//////////////////
//// GENERICS ////
//////////////////
/*void DialogSec::warnGeneric(QWidget *parent, const std::string& msg, const bool report_msg )
{
    warnGeneric( parent, QString::fromStdString( msg ), report_msg );
}*/
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



/*void DialogSec::errGeneric(QWidget *parent, const std::string& msg, const bool report_msg )
{
    errGeneric( parent, QString::fromStdString( msg ), report_msg );
}*/
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

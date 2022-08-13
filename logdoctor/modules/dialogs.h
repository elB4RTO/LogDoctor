#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QString>
#include <QMessageBox>


static QString
    // questions
    q_CONTINUE    = QMessageBox::tr("Continue?"),
    q_YOUR_CHOICE = QMessageBox::tr("Your choice?"),
    q_DA          = QMessageBox::tr("Discard it and continue, or Abort all and exit?"),
    q_DIA         = QMessageBox::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort all and exit?"),
    q_DB_CREATE   = QMessageBox::tr("Create a new database?"),
    q_DB_RENEW    = QMessageBox::tr("This database will renamed with a trailing '.copy' and a new one will be created.\nContinue?"),
    q_ENTRY_RENEW = QMessageBox::tr("The entry will renamed with a trailing '.copy' and a new one will be created.\nContinue?"),

    // titles
    t_ERROR_OCCURED = QMessageBox::tr("An error occured"),

    t_ERROR_RENAMING = QMessageBox::tr("Failed renaming"),

    t_FILE_NOT_FILE = QMessageBox::tr("Not a file"),
    t_DIR_NOT_DIR   = QMessageBox::tr("Not a folder"),

    t_CONF_FILE_NOT_FOUND = QMessageBox::tr("Configuration file not found"),
    t_CONF_FILE_FAILED_W  = QMessageBox::tr("Failed to write configuration file"),

    t_DB_DRIVER_NOT_FOUND = QMessageBox::tr("QSql driver not found"),
    t_DB_CREATED          = QMessageBox::tr("Database created"),
    t_DB_FAILED_CREATING   = QMessageBox::tr("Failed creating database"),
    t_DB_FAILED_OPENING    = QMessageBox::tr("Failed opening database"),
    t_DB_FAILED_EXECUTING  = QMessageBox::tr("Failed executing on database"),
    t_DB_WRONG_TABLE_NAME  = QMessageBox::tr("Unexpected table"),
    t_DB_MISSING_TABLE     = QMessageBox::tr("Table not found"),
    t_DB_WRONG_COLUMN_NAME = QMessageBox::tr("Unexpected column"),
    t_DB_MISSING_COLUMN    = QMessageBox::tr("Column not found"),
    t_DB_WRONG_COLUMN_TYPE = QMessageBox::tr("Unexpected data-type"),

    t_LOGTYPE_FAILED    = QMessageBox::tr("Failed defining type"),

    t_LOGFORMAT_ERROR   = QMessageBox::tr("Log format error"),
    t_LOGFORMAT_MISCONFIGURED = QMessageBox::tr("Misconfigured log format"),
    t_LOGFORMAT_INVALID = QMessageBox::tr("Invalid log format string"),

    t_FILE_ALREADY_USED = QMessageBox::tr("File already used"),
    t_FILE_SIZE_WARNING = QMessageBox::tr("File exceeds warning size"),

    t_FILE_NOT_FOUND    = QMessageBox::tr("File not found"),
    t_FILE_NOT_READABLE = QMessageBox::tr("File not readable"),
    t_FILE_NOT_WRITABLE = QMessageBox::tr("File not writable"),
    t_FILE_EMPTY        = QMessageBox::tr("File is empty"),
    t_FILE_FAILED_READ  = QMessageBox::tr("Failed reading"),
    t_FILE_FAILED_WRITE = QMessageBox::tr("Failed writing"),

    t_DIR_NOT_FOUND    = QMessageBox::tr("Directory not found"),
    t_DIR_NOT_READABLE = QMessageBox::tr("Directory not readable"),
    t_DIR_NOT_WRITABLE = QMessageBox::tr("Directory not writable"),

    // messages
    m_ERROR_RENAMING = QMessageBox::tr("An error occured while renaming"),

    m_FILE_NOT_FILE = QMessageBox::tr("The path was supposed to point to a file, but it doesn't"),
    m_DIR_NOT_DIR   = QMessageBox::tr("The path was supposed to point to a folder, but it doesn't"),

    m_CONF_FILE_ERROR     = QMessageBox::tr("An error occured while handling the configuration file"),
    m_CONF_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the configuration file"),
    m_CONF_FILE_FAILED_W  = QMessageBox::tr("Current configuration not saved"),

    m_SELECTED_FILE_NOT_FOUND = QMessageBox::tr("Failed to retrieve the selected file"),
    m_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the file"),
    m_DIR_NOT_FOUND  = QMessageBox::tr("Unable to reach the directory"),

    m_FILE_NOT_EXISTS = QMessageBox::tr("The file does not exists"),
    m_DIR_NOT_EXISTS  = QMessageBox::tr("The directory does not exists"),

    m_FILE_NOT_READABLE = QMessageBox::tr("The file is not readable"),
    m_DIR_NOT_READABLE  = QMessageBox::tr("The directory is not readable"),

    m_FILE_NOT_WRITABLE = QMessageBox::tr("The file is not writable"),
    m_DIR_NOT_WRITABLE  = QMessageBox::tr("The directory is not writable"),

    m_FILE_EMPTY        = QMessageBox::tr("The file is blank"),
    m_FILE_FAILED_READ  = QMessageBox::tr("An error accured while reading the file"),
    m_FILE_FAILED_WRITE = QMessageBox::tr("An error accured while writing the file"),

    m_FILE_ALREADY_USED = QMessageBox::tr("The file has probably been used already"),
    m_FILE_SIZE_WARNING = QMessageBox::tr("The file's size exceeds the warning size"),

    m_DB_DRIVER_NOT_FOUND = QMessageBox::tr("Failed to retrieve the driver neede to handle the database"),
    m_DB_NOT_FOUND        = QMessageBox::tr("Failed to retrieve the database file"),
    m_DB_NOT_READABLE     = QMessageBox::tr("The database file is not readable"),
    m_DB_NOT_WRITABLE     = QMessageBox::tr("The database file is not writable"),
    m_DB_CREATED          = QMessageBox::tr("Successfully created a new database"),
    m_DB_FAILED_CREATING  = QMessageBox::tr("An error occured while creating the database"),
    m_DB_FAILED_OPENING   = QMessageBox::tr("An error occured while opening the database"),
    m_DB_FAILED_EXECUTING = QMessageBox::tr("An error occured while executing a statement on the database"),
    m_DB_FAILED_STATEMENT = QMessageBox::tr("Failed at statement"),
    m_DB_FAILED_ERRMSG    = QMessageBox::tr("SQLite error message"),
    m_DB_WRONG_TABLE_NAME  = QMessageBox::tr("The database contains an unexpected table"),
    m_DB_MISSING_TABLE     = QMessageBox::tr("It seems that the database is missing a table"),
    m_DB_MISSING_COLUMN    = QMessageBox::tr("It seems that the table is missing a column"),
    m_DB_WRONG_COLUMN_NAME = QMessageBox::tr("The database contains an unexpected column"),
    m_DB_WRONG_COLUMN_TYPE = QMessageBox::tr("A column has an unexpected data-type"),

    m_LOGTYPE_FAILED    = QMessageBox::tr("Failed to determine the log type"),

    m_LOGFORMAT_NOT_SET   = QMessageBox::tr("The log format has not been set, or is invalid\nPlease add a valid one in the configurations"),
    m_LOGFORMAT_NO_FIELDS = QMessageBox::tr("No log field has been set in the current logs format,\nmaking it useless to parse logs"),
    m_LOGFORMAT_NO_SEPS   = QMessageBox::tr("A separator is missing between one or more fields,\nmaking it hard to establish net bounds,\nand possibly leading to store incorrect data"),

    // requests
    r_REPORT_ISSUE     = QMessageBox::tr("Please report this issue"),
    r_REMOVE_ENTRY     = QMessageBox::tr("Please remove the conflict and retry"),
    r_SET_PERMS_BEFORE = QMessageBox::tr("Please set the proper permissions before to start"),
    r_SET_PERMISSIONS  = QMessageBox::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue"),
    r_DB_DONT_EDIT     = QMessageBox::tr("If you haven't manually edited the database,\nplease report this issue"),

    r_CHECK_FORMAT      = QMessageBox::tr("Please check that no error is thrown from your WebServer\nIf it gets accepted, please check the presence of a typo here\nIf everything is fine, please report this issue"),
    r_SET_FORMAT_FIELDS = QMessageBox::tr("Please set up a format which contains at least one field"),
    r_SET_FORMAT_SEPS   = QMessageBox::tr("Please set up a format which contains separators between fields"),

    // footers
    f_SKIPPING = QMessageBox::tr("Skipping"),
    f_ABORTING = QMessageBox::tr("Aborting")
;


class DialogSec
{
public:
    DialogSec();

    static void warnGeneric( QWidget *parent, const QString& message, const bool& report_msg=false );
    static void errGeneric( QWidget *parent, const QString& message, const bool& report_msg=false );

    static void errRenaming( QWidget *parent, const QString& path );

    // configurations
    static void errConfFailedWriting( QWidget *parent, const QString& msg="" );
    static void warnConfFileNotFound( QWidget *parent, const QString& file );
    static void errConfFileNotReadable( QWidget *parent, const QString& file );
    static void errConfFileNotWritable( QWidget *parent, const QString& file );
    static void errConfDirNotWritable( QWidget *parent, const QString& dir );

    // database
    static void errSqlDriverNotFound( QWidget *parent, const QString& driver );
    static const bool choiceDatabaseNotFound( QWidget *parent, const QString& db_name );
    static const bool choiceDatabaseWrongTable( QWidget *parent, const QString& db_name, const QString& table_name );
    static const bool choiceDatabaseMissingTable( QWidget *parent, const QString& db_name, const QString& table_name );
    static const bool choiceDatabaseWrongColumn( QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name );
    static const bool choiceDatabaseMissingColumn( QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name );
    static const bool choiceDatabaseWrongDataType( QWidget *parent, const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type );
    static void errDatabaseNotFile( QWidget *parent, const QString& db_name );
    static void errDatabaseNotReadable( QWidget *parent, const QString& db_name );
    static void errDatabaseNotWritable( QWidget *parent, const QString& db_name );
    static void msgDatabaseCreated( QWidget *parent, const QString& db_name );
    static void errDatabaseFailedCreating( QWidget *parent, const QString& db_name, const QString& err_msg );
    static void errDatabaseFailedOpening( QWidget *parent, const QString& db_name, const QString& err_msg );
    static void errDatabaseFailedExecuting( QWidget *parent, const QString& db_name, const QString& statement="", const QString& err_msg="" );

    // LogsList
    static const bool choiceSelectedFileNotFound( QWidget *parent, const QString& file );

    // log files type
    static void errFailedDefiningLogType( QWidget *parent, const QString& file );
    // log files hash
    static const int choiceFileAlreadyUsed( QWidget *parent, const QString& msg );
    // log files size
    static const int choiceFileSizeWarning( QWidget *parent, const QString& msg );

    // log format string invalid
    static void errInvalidLogFormatString( QWidget *parent, const QString& msg );
    // log format not set
    static void errLogFormatNotSet( QWidget *parent );
    static void errLogFormatNoFields( QWidget *parent );
    static void errLogFormatNoSeparators( QWidget *parent );

    // files permissions
    /*static void errFileNotExists( QWidget *parent );
    static void errFileNotReadable( QWidget *parent );
    static void errFileNotWritable( QWidget *parent );*/
    static void warnFileNotReadable( QWidget *parent, const QString& file );
    /*static const bool choiceFileNotExists( QWidget *parent );
    static const bool choiceFileNotReadable( QWidget *parent );
    static const bool choiceFileNotWritable( QWidget *parent );*/
    // files actions
    static void warnEmptyFile( QWidget *parent, const QString& file );
    static void errFailedReadFile( QWidget *parent, const QString& file, const bool& skipping=false );
    static void errFailedWriteFile( QWidget *parent, const QString& file, const bool& skipping=false );

    // folders permissions
    static void errDirNotExists( QWidget *parent, const QString& dir );
    static void errDirNotReadable( QWidget *parent, const QString& dir );
    static void errDirNotWritable( QWidget *parent, const QString& dir );
    static void warnDirNotReadable( QWidget *parent );
    static void warnDirNotWritable( QWidget *parent );
    static const bool choiceDirNotExists( QWidget *parent, const QString& dir );
    /*static void choiceDirNotReadable( QWidget *parent );
    static void choiceDirNotWritable( QWidget *parent );*/

    // generic choices
    static const bool choiceDirNotDir( QWidget *parent, const QString& path );
    static const bool choiceFileNotFile( QWidget *parent, const QString& path );
    /*static void choiceYesNo( QWidget *parent );
    static const bool choiceIgnoreAbort( QWidget *parent );*/



private:
    static const bool choiceDatabaseRenew( QWidget *parent, const QString& title, const QString& msg );

};

#endif // DIALOGS_H

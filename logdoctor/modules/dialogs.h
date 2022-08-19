#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QString>
#include <QMessageBox>


///////////////////
//// QUESTIONS ////
static const QString q_CONTINUE    = QMessageBox::tr("Continue?");
static const QString q_YOUR_CHOICE = QMessageBox::tr("Your choice?");
static const QString q_DA          = QMessageBox::tr("Discard it and continue, or Abort all and exit?");
static const QString q_DIA         = QMessageBox::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort all and exit?");
static const QString q_DB_CREATE   = QMessageBox::tr("Create a new database?");
static const QString q_DB_RENEW    = QMessageBox::tr("This database will renamed with a trailing '.copy' and a new one will be created.\nContinue?");
static const QString q_ENTRY_RENEW = QMessageBox::tr("The entry will renamed with a trailing '.copy' and a new one will be created.\nContinue?");

////////////////
//// TITLES ////
static const QString t_ERROR_OCCURED = QMessageBox::tr("An error occured");

// IO
static const QString t_ERROR_RENAMING = QMessageBox::tr("Failed renaming");

// languages
static const QString t_LANG_INVALID = QMessageBox::tr("Invalid locale");

// configuration
static const QString t_CONF_FILE_NOT_FOUND = QMessageBox::tr("Configuration file not found");
static const QString t_CONF_FILE_FAILED_W  = QMessageBox::tr("Failed to write configuration file");

// updates
static const QString t_VERSION_CHECK        = QMessageBox::tr("Version check");
static const QString t_VERSION_CHECK_FAILED = QMessageBox::tr("Version check failed");
static const QString t_CONNECTION_FAILED    = QMessageBox::tr("Failed to establish a connection");

// help
static const QString t_HELP_FAILED_LOADING = QMessageBox::tr("Failed to retrieve the help file");

// IO files
static const QString t_FILE_NOT_FILE     = QMessageBox::tr("Not a file");
static const QString t_FILE_NOT_FOUND    = QMessageBox::tr("File not found");
static const QString t_FILE_NOT_READABLE = QMessageBox::tr("File not readable");
static const QString t_FILE_NOT_WRITABLE = QMessageBox::tr("File not writable");
static const QString t_FILE_EMPTY        = QMessageBox::tr("File is empty");
static const QString t_FILE_FAILED_READ  = QMessageBox::tr("Failed reading");
static const QString t_FILE_FAILED_WRITE = QMessageBox::tr("Failed writing");

// IO directories
static const QString t_DIR_NOT_DIR      = QMessageBox::tr("Not a folder");
static const QString t_DIR_NOT_FOUND    = QMessageBox::tr("Directory not found");
static const QString t_DIR_NOT_READABLE = QMessageBox::tr("Directory not readable");
static const QString t_DIR_NOT_WRITABLE = QMessageBox::tr("Directory not writable");

// database
static const QString t_DB_DRIVER_NOT_FOUND  = QMessageBox::tr("QSql driver not found");
static const QString t_DB_CREATED           = QMessageBox::tr("Database created");
static const QString t_DB_FAILED_CREATING   = QMessageBox::tr("Failed creating database");
static const QString t_DB_FAILED_OPENING    = QMessageBox::tr("Failed opening database");
static const QString t_DB_FAILED_EXECUTING  = QMessageBox::tr("Failed executing on database");
static const QString t_DB_WRONG_TABLE_NAME  = QMessageBox::tr("Unexpected table");
static const QString t_DB_MISSING_TABLE     = QMessageBox::tr("Table not found");
static const QString t_DB_WRONG_COLUMN_NAME = QMessageBox::tr("Unexpected column");
static const QString t_DB_MISSING_COLUMN    = QMessageBox::tr("Column not found");
static const QString t_DB_WRONG_COLUMN_TYPE = QMessageBox::tr("Unexpected data-type");

// log type
static const QString t_LOGTYPE_FAILED = QMessageBox::tr("Failed defining type");

// log format
static const QString t_LOGFORMAT_ERROR   = QMessageBox::tr("Log format error");
static const QString t_LOGFORMAT_MISCONFIGURED = QMessageBox::tr("Misconfigured log format");
static const QString t_LOGFORMAT_INVALID = QMessageBox::tr("Invalid log format string");

// log files
static const QString t_FILE_ALREADY_USED = QMessageBox::tr("File already used");
static const QString t_FILE_SIZE_WARNING = QMessageBox::tr("File exceeds warning size");

//////////////////
//// MESSAGES ////
// IO
static const QString m_ERROR_RENAMING = QMessageBox::tr("An error occured while renaming");

// language
static const QString m_LANG_LOCALE_INVALID = QMessageBox::tr("Unexpected locale format");
static const QString m_LANG_NOT_ACCEPTED   = QMessageBox::tr("The given locale is not an accepted language");

// configuration
static const QString m_CONF_FILE_ERROR     = QMessageBox::tr("An error occured while handling the configuration file");
static const QString m_CONF_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the configuration file");
static const QString m_CONF_FILE_FAILED_W  = QMessageBox::tr("Current configuration not saved");

// updates
static const QString  m_VERSION_CHECK_FAILED = QMessageBox::tr("Failed to fetch the upstream version");
static const QString  m_CONNECTION_FAILED    = QMessageBox::tr("An error occured while trying to establish a connection with");
static const QString  m_CONNECTIONS_FAILED   = QMessageBox::tr("An error occured while establishing a connection with any of the hosts");
static const QString  m_VERSION_NOT_FOUND    = QMessageBox::tr("An error occured while parsing:\nversion mark not found");
static const QString  m_VERSION_MALFORMED    = QMessageBox::tr("An error occured while parsing:\nmalformed version");
static const QString  m_NEW_VERSION_AVAILABLE  = QMessageBox::tr("A new version available!\n\nPlease visit LogDoctor's git repository\nand follow the instruction about how to update");
static const QString  m_SAME_VERSION           = QMessageBox::tr("LogDoctor is up-to-date");
static const QString  m_FUTURE_VERSION_RUNNING = QMessageBox::tr("You're running a version from the future!\n\nYour version is beyond the current upstream version\n\nAre you running the original LogDoctor?\nIf you think so, you may be a victim of a malware\nPlease visit the LogDoctor's repository and get a fresh version of it");

// help
static const QString  m_HELP_FAILED_LOADING = QMessageBox::tr("An error occured while getting the help file");

// IO files
static const QString  m_FILE_NOT_FILE     = QMessageBox::tr("The path was supposed to point to a file, but it doesn't");
static const QString  m_FILE_NOT_EXISTS   = QMessageBox::tr("The file does not exists");
static const QString  m_FILE_NOT_FOUND    = QMessageBox::tr("Unable to retrieve the file");
static const QString  m_FILE_NOT_READABLE = QMessageBox::tr("The file is not readable");
static const QString  m_FILE_NOT_WRITABLE = QMessageBox::tr("The file is not writable");
static const QString  m_FILE_EMPTY        = QMessageBox::tr("The file is blank");
static const QString  m_FILE_FAILED_READ  = QMessageBox::tr("An error accured while reading the file");
static const QString  m_FILE_FAILED_WRITE = QMessageBox::tr("An error accured while writing the file");

// IO directories
static const QString  m_DIR_NOT_DIR       = QMessageBox::tr("The path was supposed to point to a folder, but it doesn't");
static const QString  m_DIR_NOT_EXISTS    = QMessageBox::tr("The directory does not exists");
static const QString  m_DIR_NOT_FOUND     = QMessageBox::tr("Unable to reach the directory");
static const QString  m_DIR_NOT_READABLE  = QMessageBox::tr("The directory is not readable");
static const QString  m_DIR_NOT_WRITABLE  = QMessageBox::tr("The directory is not writable");

// database
static const QString  m_DB_DRIVER_NOT_FOUND  = QMessageBox::tr("Failed to retrieve the driver neede to handle the database");
static const QString  m_DB_NOT_FOUND         = QMessageBox::tr("Failed to retrieve the database file");
static const QString  m_DB_NOT_READABLE      = QMessageBox::tr("The database file is not readable");
static const QString  m_DB_NOT_WRITABLE      = QMessageBox::tr("The database file is not writable");
static const QString  m_DB_CREATED           = QMessageBox::tr("Successfully created a new database");
static const QString  m_DB_FAILED_CREATING   = QMessageBox::tr("An error occured while creating the database");
static const QString  m_DB_FAILED_OPENING    = QMessageBox::tr("An error occured while opening the database");
static const QString  m_DB_FAILED_EXECUTING  = QMessageBox::tr("An error occured while executing a statement on the database");
static const QString  m_DB_FAILED_STATEMENT  = QMessageBox::tr("Failed at statement");
static const QString  m_DB_FAILED_ERRMSG     = QMessageBox::tr("SQLite error message");
static const QString  m_DB_WRONG_TABLE_NAME  = QMessageBox::tr("The database contains an unexpected table");
static const QString  m_DB_MISSING_TABLE     = QMessageBox::tr("It seems that the database is missing a table");
static const QString  m_DB_MISSING_COLUMN    = QMessageBox::tr("It seems that the table is missing a column");
static const QString  m_DB_WRONG_COLUMN_NAME = QMessageBox::tr("The database contains an unexpected column");
static const QString  m_DB_WRONG_COLUMN_TYPE = QMessageBox::tr("A column has an unexpected data-type");

// log files
static const QString  m_SELECTED_FILE_NOT_FOUND = QMessageBox::tr("Failed to retrieve the selected file");
static const QString  m_FILE_ALREADY_USED = QMessageBox::tr("The file has probably been used already");
static const QString  m_FILE_SIZE_WARNING = QMessageBox::tr("The file's size exceeds the warning size");

// log type
static const QString  m_LOGTYPE_FAILED = QMessageBox::tr("Failed to determine the log type");

// log format
static const QString  m_LOGFORMAT_NOT_SET   = QMessageBox::tr("The log format has not been set, or is invalid\nPlease add a valid one in the configurations");
static const QString  m_LOGFORMAT_NO_FIELDS = QMessageBox::tr("No log field has been set in the current logs format,\nmaking it useless to parse logs");
static const QString  m_LOGFORMAT_NO_SEPS   = QMessageBox::tr("A separator is missing between one or more fields,\nmaking it hard to establish net bounds,\nand possibly leading to store incorrect data");

//////////////////
//// REQUESTS ////
static const QString  r_REPORT_ISSUE     = QMessageBox::tr("Please report this issue");

// language
static const QString  r_LANG_NOT_ACCEPTED = QMessageBox::tr("IF you'd like to have this locale in LogDoctor,\nplease follow the instruction on the repository page");

// version check
static const QString  r_UPSTREAM_FAILURE = QMessageBox::tr("Please check your internet connection and retry\nIf this error persists, please report this issue");

// IO conflict
static const QString  r_REMOVE_ENTRY     = QMessageBox::tr("Please remove the conflict and retry");

// permissions
static const QString  r_SET_PERMS_BEFORE = QMessageBox::tr("Please set the proper permissions before to start");
static const QString  r_SET_PERMISSIONS  = QMessageBox::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue");

// don't edit
static const QString  r_CONF_DONT_EDIT   = QMessageBox::tr("If you haven't manually edited the configuration file,\nplease report this issue");
static const QString  r_DB_DONT_EDIT     = QMessageBox::tr("If you haven't manually edited the database,\nplease report this issue");

// log format
static const QString  r_CHECK_FORMAT      = QMessageBox::tr("Please check that no error is thrown from your WebServer\nIf it gets accepted, please check the presence of a typo here\nIf everything is fine, please report this issue");
static const QString  r_SET_FORMAT_FIELDS = QMessageBox::tr("Please set up a format which contains at least one field");
static const QString  r_SET_FORMAT_SEPS   = QMessageBox::tr("Please set up a format which contains separators between fields");

/////////////////
//// FOOTERS ////
static const QString  f_SKIPPING = QMessageBox::tr("Skipping");
static const QString  f_ABORTING = QMessageBox::tr("Aborting");

// additional resources
static const QString  f_ADD_RESOURCES = QMessageBox::tr("Additional resources can be downloaded from the git repo");


class DialogSec
{
public:
    DialogSec();

    static void warnGeneric( QWidget *parent, const QString& message, const bool& report_msg=false );
    static void errGeneric( QWidget *parent, const QString& message, const bool& report_msg=false );

    static void errRenaming( QWidget *parent, const QString& path );

    // language
    static void errLangLocaleInvalid( QWidget *parent, const QString& locale );
    static void errLangNotAccepted( QWidget *parent, const QString& locale );

    // configurations
    static void errConfFailedWriting( QWidget *parent, const QString& msg="" );
    static void warnConfFileNotFound( QWidget *parent, const QString& file );
    static void errConfFileNotReadable( QWidget *parent, const QString& file );
    static void errConfFileNotWritable( QWidget *parent, const QString& file );
    static void errConfDirNotWritable( QWidget *parent, const QString& dir );

    // update
    static void errVersionCheckFailed( QWidget *parent, const int& err_code );
    static void warnConnectionFailed( QWidget *parent, const QString& link, const QString& err_msg );
    static void msgNewVersion( QWidget *parent );
    static void msgSameVersion( QWidget *parent );
    static void warnFutureVersion( QWidget *parent );

    // help
    static void errHelpFailed( QWidget *parent, const QString& link, const QString& msg="" );
    static void errHelpNotFound( QWidget *parent, const QString& link );
    static void errHelpNotReadable( QWidget *parent, const QString& link );

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

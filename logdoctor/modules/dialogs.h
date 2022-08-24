#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QString>
#include <QMessageBox>

#include <string>


class DialogSec : public QObject
{
    Q_OBJECT

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
    static void errDatabaseFailedBackup( QWidget *parent, const QString& msg );

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

    // files
    static void errFileNotFound( QWidget *parent, const QString& file, const bool& report=false );
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
    static const bool choiceDirNotExists( QWidget *parent, const QString& dir );
    static void errDirNotExists( QWidget *parent, const QString& dir );
    static void errDirNotReadable( QWidget *parent, const QString& dir );
    static void errDirNotWritable( QWidget *parent, const QString& dir );
    static void warnDirNotReadable( QWidget *parent );
    static void warnDirNotWritable( QWidget *parent );
    /*static void choiceDirNotReadable( QWidget *parent );
    static void choiceDirNotWritable( QWidget *parent );*/
    // folders actions
    static void errFailedMakeDir( QWidget *parent, const QString& msg );

    // generic choices
    static const bool choiceDirNotDir( QWidget *parent, const QString& path );
    static const bool choiceFileNotFile( QWidget *parent, const QString& path );
    /*static void choiceYesNo( QWidget *parent );
    static const bool choiceIgnoreAbort( QWidget *parent );*/



private:
    static const bool choiceDatabaseRenew( QWidget *parent, const QString& title, const QString& msg );

};


///////////////////
//// QUESTIONS ////
const std::string q_CONTINUE    = DialogSec::tr("Continue?").toStdString();
const std::string q_YOUR_CHOICE = DialogSec::tr("Your choice?").toStdString();
const std::string q_DA          = DialogSec::tr("Discard it and continue, or Abort all and exit?").toStdString();
const std::string q_DIA         = DialogSec::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort all and exit?").toStdString();
const std::string q_DB_CREATE   = DialogSec::tr("Create a new database?").toStdString();
const std::string q_DB_RENEW    = DialogSec::tr("This database will renamed with a trailing '.copy' and a new one will be created.\nContinue?").toStdString();
const std::string q_ENTRY_RENEW = DialogSec::tr("The entry will renamed with a trailing '.copy' and a new one will be created.\nContinue?").toStdString();

////////////////
//// TITLES ////
const std::string t_ERROR_OCCURED = DialogSec::tr("An error occured").toStdString();

// IO
const std::string t_ERROR_RENAMING = DialogSec::tr("Failed renaming").toStdString();

// languages
const std::string t_LANG_INVALID = DialogSec::tr("Invalid locale").toStdString();

// configuration
const std::string t_CONF_FILE_NOT_FOUND = DialogSec::tr("Configuration file not found").toStdString();
const std::string t_CONF_FILE_FAILED_W  = DialogSec::tr("Failed to write configuration file").toStdString();

// updates
const std::string t_VERSION_CHECK        = DialogSec::tr("Version check").toStdString();
const std::string t_VERSION_CHECK_FAILED = DialogSec::tr("Version check failed").toStdString();
const std::string t_CONNECTION_FAILED    = DialogSec::tr("Failed to establish a connection").toStdString();

// help
const std::string t_HELP_FAILED_LOADING = DialogSec::tr("Failed to retrieve the help file").toStdString();

// IO files
const std::string t_FILE_NOT_FILE     = DialogSec::tr("Not a file").toStdString();
const std::string t_FILE_NOT_FOUND    = DialogSec::tr("File not found").toStdString();
const std::string t_FILE_NOT_READABLE = DialogSec::tr("File not readable").toStdString();
const std::string t_FILE_NOT_WRITABLE = DialogSec::tr("File not writable").toStdString();
const std::string t_FILE_EMPTY        = DialogSec::tr("File is empty").toStdString();
const std::string t_FILE_FAILED_READ  = DialogSec::tr("Failed reading").toStdString();
const std::string t_FILE_FAILED_WRITE = DialogSec::tr("Failed writing").toStdString();

// IO directories
const std::string t_DIR_NOT_DIR      = DialogSec::tr("Not a folder").toStdString();
const std::string t_DIR_NOT_FOUND    = DialogSec::tr("Directory not found").toStdString();
const std::string t_DIR_NOT_READABLE = DialogSec::tr("Directory not readable").toStdString();
const std::string t_DIR_NOT_WRITABLE = DialogSec::tr("Directory not writable").toStdString();
const std::string t_DIR_FAILED_MAKE  = DialogSec::tr("Failed creating directory").toStdString();

// database
const std::string t_DB_DRIVER_NOT_FOUND  = DialogSec::tr("QSql driver not found").toStdString();
const std::string t_DB_CREATED           = DialogSec::tr("Database created").toStdString();
const std::string t_DB_FAILED_CREATING   = DialogSec::tr("Failed creating database").toStdString();
const std::string t_DB_FAILED_OPENING    = DialogSec::tr("Failed opening database").toStdString();
const std::string t_DB_FAILED_EXECUTING  = DialogSec::tr("Failed executing on database").toStdString();
const std::string t_DB_WRONG_TABLE_NAME  = DialogSec::tr("Unexpected table").toStdString();
const std::string t_DB_MISSING_TABLE     = DialogSec::tr("Table not found").toStdString();
const std::string t_DB_WRONG_COLUMN_NAME = DialogSec::tr("Unexpected column").toStdString();
const std::string t_DB_MISSING_COLUMN    = DialogSec::tr("Column not found").toStdString();
const std::string t_DB_WRONG_COLUMN_TYPE = DialogSec::tr("Unexpected data-type").toStdString();
const std::string t_DB_FAILED_BACKUP     = DialogSec::tr("Failed to backup database").toStdString();

// log type
const std::string t_LOGTYPE_FAILED = DialogSec::tr("Failed defining type").toStdString();

// log format
const std::string t_LOGFORMAT_ERROR   = DialogSec::tr("Log format error").toStdString();
const std::string t_LOGFORMAT_MISCONFIGURED = DialogSec::tr("Misconfigured log format").toStdString();
const std::string t_LOGFORMAT_INVALID = DialogSec::tr("Invalid log format string").toStdString();

// log files
const std::string t_FILE_ALREADY_USED = DialogSec::tr("File already used").toStdString();
const std::string t_FILE_SIZE_WARNING = DialogSec::tr("File exceeds warning size").toStdString();

//////////////////
//// MESSAGES ////
// IO
const std::string m_ERROR_RENAMING = DialogSec::tr("An error occured while renaming").toStdString();

// language
const std::string m_LANG_LOCALE_INVALID = DialogSec::tr("Unexpected locale format").toStdString();
const std::string m_LANG_NOT_ACCEPTED   = DialogSec::tr("The given locale is not an accepted language").toStdString();

// configuration
const std::string m_CONF_FILE_ERROR     = DialogSec::tr("An error occured while handling the configuration file").toStdString();
const std::string m_CONF_FILE_NOT_FOUND = DialogSec::tr("Unable to retrieve the configuration file").toStdString();
const std::string m_CONF_FILE_FAILED_W  = DialogSec::tr("Current configuration not saved").toStdString();

// updates
const std::string m_VERSION_CHECK_FAILED = DialogSec::tr("Failed to fetch the upstream version").toStdString();
const std::string m_CONNECTION_FAILED    = DialogSec::tr("An error occured while trying to establish a connection with").toStdString();
const std::string m_CONNECTIONS_FAILED   = DialogSec::tr("An error occured while establishing a connection with any of the hosts").toStdString();
const std::string m_VERSION_NOT_FOUND    = DialogSec::tr("An error occured while parsing:\nversion mark not found").toStdString();
const std::string m_VERSION_MALFORMED    = DialogSec::tr("An error occured while parsing:\nmalformed version").toStdString();
const std::string m_NEW_VERSION_AVAILABLE  = DialogSec::tr("A new version available!\n\nPlease visit LogDoctor's git repository\nand follow the instruction about how to update").toStdString();
const std::string m_SAME_VERSION           = DialogSec::tr("LogDoctor is up-to-date").toStdString();
const std::string m_FUTURE_VERSION_RUNNING = DialogSec::tr("You're running a version from the future!\n\nYour version is beyond the current upstream version\n\nAre you running the original LogDoctor?\nIf you think so, you may be a victim of a malware\nPlease visit the LogDoctor's repository and get a fresh version of it").toStdString();

// help
const std::string m_HELP_FAILED_LOADING = DialogSec::tr("An error occured while getting the help file").toStdString();

// IO files
const std::string m_FILE_NOT_FILE     = DialogSec::tr("The path was supposed to point to a file, but it doesn't").toStdString();
const std::string m_FILE_NOT_EXISTS   = DialogSec::tr("The file does not exists").toStdString();
const std::string m_FILE_NOT_FOUND    = DialogSec::tr("Unable to retrieve the file").toStdString();
const std::string m_FILE_NOT_READABLE = DialogSec::tr("The file is not readable").toStdString();
const std::string m_FILE_NOT_WRITABLE = DialogSec::tr("The file is not writable").toStdString();
const std::string m_FILE_EMPTY        = DialogSec::tr("The file is blank").toStdString();
const std::string m_FILE_FAILED_READ  = DialogSec::tr("An error accured while reading the file").toStdString();
const std::string m_FILE_FAILED_WRITE = DialogSec::tr("An error accured while writing the file").toStdString();

// IO directories
const std::string m_DIR_NOT_DIR       = DialogSec::tr("The path was supposed to point to a folder, but it doesn't").toStdString();
const std::string m_DIR_NOT_EXISTS    = DialogSec::tr("The directory does not exists").toStdString();
const std::string m_DIR_NOT_FOUND     = DialogSec::tr("Unable to reach the directory").toStdString();
const std::string m_DIR_NOT_READABLE  = DialogSec::tr("The directory is not readable").toStdString();
const std::string m_DIR_NOT_WRITABLE  = DialogSec::tr("The directory is not writable").toStdString();

// database
const std::string m_DB_DRIVER_NOT_FOUND  = DialogSec::tr("Failed to retrieve the driver neede to handle the database").toStdString();
const std::string m_DB_NOT_FOUND         = DialogSec::tr("Failed to retrieve the database file").toStdString();
const std::string m_DB_NOT_READABLE      = DialogSec::tr("The database file is not readable").toStdString();
const std::string m_DB_NOT_WRITABLE      = DialogSec::tr("The database file is not writable").toStdString();
const std::string m_DB_CREATED           = DialogSec::tr("Successfully created a new database").toStdString();
const std::string m_DB_FAILED_CREATING   = DialogSec::tr("An error occured while creating the database").toStdString();
const std::string m_DB_FAILED_OPENING    = DialogSec::tr("An error occured while opening the database").toStdString();
const std::string m_DB_FAILED_EXECUTING  = DialogSec::tr("An error occured while executing a statement on the database").toStdString();
const std::string m_DB_FAILED_STATEMENT  = DialogSec::tr("Failed at statement").toStdString();
const std::string m_DB_FAILED_ERRMSG     = DialogSec::tr("SQLite error message").toStdString();
const std::string m_DB_WRONG_TABLE_NAME  = DialogSec::tr("The database contains an unexpected table").toStdString();
const std::string m_DB_MISSING_TABLE     = DialogSec::tr("It seems that the database is missing a table").toStdString();
const std::string m_DB_MISSING_COLUMN    = DialogSec::tr("It seems that the table is missing a column").toStdString();
const std::string m_DB_WRONG_COLUMN_NAME = DialogSec::tr("The database contains an unexpected column").toStdString();
const std::string m_DB_WRONG_COLUMN_TYPE = DialogSec::tr("A column has an unexpected data-type").toStdString();

// log files
const std::string m_SELECTED_FILE_NOT_FOUND = DialogSec::tr("Failed to retrieve the selected file").toStdString();
const std::string m_FILE_ALREADY_USED = DialogSec::tr("The file has probably been used already").toStdString();
const std::string m_FILE_SIZE_WARNING = DialogSec::tr("The file's size exceeds the warning size").toStdString();

// log type
const std::string m_LOGTYPE_FAILED = DialogSec::tr("Failed to determine the log type").toStdString();

// log format
const std::string m_LOGFORMAT_NOT_SET   = DialogSec::tr("The log format has not been set, or is invalid\nPlease add a valid one in the configurations").toStdString();
const std::string m_LOGFORMAT_NO_FIELDS = DialogSec::tr("No log field has been set in the current logs format,\nmaking it useless to parse logs").toStdString();
const std::string m_LOGFORMAT_NO_SEPS   = DialogSec::tr("A separator is missing between one or more fields,\nmaking it hard to establish net bounds,\nand possibly leading to store incorrect data").toStdString();

//////////////////
//// REQUESTS ////
const std::string r_REPORT_ISSUE     = DialogSec::tr("Please report this issue").toStdString();

// language
const std::string r_LANG_NOT_ACCEPTED = DialogSec::tr("IF you'd like to have this locale in LogDoctor,\nplease follow the instruction on the repository page").toStdString();

// version check
const std::string r_UPSTREAM_FAILURE = DialogSec::tr("Please check your internet connection and retry\nIf this error persists, please report this issue").toStdString();

// IO conflict
const std::string r_REMOVE_ENTRY     = DialogSec::tr("Please remove the conflict and retry").toStdString();

// permissions
const std::string r_SET_PERMS_BEFORE = DialogSec::tr("Please set the proper permissions before to start").toStdString();
const std::string r_SET_PERMISSIONS  = DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue").toStdString();

// don't edit
const std::string r_CONF_DONT_EDIT   = DialogSec::tr("If you haven't manually edited the configuration file,\nplease report this issue").toStdString();
const std::string r_DB_DONT_EDIT     = DialogSec::tr("If you haven't manually edited the database,\nplease report this issue").toStdString();

// log format
const std::string r_CHECK_FORMAT      = DialogSec::tr("Please check that no error is thrown from your WebServer\nIf it gets accepted, please check the presence of a typo here\nIf everything is fine, please report this issue").toStdString();
const std::string r_SET_FORMAT_FIELDS = DialogSec::tr("Please set up a format which contains at least one field").toStdString();
const std::string r_SET_FORMAT_SEPS   = DialogSec::tr("Please set up a format which contains separators between fields").toStdString();

/////////////////
//// FOOTERS ////
const std::string f_SKIPPING = DialogSec::tr("Skipping").toStdString();
const std::string f_ABORTING = DialogSec::tr("Aborting").toStdString();

// additional resources
const std::string f_ADD_RESOURCES = DialogSec::tr("Additional resources can be downloaded from the git repo").toStdString();


#endif // DIALOGS_H

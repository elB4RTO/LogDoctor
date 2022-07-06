#ifndef DIALOGS_H
#define DIALOGS_H

#include "qwidget.h"
#include "qstring.h"
#include "qmessagebox.h"


static QString
    // questions
    q_CONTINUE    = QMessageBox::tr("Continue?"),
    q_YOUR_CHOICE = QMessageBox::tr("Your choice?"),
    q_DA          = QMessageBox::tr("Discard it and continue, or Abort all and exit?"),
    q_DIA         = QMessageBox::tr("Ignore and use it anyway, Discard it and continue, or Abort all and exit?"),
    q_DB_CREATE   = QMessageBox::tr("Create a new database?"),

    // titles
    t_ERROR_OCCURED = QMessageBox::tr("An error occured"),

    t_DB_FAILED_CREATING  = QMessageBox::tr("Failed creating database"),
    t_DB_FAILED_OPENING   = QMessageBox::tr("Failed opening database"),
    t_DB_FAILED_EXECUTING = QMessageBox::tr("Failed executing on database"),

    t_LOGTYPE_FAILED    = QMessageBox::tr("Failed defining type"),
    t_LOGTYPE_UNDEFINED = QMessageBox::tr("Undefined type"),

    t_FILE_ALREADY_USED = QMessageBox::tr("File already used"),
    t_FILE_SIZE_WARNING = QMessageBox::tr("File exceeds warning size"),

    t_FILE_NOT_FOUND    = QMessageBox::tr("File not found"),
    t_FILE_NOT_READABLE = QMessageBox::tr("File not readable"),
    t_FILE_NOT_WRITABLE = QMessageBox::tr("File not writable"),
    t_FILE_EMPTY        = QMessageBox::tr("File is empty"),
    t_FILE_FAILED_READ  = QMessageBox::tr("Failed reading"),

    t_DIR_NOT_FOUND = QMessageBox::tr("Directory not found"),

    // messages
    m_SELECTED_FILE_NOT_FOUND = QMessageBox::tr("Failed to retrieve the selected file"),
    m_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the file"),
    m_DIR_NOT_FOUND  = QMessageBox::tr("Unable to reach the directory"),

    m_FILE_NOT_EXISTS = QMessageBox::tr("The file does not exists"),
    m_DIR_NOT_EXISTS  = QMessageBox::tr("The directory does not exists"),

    m_FILE_EMPTY       = QMessageBox::tr("The file is blank"),
    m_FILE_FAILED_READ = QMessageBox::tr("An error accured while reading the file"),

    m_FILE_ALREADY_USED = QMessageBox::tr("The file has probably been used already"),
    m_FILE_SIZE_WARNING = QMessageBox::tr("The file's size exceeds the warning size"),

    m_DB_NOT_FOUND        = QMessageBox::tr("Failed to find the database file"),
    m_DB_NOT_READABLE     = QMessageBox::tr("The database file is not readable"),
    m_DB_NOT_WRITABLE     = QMessageBox::tr("The database file is not writable"),
    m_DB_FAILED_CREATING  = QMessageBox::tr("An error occured while creating the database"),
    m_DB_FAILED_OPENING   = QMessageBox::tr("An error occured while opening the database"),
    m_DB_FAILED_EXECUTING = QMessageBox::tr("An error occured while executing a statement on the database"),
    m_DB_FAILED_STATEMENT = QMessageBox::tr("Failed at statement"),
    m_DB_FAILED_ERRMSG    = QMessageBox::tr("SQLite error message"),

    m_LOGTYPE_FAILED    = QMessageBox::tr("Failed to determine if Access or Error type file"),
    m_LOGTYPE_UNDEFINED = QMessageBox::tr("This file's LogType is not Access nor Error"),

    // requests
    r_REPORT_ISSUE    = QMessageBox::tr("Please report this issue"),
    r_SET_PERMISSIONS = QMessageBox::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue"),

    // footers
    f_SKIPPING = QMessageBox::tr("Skipping"),
    f_ABORTING = QMessageBox::tr("Aborting")
;


class DialogSec
{
public:
    DialogSec();

    static void warnGeneric( QWidget *parent, const QString& message, const bool report_msg=false );
        static void warnGeneric( QWidget *parent, const std::string& message, const bool report_msg=false );
    static void errGeneric( QWidget *parent, const QString& message, const bool report_msg=false );
        static void errGeneric( QWidget *parent, const std::string& message, const bool report_msg=false );

    // database
    static bool choiceDatabaseNotFound( QWidget *parent, const QString& db_name );
        static bool choiceDatabaseNotFound( QWidget *parent, const std::string& db_name );
    static void errDatabaseNotReadable( QWidget *parent, const QString& db_name );
        static void errDatabaseNotReadable( QWidget *parent, const std::string& db_name );
    static void errDatabaseNotWritable( QWidget *parent, const QString& db_name );
        static void errDatabaseNotWritable( QWidget *parent, const std::string& db_name );
    static void errDatabaseFailedCreating( QWidget *parent, const QString& db_name, const QString& err_msg );
        static void errDatabaseFailedCreating( QWidget *parent, const std::string& db_name, const std::string& err_msg );
    static void errDatabaseFailedOpening( QWidget *parent, const QString& db_name, const QString& err_msg );
        static void errDatabaseFailedOpening( QWidget *parent, const std::string& db_name, const std::string& err_msg );
    static void errDatabaseFailedExecuting( QWidget *parent, const QString& db_name, const QString& statement="", const QString& err_msg="" );
        static void errDatabaseFailedExecuting( QWidget *parent, const std::string& db_name, const std::string& statement="", const std::string& err_msg="" );

    // LogsList
    static bool choiceSelectedFileNotFound( QWidget *parent, const QString& file );
        static bool choiceSelectedFileNotFound( QWidget *parent, const std::string& file );

    // log files type
    static void errFailedDefiningLogType( QWidget *parent, const QString& file );
        static void errFailedDefiningLogType( QWidget *parent, const std::string& file );
    static void errUndefinedLogType( QWidget *parent, const QString& file );
        static void errUndefinedLogType( QWidget *parent, const std::string& file );
    static bool choiceUndefinedLogType( QWidget *parent, const QString& file );
        static bool choiceUndefinedLogType( QWidget *parent, const std::string& file );
    // log files hash
    static int choiceFileAlreadyUsed( QWidget *parent, const QString& msg );
        static int choiceFileAlreadyUsed( QWidget *parent, const std::string& msg );
    // log files size
    static int choiceFileSizeWarning( QWidget *parent, const QString& msg );
        static int choiceFileSizeWarning( QWidget *parent, const std::string& msg );

    // files permissions
    static void errFileNotExists( QWidget *parent );
    static void errFileNotReadable( QWidget *parent );
    static void errFileNotWritable( QWidget *parent );
    static bool choiceFileNotExists( QWidget *parent );
    static bool choiceFileNotReadable( QWidget *parent );
    static bool choiceFileNotWritable( QWidget *parent );
    // files actions
    static void warnEmptyFile( QWidget *parent, const QString& file );
        static void warnEmptyFile( QWidget *parent, const std::string& file );
    static void errFailedReadFile( QWidget *parent, const QString& file, const bool skipping=false );
        static void errFailedReadFile( QWidget *parent, const std::string& file, const bool skipping=false );

    // folders permissions
    static void errDirNotExists( QWidget *parent, const QString& dir );
        static void errDirNotExists( QWidget *parent, const std::string& dir );
    static void warnDirNotReadable( QWidget *parent );
    static void warnDirNotWritable( QWidget *parent );
    static bool choiceDirNotExists( QWidget *parent, const QString& dir );
        static bool choiceDirNotExists( QWidget *parent, const std::string& dir );
    static void choiceDirNotReadable( QWidget *parent );
    static void choiceDirNotWritable( QWidget *parent );

    // generic choices
    static void choiceYesNo( QWidget *parent );
    static bool choiceIgnoreAbort( QWidget *parent );


    // overrides


};

#endif // DIALOGS_H

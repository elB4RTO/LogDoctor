#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QString>


//! DialogSec
/*!
    Implements various dialog models
    \see DialogMsg, DialogBool, DialogDia
*/
class DialogSec : public QObject
{
    Q_OBJECT

public:
    explicit DialogSec();

    /////////////////
    //// GENERIC ////

    /*static void warnGeneric( const QString& message, const bool& report_msg=false, QWidget* parent=nullptr );*/

    //! Generic error message
    static void errGeneric( const QString& message, const bool& report_msg=false, QWidget* parent=nullptr );

    //! Error while renaming
    static void errRenaming( const QString& path, const QString& err, QWidget* parent=nullptr );


    //////////////////
    //// LANGUAGE ////

    //! Invalid locale found
    static void errLangLocaleInvalid( const QString& locale, QWidget* parent=nullptr );

    //! Language not supported
    static void errLangNotAccepted( const QString& locale, QWidget* parent=nullptr );


    ////////////////////////
    //// CONFIGURATIONS ////

    //! Failed to write the configurations on file
    static void errConfFailedWriting( const QString& msg, const QString& err, QWidget* parent=nullptr );

    //! Configuration file not found
    static void warnConfFileNotFound( const QString& file, QWidget* parent=nullptr );

    //! Missing read permission for the configuration file
    static void errConfFileNotReadable( const QString& file, const QString& err, QWidget* parent=nullptr );

    //! Missing write permission for the configuration file
    static void errConfFileNotWritable( const QString& file, const QString& err, QWidget* parent=nullptr );

    //! Missing read permission for the configuration file's directory
    static void errConfDirNotWritable( const QString& dir, const QString& err, QWidget* parent=nullptr );

    //! Missing write permission for the configuration file's directory
    static void errFailedApplyingConfigs( const QString& msg, QWidget* parent=nullptr );


    //////////////
    //// HELP ////

    //! Help resource is not a file
    static void errHelpFailed( const QString& link, const QString& msg, QWidget* parent=nullptr );

    //! Help file not found
    static void errHelpNotFound( const QString& link, QWidget* parent=nullptr );

    //! Missing read permission for the help file
    static void errHelpNotReadable( const QString& link, QWidget* parent=nullptr );


    //////////////////
    //// DATABASE ////

    //! SQLite driver not installed or not found
    static void errSqlDriverNotFound( const QString& driver, QWidget* parent=nullptr );

    //! Database file not found
    static const bool choiceDatabaseNotFound( const QString& db_name, QWidget* parent=nullptr );

    //! A database contains an unexpected table
    static const bool choiceDatabaseWrongTable( const QString& db_name, const QString& table_name, QWidget* parent=nullptr );

    //! A table is missing from the database
    static const bool choiceDatabaseMissingTable( const QString& db_name, const QString& table_name, QWidget* parent=nullptr );

    //! A database contains a table with an unexpected column
    static const bool choiceDatabaseWrongColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent=nullptr );

    //! A column is missing from table of the database
    static const bool choiceDatabaseMissingColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent=nullptr );

    //! A column has an unexpected type parameter
    static const bool choiceDatabaseWrongDataType( const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type, QWidget* parent=nullptr );

    //! The entry corresponding to the database path is not a file
    static void errDatabaseNotFile( const QString& db_name, QWidget* parent=nullptr );

    //! Missing read permission for a database
    static void errDatabaseNotReadable( const QString& db_name, QWidget* parent=nullptr );

    //! Missing write permission for a database
    static void errDatabaseNotWritable( const QString& db_name, QWidget* parent=nullptr );

    //! Successfully created a new database
    static void msgDatabaseCreated( const QString& db_name, QWidget* parent=nullptr );

    //! Failed to create a new database
    static void errDatabaseFailedCreating( const QString& db_name, QWidget* parent=nullptr );

    //! Failed to open a database
    static void errDatabaseFailedOpening( const QString& db_name, const QString& err, QWidget* parent=nullptr );

    //! Failed to execute a statement on a database
    static void errDatabaseFailedExecuting( const QString& db_name, const QString& statement, const QString& err, QWidget* parent=nullptr );

    //! Failed to backup a database
    static void errDatabaseFailedBackup( const QString& msg, const QString& err, QWidget* parent=nullptr );


    ///////////////////
    //// LOGS-LIST ////

    //! A log file which has been selected to be used is no more available
    static const int choiceSelectedFileNotFound( const QString& file, QWidget* parent=nullptr );


    ////////////////////////////
    //// WARNLIST/BLACKLIST ////

    //! An item can't be added to a warnlist/blacklist
    static void warnInvalidItemBW( QWidget* parent=nullptr );


    //////////////////
    //// LOG TYPE ////

    //! The log type can't be defined for a file
    static void errFailedDefiningLogType( const QString& file, QWidget* parent=nullptr );

    ////////////////////
    //// LOG HASHES ////

    //! A log file has already been used
    static const int choiceFileAlreadyUsed( const QString& msg, QWidget* parent=nullptr );

    ///////////////////
    //// LOGS SIZE ////

    //! A log file exceeds the warning size
    static const int choiceFileSizeWarning( const QString& msg, QWidget* parent=nullptr );

    //! A log file exceeds the warning size
    static const bool choiceFileSizeWarning2( const QString& msg, QWidget* parent=nullptr );


    /////////////////////
    //// LOGS FORMAT ////

    //! Failed to process a logs format string
    static void errInvalidLogFormatString( const QString& msg, QWidget* parent=nullptr );

    //! The logs format has not been set for the web server in use
    static void errLogFormatNotSet( QWidget* parent=nullptr );

    //! The log format in use has no log fields
    static void errLogFormatNoFields( QWidget* parent=nullptr );

    //! The log format in use has no log separators
    static void errLogFormatNoSeparators( QWidget* parent=nullptr );


    /////////////////////
    //// LOGS PARSER ////

    //! An error occured while parsing logs
    static void errFailedParsingLogs( const QString& message, QWidget* parent=nullptr );


    ///////////////
    //// FILES ////

    //! File not found
    static void errFileNotFound( const QString& file, const bool& report=false, QWidget* parent=nullptr );

    //! Missing read permission for a file
    static void warnFileNotReadable( const QString& file, QWidget* parent=nullptr );

    //! A file is empty
    static void warnEmptyFile( const QString& file, QWidget* parent=nullptr );

    //! An error occured while reading a file
    static void errFailedReadFile( const QString& file, const bool& skipping=false, QWidget* parent=nullptr );

    /*static void errFailedWriteFile( const QString& file, const bool& skipping=false, QWidget* parent=nullptr );*/


    /////////////////
    //// FOLDERS ////

    /*static const int choiceDirNotExists( const QString& dir, QWidget* parent=nullptr );*/

    //! A directory does not extst
    static void errDirNotExists( const QString& dir, QWidget* parent=nullptr );

    //! Missing read permission for a directory
    static void errDirNotReadable( const QString& dir, const QString& err, QWidget* parent=nullptr );

    //! Missing write permission for a directory
    static void errDirNotWritable( const QString& dir, const QString& err, QWidget* parent=nullptr );

    //! Missing read permission for a directory
    static void warnDirNotReadable( QWidget* parent=nullptr );

    //! Missing write permission for a directory
    static void warnDirNotWritable( QWidget* parent=nullptr );

    //! Failed to create a directory
    static void errFailedMakeDir( const QString& msg, const QString& err, QWidget* parent=nullptr );

    /////////////////
    //// CHOICES ////

    //! An entry was supposed to be a directory but it's not
    static const bool choiceDirNotDir( const QString& path, QWidget* parent=nullptr );

    //! An entry was supposed to be a file but it's not
    static const bool choiceFileNotFile( const QString& path, QWidget* parent=nullptr );



private:

    //! Asks to renew a database
    static const bool choiceDatabaseRenew( const QString& title, const QString& msg, QWidget* parent=nullptr );

};

#endif // DIALOGS_H

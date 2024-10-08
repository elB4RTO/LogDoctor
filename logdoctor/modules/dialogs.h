#ifndef LOGDOCTOR__DIALOGS_H
#define LOGDOCTOR__DIALOGS_H


#include <QWidget>


enum class MsgType;


//! DialogSec
/*!
    Implements various dialog models
    \see DialogMsg, DialogBool, DialogDia
*/
class DialogSec final : public QObject
{
    Q_OBJECT

public:
    explicit DialogSec() = default;

    /////////////////
    //// GENERIC ////

    /*static void warnGeneric( const QString& message, const bool report=false, QWidget* parent=nullptr );*/

    //! Generic error message
    static void errGeneric( const QString& message, const bool report=false, QWidget* parent=nullptr );

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

    //! The path of the configuration file contains a symlink
    static void errConfPathHasSymlink( const QString& full_path, const QString& symlink, QWidget* parent=nullptr );

    //! An error occured while applying the value retrieved from the conf file
    static void errFailedApplyingConfigsItem( const QString& msg, QWidget* parent=nullptr );

    //! The lines in the list caused an error and cannot be applied
    static void warnInvalidConfigsList( const QStringList& list, QWidget* parent=nullptr );

    //! Failed to apply user provided configurations
    static bool choiceFailedApplyingConfigs( const QString& msg, QWidget* parent=nullptr );


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
    static bool choiceDatabaseNotFound( const QString& db_name, QWidget* parent=nullptr );

    //! A database contains an unexpected table
    static bool choiceDatabaseWrongTable( const QString& db_name, const QString& table_name, QWidget* parent=nullptr );

    //! A table is missing from the database
    static bool choiceDatabaseMissingTable( const QString& db_name, const QString& table_name, QWidget* parent=nullptr );

    //! A database contains a table with an unexpected column
    static bool choiceDatabaseWrongColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent=nullptr );

    //! A column is missing from table of the database
    static bool choiceDatabaseMissingColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent=nullptr );

    //! A column has an unexpected type parameter
    static bool choiceDatabaseWrongDataType( const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type, QWidget* parent=nullptr );

    //! The database path does not exists
    static void errDatabaseNotFound( const QString& db_name, QWidget* parent=nullptr );

    //! The entry corresponding to the database path is not a file
    static void errDatabaseNotFile( const QString& db_name, QWidget* parent=nullptr );

    //! Missing read permission for a database
    static void errDatabaseNotReadable( const QString& db_name, QWidget* parent=nullptr );

    //! Missing write permission for a database
    static void errDatabaseNotWritable( const QString& db_name, QWidget* parent=nullptr );

    //! The path of a database file contains a symlink
    static void errDatabasePathHasSymlink( const QString& full_path, const QString& symlink, QWidget* parent=nullptr );

    //! Successfully created a new database
    static void msgDatabaseCreated( const QString& db_name, QWidget* parent=nullptr );

    //! Failed to create a new database
    static void errDatabaseFailedCreating( const QString& db_name, QWidget* parent=nullptr );

    //! Failed to open a database
    static void errDatabaseFailedOpening( const QString& db_name, const QString& err, QWidget* parent=nullptr );

    //! Failed to execute a statement on a database
    static void errDatabaseFailedExecuting( const QString& db_name, const QString& statement, const QString& err, QWidget* parent=nullptr );

    //! Failed to backup a database
    static void errDatabaseFailedBackup( const QString& msg, const QString& err, const bool report=true, QWidget* parent=nullptr );


    ///////////////////
    //// LOGS-LIST ////

    //! A log file which has been selected to be used is no more available
    static int choiceSelectedFileNotFound( const QString& file, QWidget* parent=nullptr );

    //! The list of files to parse is empty
    static void msgNoFileToParse( QWidget* parent=nullptr );


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

    //! A selected log file has already been used
    static int choiceFileAlreadyUsed( const QString& msg, QWidget* parent=nullptr );

    //! A selected log file appears twice in the selection
    static int choiceDuplicateFile( const QString& msg, QWidget* parent=nullptr );

    //! An error occured while inserting the new files' hashes into the daabase
    static void errFailedInsertUsedHashes( QWidget* parent=nullptr );

    ///////////////////
    //// LOGS SIZE ////

    //! The size of a log file which has been selected to be parsed exceeds the warning size
    static int choiceFileSizeWarning( const QString& msg, QWidget* parent=nullptr );

    //! The size of a log file which has been selected to be displayed exceeds the warning size
    static bool choiceFileSizeWarning2( const QString& msg, QWidget* parent=nullptr );

    //! The total size of the selected files to parse is greater than the available free memory of the system
    static void msgNotEnoughMemory( const QString& msg, QWidget* parent=nullptr );


    /////////////////////
    //// LOGS FORMAT ////

    //! Failed to process a logs format string
    static void errInvalidLogFormatString( const QString& msg, QWidget* parent=nullptr );

    //! The log format has not been set for the web server in use
    static void errLogFormatNotSet( QWidget* parent=nullptr );

    //! The log format in use has no log fields
    static void errLogFormatNoFields( QWidget* parent=nullptr );

    //! The log format in use has no log separators
    static void errLogFormatNoSeparators( QWidget* parent=nullptr );

    //! The provided log format is missing an important field
    static bool choiceLogFormatMissingField( const QString& field, QWidget* parent=nullptr );

    //! The provided log format contains the CR control character
    static bool choiceLogFormatWithCarriageReturn( QWidget* parent=nullptr );


    /////////////////////
    //// LOGS PARSER ////

    //! An error occured while parsing logs
    static void errFailedParsingLogs( const QString& message, QWidget* parent=nullptr );


    ////////////////////
    //// STATISTICS ////

    //! An error occured while processing data
    static void errProcessingStatsData( const QString& err, QWidget* parent=nullptr );

    //! No data, cannot draw the chart
    static void msgNoDataForStats( QWidget* parent=nullptr );


    ///////////////
    //// FILES ////

    //! File not found
    static void errFileNotFound( const QString& file, const bool report=false, QWidget* parent=nullptr );

    //! Missing read permission for a file
    static void warnFileNotReadable( const QString& file, QWidget* parent=nullptr );

    //! A file is empty
    static void warnEmptyFile( const QString& file, QWidget* parent=nullptr );

    //! An error occured while reading a file
    static void errFailedReadFile( const QString& file, const bool skipping=false, QWidget* parent=nullptr );

    /*static void errFailedWriteFile( const QString& file, const bool skipping=false, QWidget* parent=nullptr );*/


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


    ///////////////
    //// PATHS ////

    //! A component of the path does not extst
    static void errPathNotExists( const QString& component, const QString& path, QWidget* parent=nullptr );

    //! A component of the path is a symlink
    static void errPathHasSymlink( const QString& component, const QString& path, QWidget* parent=nullptr );


    //////////////
    //// DATA ////

    //! Failed to convert some data from one type into another
    static void errConvertingData( const QString& fromType, const QString& intoType, const QString& value, QWidget* parent=nullptr );


    /////////////////
    //// CHOICES ////

    //! An entry was supposed to be a directory but it's not
    static bool choiceDirNotDir( const QString& path, QWidget* parent=nullptr );

    //! An entry was supposed to be a file but it's not
    static bool choiceFileNotFile( const QString& path, QWidget* parent=nullptr );



private:

    //! Asks to renew a database
    static bool choiceDatabaseRenew( const QString& title, const QString& msg, QWidget* parent=nullptr );

    //! Display an error message about the configuration file/folder/path
    static void errHandlingConfFile( const QString& title, const QString& message, const QString& additional, const MsgType type, QWidget* parent=nullptr );

};


#endif // LOGDOCTOR__DIALOGS_H

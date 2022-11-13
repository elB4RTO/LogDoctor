#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QString>


class DialogSec : public QObject
{
    Q_OBJECT

public:
    DialogSec();

    static void warnGeneric( const QString& message, const bool& report_msg=false, QWidget *parent=nullptr );
    static void errGeneric( const QString& message, const bool& report_msg=false, QWidget *parent=nullptr );

    static void errRenaming( const QString& path, QWidget *parent=nullptr );

    // language
    static void errLangLocaleInvalid( const QString& locale, QWidget *parent=nullptr );
    static void errLangNotAccepted( const QString& locale, QWidget *parent=nullptr );

    // configurations
    static void errConfFailedWriting( const QString& msg="", QWidget *parent=nullptr );
    static void warnConfFileNotFound( const QString& file, QWidget *parent=nullptr );
    static void errConfFileNotReadable( const QString& file, QWidget *parent=nullptr );
    static void errConfFileNotWritable( const QString& file, QWidget *parent=nullptr );
    static void errConfDirNotWritable( const QString& dir, QWidget *parent=nullptr );
    static void errFailedApplyingConfigs( const QString& msg="", QWidget *parent=nullptr );

    // help
    static void errHelpFailed( const QString& link, const QString& msg="", QWidget *parent=nullptr );
    static void errHelpNotFound( const QString& link, QWidget *parent=nullptr );
    static void errHelpNotReadable( const QString& link, QWidget *parent=nullptr );

    // database
    static void errSqlDriverNotFound( const QString& driver, QWidget *parent=nullptr );
    static const bool choiceDatabaseNotFound( const QString& db_name, QWidget *parent=nullptr );
    static const bool choiceDatabaseWrongTable( const QString& db_name, const QString& table_name, QWidget *parent=nullptr );
    static const bool choiceDatabaseMissingTable( const QString& db_name, const QString& table_name, QWidget *parent=nullptr );
    static const bool choiceDatabaseWrongColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget *parent=nullptr );
    static const bool choiceDatabaseMissingColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget *parent=nullptr );
    static const bool choiceDatabaseWrongDataType( const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type, QWidget *parent=nullptr );
    static void errDatabaseNotFile( const QString& db_name, QWidget *parent=nullptr );
    static void errDatabaseNotReadable( const QString& db_name, QWidget *parent=nullptr );
    static void errDatabaseNotWritable( const QString& db_name, QWidget *parent=nullptr );
    static void msgDatabaseCreated( const QString& db_name, QWidget *parent=nullptr );
    static void errDatabaseFailedCreating( const QString& db_name, const QString& err_msg, QWidget *parent=nullptr );
    static void errDatabaseFailedOpening( const QString& db_name, const QString& err_msg, QWidget *parent=nullptr );
    static void errDatabaseFailedExecuting( const QString& db_name, const QString& statement="", const QString& err_msg="", QWidget *parent=nullptr );
    static void errDatabaseFailedBackup( const QString& msg, QWidget *parent=nullptr );

    // LogsList
    static const int choiceSelectedFileNotFound( const QString& file, QWidget *parent=nullptr );

    // Warn/Black lists
    static void warnInvalidItemBW( QWidget *parent=nullptr );

    // log files type
    static void errFailedDefiningLogType( const QString& file, QWidget *parent=nullptr );
    // log files hash
    static const int choiceFileAlreadyUsed( const QString& msg, QWidget *parent=nullptr );
    // log files size
    static const int choiceFileSizeWarning( const QString& msg, QWidget *parent=nullptr );
    static const bool choiceFileSizeWarning2( const QString& msg, QWidget *parent=nullptr );

    // log format string invalid
    static void errInvalidLogFormatString( const QString& msg, QWidget *parent=nullptr );
    // log format not set
    static void errLogFormatNotSet( QWidget *parent=nullptr );
    static void errLogFormatNoFields( QWidget *parent=nullptr );
    static void errLogFormatNoSeparators( QWidget *parent=nullptr );

    // log parser
    static void errFailedParsingLogs( const QString& message, QWidget *parent=nullptr );

    // files
    static void errFileNotFound( const QString& file, const bool& report=false, QWidget *parent=nullptr );
    // files permissions
    static void warnFileNotReadable( const QString& file, QWidget *parent=nullptr );

    // files actions
    static void warnEmptyFile( const QString& file, QWidget *parent=nullptr );
    static void errFailedReadFile( const QString& file, const bool& skipping=false, QWidget *parent=nullptr );
    static void errFailedWriteFile( const QString& file, const bool& skipping=false, QWidget *parent=nullptr );

    // folders permissions
    static const int choiceDirNotExists( const QString& dir, QWidget *parent=nullptr );
    static void errDirNotExists( const QString& dir, QWidget *parent=nullptr );
    static void errDirNotReadable( const QString& dir, QWidget *parent=nullptr );
    static void errDirNotWritable( const QString& dir, QWidget *parent=nullptr );
    static void warnDirNotReadable( QWidget *parent=nullptr );
    static void warnDirNotWritable( QWidget *parent=nullptr );
    // folders actions
    static void errFailedMakeDir( const QString& msg, QWidget *parent=nullptr );

    // generic choices
    static const bool choiceDirNotDir( const QString& path, QWidget *parent=nullptr );
    static const bool choiceFileNotFile( const QString& path, QWidget *parent=nullptr );



private:
    static const bool choiceDatabaseRenew( const QString& title, const QString& msg, QWidget *parent=nullptr );

};

#endif // DIALOGS_H

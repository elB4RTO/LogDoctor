
#include "dialogs.h"

#include "modules/dialogs/message_dialog.h"
#include "modules/dialogs/boolean_dialog.h"
#include "modules/dialogs/ida_dialog.h"


//////////////////
//// LANGUAGE ////
//////////////////
void DialogSec::errLangLocaleInvalid( const QString& locale, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Invalid locale"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("Unexpected locale format"),
            locale,
            DialogSec::tr("If you haven't manually edited the configuration file,\nplease report this issue") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}
void DialogSec::errLangNotAccepted( const QString& locale, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Invalid locale"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The given locale is not an accepted language"),
            locale,
            DialogSec::tr("If you'd like to have this locale in LogDoctor,\nplease follow the instruction on the repository page") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}



///////////////////////
//// CONFIGURATION ////
///////////////////////
void DialogSec::warnConfFileNotFound( const QString& file, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Configuration file not found"),
        QStringLiteral("%1%2\n\n%3").arg(
            DialogSec::tr("Unable to retrieve the configuration file"),
            (file.isEmpty()) ? file : ":\n"+file,
            DialogSec::tr("Skipping") ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errConfFailedWriting( const QString& msg, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed to write the configuration file"),
        QStringLiteral("%1%2\n\n%3").arg(
            DialogSec::tr("An error occured while handling the configuration file"),
            (msg.isEmpty()) ? msg : ":\n"+msg,
            DialogSec::tr("Current configuration not saved") ),
        err, MsgType::Warning, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errConfFileNotReadable( const QString& file, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not readable"),
        QStringLiteral("%1\n\n%2%3").arg(
            DialogSec::tr("An error occured while handling the configuration file"),
            DialogSec::tr("The file is not readable"),
            (file.isEmpty()) ? file : ":\n"+file ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}
void DialogSec::errConfFileNotWritable( const QString& file, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not writable"),
        QStringLiteral("%1\n\n%2%3").arg(
            DialogSec::tr("An error occured while handling the configuration file"),
            DialogSec::tr("The file is not writable"),
            (file.isEmpty()) ? file : ":\n"+file ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}


void DialogSec::errConfDirNotWritable( const QString& dir, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not writable"),
        QStringLiteral("%1\n\n%2%3").arg(
            DialogSec::tr("An error occured while handling the configuration file"),
            DialogSec::tr("The directory is not writable"),
            (dir.isEmpty()) ? dir : ":\n"+dir ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}


void DialogSec::errFailedApplyingConfigsItem( const QString& msg, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed applying configuration"),
        QStringLiteral("%1\n%2").arg(
            (msg.isEmpty()) ? msg : QString("%1\n").arg(msg),
            DialogSec::tr("Skipping") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}


void DialogSec::warnInvalidConfigsList( const QStringList& list, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Invalid configuration lines"),
        QStringLiteral("%1\n%2").arg(
            DialogSec::tr("Has not been possible to apply some of the configurations"),
            DialogSec::tr("If you haven't manually edited the configuration file,\nplease report this issue") ),
        list.join('\n'),
        MsgType::Warning, parent };
    std::ignore = dialog.exec();
}


bool DialogSec::choiceFailedApplyingConfigs( const QString& msg, QWidget* parent )
{
    DialogBool dialog{
        DialogSec::tr("Failed applying configuration"),
        QStringLiteral("%1\n\n%2").arg(
            msg,
            DialogSec::tr("If you choose to proceed, all of the unapplied configurations will be lost\nContinue?") ),
        parent };
    return dialog.exec();
}



//////////////
//// HELP ////
//////////////
void DialogSec::errHelpFailed( const QString& link, const QString& msg, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed to retrieve the help file"),
        QStringLiteral("%1%2\n\n%3%4").arg(
            DialogSec::tr("An error occured while getting the help file"),
            (msg.isEmpty()) ? msg : ":\n"+msg,
            DialogSec::tr("Additional resources can be downloaded from the git repo"),
            (link.isEmpty()) ? link : ":\n"+link ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errHelpNotFound( const QString& link, QWidget* parent )
{
    DialogSec::errHelpFailed(
        link,
        DialogSec::tr("Unable to retrieve the file").toLower(),
        parent );
}

void DialogSec::errHelpNotReadable( const QString& link, QWidget* parent )
{
    DialogSec::errHelpFailed(
        link,
        DialogSec::tr("The file is not readable").toLower(),
        parent );
}



//////////////////////////
//// WARN/BLACK LISTS ////
//////////////////////////
void DialogSec::warnInvalidItemBW( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Invalid string"),
        DialogSec::tr("The given string is invalid and cannot be added to the list\n\nPlease correct it and retry"),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}



//////////////////
//// DATABASE ////
//////////////////
void DialogSec::errSqlDriverNotFound( const QString& driver, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("QSql driver not found"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("Failed to retrieve the driver needed to handle the database"),
            driver,
            DialogSec::tr("Aborting") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

bool DialogSec::choiceDatabaseNotFound( const QString& db_name, QWidget* parent )
{
    DialogBool dialog{
        DialogSec::tr("File not found"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("Failed to retrieve the database file"),
            db_name,
            DialogSec::tr("Create a new database?") ),
        parent };
    return dialog.exec();
}

bool DialogSec::choiceDatabaseWrongTable(const QString& db_name, const QString& table_name, QWidget* parent )
{
    const QString msg{ QStringLiteral("%1:\n%2 -> %3").arg(
        DialogSec::tr("The database contains an unexpected table"),
        db_name, table_name ) };
    return choiceDatabaseRenew(
        DialogSec::tr("Unexpected table"),
        msg,
        parent );
}
bool DialogSec::choiceDatabaseMissingTable( const QString& db_name, const QString& table_name, QWidget* parent )
{
    QString msg{ QStringLiteral("%1:\n%2 -> %3").arg(
        DialogSec::tr("It seems that the database is missing a table"),
        db_name, table_name ) };
    return choiceDatabaseRenew(
        DialogSec::tr("Table not found"),
        msg,
        parent );
}
bool DialogSec::choiceDatabaseWrongColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent )
{
    QString msg{ QStringLiteral("%1:\n%2 -> %3 -> %4").arg(
        DialogSec::tr("The database contains an unexpected column"),
        db_name, table_name, column_name ) };
    return choiceDatabaseRenew(
        DialogSec::tr("Unexpected column"),
        msg,
        parent );
}
bool DialogSec::choiceDatabaseMissingColumn( const QString& db_name, const QString& table_name, const QString& column_name, QWidget* parent )
{
    QString msg{ QStringLiteral("%1:\n%2 -> %3 -> %4").arg(
        DialogSec::tr("It seems that the table is missing a column"),
        db_name, table_name, column_name ) };
    return choiceDatabaseRenew(
        DialogSec::tr("Column not found"),
        msg,
        parent );
}
bool DialogSec::choiceDatabaseWrongDataType( const QString& db_name, const QString& table_name, const QString& column_name, const QString& data_type, QWidget* parent )
{
    QString msg{ QStringLiteral("%1:\n%2 -> %3 -> %4 -> %5").arg(
        DialogSec::tr("A column has an unexpected data-type"),
        db_name, table_name, column_name, data_type ) };
    return choiceDatabaseRenew(
        DialogSec::tr("Unexpected data-type"),
        msg,
        parent );
}
bool DialogSec::choiceDatabaseRenew( const QString& title, const QString& msg, QWidget* parent )
{
    DialogBool dialog{
        title,
        QStringLiteral("%1\n\n%2").arg(
            msg,
            DialogSec::tr("This database will be renamed with a trailing '.copy' and a new one will be created.\nContinue?") ),
        parent };
    return dialog.exec();
}
void DialogSec::msgDatabaseCreated( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Database created"),
        QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("Successfully created a new database"),
            db_name ),
        QString(), MsgType::Info, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseNotFound( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not found"),
        QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("Failed to retrieve the database file"),
            db_name ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseNotFile( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Not a file"),
        QStringLiteral("%1\n\n%2:\n%3\n\n%4").arg(
            DialogSec::tr("Failed to retrieve the database file"),
            DialogSec::tr("The path was supposed to point to a file, but it doesn't"),
            db_name,
            DialogSec::tr("Please remove the conflict and retry") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseNotReadable( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not readable"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The database file is not readable"),
            db_name,
            DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseNotWritable( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not writable"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The database file is not writable"),
            db_name,
            DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseFailedCreating( const QString& db_name, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed creating database"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("An error occured while creating the database"),
            db_name,
            DialogSec::tr("Aborting") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseFailedOpening( const QString& db_name, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed opening database"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("An error occured while opening the database"),
            db_name,
            DialogSec::tr("Aborting") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseFailedExecuting( const QString& db_name, const QString& statement, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed executing on database"),
        QStringLiteral("%1:\n%2%3\n\n%4").arg(
            DialogSec::tr("An error occured while executing a statement on the database"),
            db_name,
            !statement.isEmpty() ? "\n"+statement : QString(),
            DialogSec::tr("Aborting") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDatabaseFailedBackup( const QString& msg, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed to backup database"),
        QStringLiteral("%1\n\n%2").arg(
            msg,
            DialogSec::tr("Please report this issue") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}



/////////////////////
//// LOGS FORMAT ////
/////////////////////
void DialogSec::errInvalidLogFormatString( const QString& msg, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Invalid log format string"),
        DialogSec::tr("Please check that no error is thrown by your WebServer\nIf it gets accepted, please check the presence of a typo here\nIf everything is fine, please report this issue"),
        msg, MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errLogFormatNotSet( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Log format error"),
        DialogSec::tr("The log format has not been set, or is invalid\nPlease add a valid one in the configurations"),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errLogFormatNoFields( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Misconfigured log format"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("No log field has been set in the current logs format,\nmaking it useless to parse logs"),
            DialogSec::tr("Please set up a format which contains at least one field") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errLogFormatNoSeparators( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Misconfigured log format"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("A separator is missing between one or more fields,\nmaking it hard to establish net bounds,\nand possibly leading to store incorrect data"),
            DialogSec::tr("Please set up a format which contains separators between fields") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errFailedParsingLogs( const QString& message, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("An error occured while parsing logs"),
        message,
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}



///////////////////
//// LOG FILES ////
///////////////////
void DialogSec::errFailedDefiningLogType( const QString& file, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed defining type"),
        QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("Failed to determine the log type"),
            file ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}


int DialogSec::choiceFileAlreadyUsed( const QString& msg, QWidget* parent )
{
    DialogIda dialog{
        DialogSec::tr("File already used"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file has probably been used already"),
            msg,
            DialogSec::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort the entire process?") ),
        true, true, true, parent };
    return dialog.exec();
}


int DialogSec::choiceDuplicateFile( const QString& msg, QWidget* parent )
{
    DialogIda dialog{
        DialogSec::tr("Duplicate file"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file appears twice in the list of selections"),
            msg,
            DialogSec::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort the entire process?") ),
        true, true, true, parent };
    return dialog.exec();
}

void DialogSec::errFailedInsertUsedHashes( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed updating hashes"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("An error occured while inserting the parsed files hashes into the database"),
            DialogSec::tr("Aborting") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}


int DialogSec::choiceFileSizeWarning( const QString& msg, QWidget* parent )
{
    DialogIda dialog{
        DialogSec::tr("File exceeds warning size"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file's size exceeds the warning size"),
            msg,
            DialogSec::tr("Ignore the warning and use it anyway, Discard it and continue, or Abort the entire process?") ),
        true, true, true, parent };
    return dialog.exec();
}
bool DialogSec::choiceFileSizeWarning2( const QString& msg, QWidget* parent )
{
    DialogBool dialog{
        DialogSec::tr("File exceeds warning size"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file's size exceeds the warning size"),
            msg,
            DialogSec::tr("Proceed anyway?") ),
        parent };
    return dialog.exec();
}

void DialogSec::msgNotEnoughMemory( const QString& msg, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Not enough memory"),
        QStringLiteral("%1%2\n\n%3").arg(
            DialogSec::tr("The total size of the selected files exceeds the available memory"),
            msg,
            DialogSec::tr("Please free some resources, parse the files in different steps or split them into smaller units") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}


void DialogSec::errFailedReadFile( const QString& file , const bool skipping, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed reading"),
        QStringLiteral("%1:\n%2%3").arg(
            DialogSec::tr("An error occured while reading the file"),
            file,
            skipping ? DialogSec::tr("Skipping").prepend("\n\n") : QString() ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

/*void DialogSec::errFailedWriteFile( const QString& file , const bool skipping, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed writing"),
        QStringLiteral("%1:\n%2%3").arg(
            DialogSec::tr("An error occured while writing the file"),
            file,
            (skipping) ? QString("\n\n%1").arg(DialogSec::tr("Skipping")) : QString() ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}*/


void DialogSec::warnEmptyFile( const QString& file, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File is empty"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file is blank"),
            file,
            DialogSec::tr("Skipping") ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errFileNotFound( const QString& file , const bool report, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not found"),
        QStringLiteral("%1:\n%2%3").arg(
            DialogSec::tr("Unable to retrieve the file"),
            file,
            report ? DialogSec::tr("Please report this issue").prepend("\n\n") : QString() ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::warnFileNotReadable( const QString& file, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("File not readable"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The file is not readable"),
            file,
            DialogSec::tr("Skipping") ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}



int DialogSec::choiceSelectedFileNotFound( const QString& file, QWidget* parent )
{
    DialogIda dialog{
        DialogSec::tr("File not found"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("Failed to retrieve the selected file"),
            file,
            DialogSec::tr("Discard it and continue, or Abort all and exit?") ),
        false, true, true, parent };
    return dialog.exec();
}

void DialogSec::msgNoFileToParse( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("No file to parse"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("The list of files to parse is empty"),
            DialogSec::tr("Aborting") ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}



/////////////////////
//// PERMISSIONS ////
/////////////////////
void DialogSec::errDirNotExists( const QString& dir, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not found"),
        QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("The directory does not exists"),
            dir ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}

/*const int DialogSec::choiceDirNotExists( const QString& dir, QWidget* parent )
{
    DialogDia dialog{
        DialogSec::tr("Directory not found"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The directory does not exists"),
            dir,
            DialogSec::tr("Discard it and continue, or Abort all and exit?") ),
        false, true, true, parent };
    return dialog.exec();
}*/


void DialogSec::errDirNotReadable( const QString& dir, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not readable"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The directory is not readable"),
            dir,
            DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}
void DialogSec::warnDirNotReadable( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not readable"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("The directory is not readable"),
            DialogSec::tr("Please set the proper permissions before to start") ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errDirNotWritable( const QString& dir, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not writable"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The directory is not writable"),
            dir,
            DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}
void DialogSec::warnDirNotWritable( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Directory not writable"),
        QStringLiteral("%1\n\n%2").arg(
            DialogSec::tr("The directory is not writable"),
            DialogSec::tr("Please set the proper permissions before to start") ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}

void DialogSec::errFailedMakeDir( const QString& msg, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed creating directory"),
        QStringLiteral("%1\n\n%2").arg(
            msg,
            DialogSec::tr("Please set the proper permissions and retry\nIf this error persists, please report this issue") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}


//////////////
//// DATA ////
//////////////
void DialogSec::errConvertingData( const QString& fromType, const QString& intoType, const QString& value, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Data conversion failed"),
        QStringLiteral("%1:\n%2").arg(
            DialogSec::tr("Failed to convert from '%1' to '%2'").arg(
                fromType, intoType ),
            value ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}


////////////////////
//// STATISTICS ////
////////////////////
void DialogSec::errProcessingStatsData( const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed to create statistics"),
        DialogSec::tr("An error occured while processing"),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}

void DialogSec::msgNoDataForStats( QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Cannot create statistics"),
        DialogSec::tr("No data has been found that matches with the currently set parameters"),
        QString(), MsgType::Info, parent };
    std::ignore = dialog.exec();
}


//////////////////
//// GENERICS ////
//////////////////
bool DialogSec::choiceDirNotDir( const QString& path, QWidget* parent )
{
    DialogBool dialog{
        DialogSec::tr("Not a folder"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The path was supposed to point to a folder, but it doesn't"),
            path,
            DialogSec::tr("The entry will be renamed with a trailing '.copy' and a new one will be created.\nContinue?") ),
        parent };
    return dialog.exec();
}
bool DialogSec::choiceFileNotFile( const QString& path, QWidget* parent )
{
    DialogBool dialog{
        DialogSec::tr("Not a file"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("The path was supposed to point to a file, but it doesn't"),
            path,
            DialogSec::tr("The entry will be renamed with a trailing '.copy' and a new one will be created.\nContinue?") ),
        parent };
    return dialog.exec();
}



/*void DialogSec::warnGeneric( const QString& msg, const bool report_msg, QWidget* parent )
{
    QString footer;
    if ( report_msg ) {
        footer += "\n\n" + DialogSec::tr("Please report this issue");
    }
    DialogMsg dialog{
        DialogSec::tr("An error occured"),
        QStringLiteral("%1%2").arg(
            msg, footer ),
        QString(), MsgType::Warning, parent };
    std::ignore = dialog.exec();
}*/



void DialogSec::errGeneric( const QString& msg, const bool report_msg, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("An error occured"),
        QStringLiteral("%1%2").arg(
            msg,
            report_msg ? DialogSec::tr("Please report this issue").prepend("\n\n") : QString() ),
        QString(), MsgType::Error, parent };
    std::ignore = dialog.exec();
}



void DialogSec::errRenaming( const QString& path, const QString& err, QWidget* parent )
{
    DialogMsg dialog{
        DialogSec::tr("Failed renaming"),
        QStringLiteral("%1:\n%2\n\n%3").arg(
            DialogSec::tr("An error occured while renaming"),
            path,
            DialogSec::tr("Aborting") ),
        err, MsgType::Error, parent };
    std::ignore = dialog.exec();
}

#ifndef DIALOGS_H
#define DIALOGS_H

#include "qwidget.h"
#include "qstring.h"
#include "qmessagebox.h"


static QString
    // questions
    q_CONTINUE      = QMessageBox::tr("Continue?"),
    q_YOUR_CHOICE   = QMessageBox::tr("Your choice?"),
    q_DA            = QMessageBox::tr("Discard it and continue, or Abort all and exit?"),
    q_DIA           = QMessageBox::tr("Ignore and use it anyway, Discard it and continue, or Abort all and exit?"),

    // titles
    t_ERROR_OCCURED   = QMessageBox::tr("An error occured"),

    t_LOGTYPE_FAILED  = QMessageBox::tr("Failed defining type"),
    t_LOGTYPE_UNDEFINED  = QMessageBox::tr("Undefined type"),

    t_FILE_ALREADY_USED  = QMessageBox::tr("File already used"),

    t_FILE_NOT_FOUND    = QMessageBox::tr("File not found"),
    t_FILE_FAILED_READ  = QMessageBox::tr("Failed reading"),

    t_DIR_NOT_FOUND   = QMessageBox::tr("Directory not found"),

    // messages
    m_SELECTED_FILE_NOT_FOUND = QMessageBox::tr("Failed to retrieve the selected file"),
    m_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the file"),
    m_DIR_NOT_FOUND  = QMessageBox::tr("Unable to reach the directory"),

    m_FILE_NOT_EXISTS = QMessageBox::tr("The file does not exists"),
    m_DIR_NOT_EXISTS  = QMessageBox::tr("The directory does not exists"),

    m_FILE_FAILED_READ  = QMessageBox::tr("An error accured while reading the file"),

    m_FILE_ALREADY_USED  = QMessageBox::tr("The file has probably been used already"),

    m_LOGTYPE_FAILED  = QMessageBox::tr("Failed to determine if Access or Error type file"),
    m_LOGTYPE_UNDEFINED  = QMessageBox::tr("This file's LogType is not Access nor Error"),

    // requests
    r_REPORT_ISSUE = QMessageBox::tr("Please report this issue"),

    // footers
    f_SKIPPING = QMessageBox::tr("Skipping")
;


class DialogSec
{
public:
    DialogSec();

    static void msgGenericError( QWidget *parent, const QString& message );
        static void msgGenericError( QWidget *parent, const std::string& message );

    // LogsList
    static bool choiceSelectedFileNotFound( QWidget *parent, const QString& file );
        static bool choiceSelectedFileNotFound( QWidget *parent, const std::string& file );

    // log files type
    static void msgFailedDefineLogType( QWidget *parent, const QString& file );
        static void msgFailedDefineLogType( QWidget *parent, const std::string& file );
    static void msgUndefinedLogType( QWidget *parent, const QString& file );
        static void msgUndefinedLogType( QWidget *parent, const std::string& file );
    static bool choiceUndefinedLogType( QWidget *parent, const QString& file );
        static bool choiceUndefinedLogType( QWidget *parent, const std::string& file );
    // log files hash
    static int choiceFileAlreadyUsed( QWidget *parent, const QString& file );
        static int choiceFileAlreadyUsed( QWidget *parent, const std::string& file );

    // files permissions
    static void msgFileNotExists( QWidget *parent );
    static void msgFileNotReadabl( QWidget *parent );
    static void msgFileNotWritable( QWidget *parent );
    static bool choiceFileNotExists( QWidget *parent );
    static bool choiceFileNotReadable( QWidget *parent );
    static bool choiceFileNotWritable( QWidget *parent );
    // files actions
    static void msgFailedReadFile( QWidget *parent, const QString& file, const bool skipping=false );
        static void msgFailedReadFile( QWidget *parent, const std::string& file, const bool skipping=false );

    // folders permissions
    static void msgDirNotExists( QWidget *parent, const QString& dir );
        static void msgDirNotExists( QWidget *parent, const std::string& dir );
    static void msgDirNotReadable( QWidget *parent );
    static void msgDirNotWritable( QWidget *parent );
    static bool choiceDirNotExists( QWidget *parent, const QString& dir );
        static bool choiceDirNotExists( QWidget *parent, const std::string& dir );
    static void choiceDirNotReadable( QWidget *parent );
    static void choiceDirNotWritable( QWidget *parent );

    // choices
    static void choiceYesNo( QWidget *parent );
    static bool choiceIgnoreAbort( QWidget *parent );


    // overrides


};

#endif // DIALOGS_H

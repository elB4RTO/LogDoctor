#ifndef DIALOGS_H
#define DIALOGS_H

#include "qwidget.h"
#include "qstring.h"
#include "qmessagebox.h"


static QString
    // questions
    q_CONTINUE    = QMessageBox::tr("Continue?"),
    q_YOUR_CHOICE = QMessageBox::tr("Your choice?"),

    // titles
    t_ERROR_OCCURED   = QMessageBox::tr("An error occured"),
    t_FILE_NOT_FOUND  = QMessageBox::tr("File not found"),
    t_DIR_NOT_FOUND   = QMessageBox::tr("Directory not found"),

    // messages
    m_SELECTED_FILE_NOT_FOUND = QMessageBox::tr("Failed to retrieve the selected file"),
    m_FILE_NOT_FOUND = QMessageBox::tr("Unable to retrieve the file"),
    m_DIR_NOT_FOUND  = QMessageBox::tr("Unable to reach the directory"),

    // requests
    r_REPORT_ISSUE = QMessageBox::tr("Please report this issue")
;


class Dialogs
{
public:
    Dialogs();

    static void msgGenericError( QWidget *parent, const QString& message );
        static void msgGenericError( QWidget *parent, const std::string& message );

    // LogsList
    static bool choiceSelectedFileNotFound( QWidget *parent, const QString& file );
        static bool choiceSelectedFileNotFound( QWidget *parent, const std::string& file );

    // files
    static void msgFileNotExists( QWidget *parent );
    static void msgFileNotReadabl( QWidget *parent );
    static void msgFileNotWritable( QWidget *parent );
    static bool choiceFileNotExists( QWidget *parent );
    static bool choiceFileNotReadable( QWidget *parent );
    static bool choiceFileNotWritable( QWidget *parent );
    // folders
    static void msgDirNotExists( QWidget *parent, const QString& dir );
        static void msgDirNotExists( QWidget *parent, const std::string& dir );
    static void msgDirNotReadable( QWidget *parent );
    static void msgDirNotWritable( QWidget *parent );
    static void choiceDirNotExists( QWidget *parent );
    static void choiceDirNotReadable( QWidget *parent );
    static void choiceDirNotWritable( QWidget *parent );

    // choices
    static void choiceYesNo( QWidget *parent );
    static bool choiceIgnoreAbort( QWidget *parent );


    // overrides


};

#endif // DIALOGS_H

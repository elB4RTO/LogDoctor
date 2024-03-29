#ifndef LOGDOCTOR__DIALOGS__MESSAGE_DIALOG_H
#define LOGDOCTOR__DIALOGS__MESSAGE_DIALOG_H


#include <QDialog>


enum class MsgType {
    Info    = 0,
    Warning = 1,
    Error   = 2
};


namespace Ui {
    class DialogMsg;
}


//! DialogMsg
/*!
    A dialog message to provide informations to the user
*/
class DialogMsg final : public QDialog
{
    Q_OBJECT

public:

    /*!
        \param title The title
        \param text The message
        \param additional Additional informations, usually an error message
        \param parent The parent Widget
    */
    explicit DialogMsg( const QString& title, const QString& text, const QString& additional, const MsgType type, QWidget *parent=nullptr );
    Q_DISABLE_COPY_MOVE(DialogMsg)

private slots:

    //! Shows the additional informations
    void on_button_ShowAdditional_clicked();

    //! Called when the OK button gets clicked
    void on_button_Ok_clicked();

private:
    QSharedPointer<Ui::DialogMsg> ui;

    bool additional_shown{ false };

    int initial_height    { 0 };
    int additional_height { 0 };
};


#endif // LOGDOCTOR__DIALOGS__MESSAGE_DIALOG_H

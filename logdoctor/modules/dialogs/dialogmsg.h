#ifndef DIALOGMSG_H
#define DIALOGMSG_H

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
class DialogMsg : public QDialog
{
    Q_OBJECT

public:

    //! Dialog constructor
    /*!
        \param title The title
        \param text The message
        \param additional Additional informations, usually an error message
        \param parent The parent Widget
    */
    explicit DialogMsg( const QString& title, const QString& text, const QString& additional, const MsgType type, QWidget *parent=nullptr );

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

#endif // DIALOGMSG_H

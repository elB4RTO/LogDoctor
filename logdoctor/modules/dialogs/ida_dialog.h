#ifndef LOGDOCTOR__DIALOGS__IDA_DIALOG_H
#define LOGDOCTOR__DIALOGS__IDA_DIALOG_H


#include <QDialog>


namespace Ui {
    class DialogIda;
}


//! DialogIda
/*!
    IGNORE/DISCARD/ABORT choice dialog
*/
class DialogIda final : public QDialog
{
    Q_OBJECT

public:

    /*!
        \param title The title
        \param text The message
        \param ignore Whether to show the IGNORE button or not
        \param discard Whether to show the DISCARD button or not
        \param abort Whether to show the ABORT button or not
        \param parent The parent Widget
    */
    explicit DialogIda( const QString& title, const QString& text, const bool ignore=true, const bool discard=true, const bool abort=true, QWidget* parent=nullptr );
    Q_DISABLE_COPY_MOVE(DialogIda)

private slots:

    //! Called when the IGNORE button gets clicked
    void on_button_IGNORE_clicked();

    //! Called when the DISCARD button gets clicked
    void on_button_DISCARD_clicked();

    //! Called when the ABORT button gets clicked
    void on_button_ABORT_clicked();

private:
    QSharedPointer<Ui::DialogIda> ui;
};


#endif // LOGDOCTOR__DIALOGS__IDA_DIALOG_H

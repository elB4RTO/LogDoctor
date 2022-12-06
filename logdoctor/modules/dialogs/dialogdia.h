#ifndef DIALOGDIA_H
#define DIALOGDIA_H

#include <QDialog>


namespace Ui {
    class DialogDia;
}

//! DialogDia
/*!
    DISCARD/IGNORE/ABORT choice dialog
*/
class DialogDia : public QDialog
{
    Q_OBJECT

public:

    //! Dialog constructor
    /*!
        \param title The title
        \param text The message
        \param ignore Whether to show the IGNORE button or not
        \param discard Whether to show the DISCARD button or not
        \param abort Whether to show the ABORT button or not
        \param parent The parent Widget
    */
    DialogDia( const QString& title, const QString& text, const bool& ignore=true, const bool& discard=true, const bool& abort=true, QWidget *parent=nullptr );
    ~DialogDia();

private slots:

    //! Called when the IGNORE button gets clicked
    void on_button_IGNORE_clicked();

    //! Called when the DISCARD button gets clicked
    void on_button_DISCARD_clicked();

    //! Called when the ABORT button gets clicked
    void on_button_ABORT_clicked();

private:
    Ui::DialogDia *ui;
};

#endif // DIALOGDIA_H

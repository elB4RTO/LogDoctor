#ifndef LOGDOCTOR__DIALOGS__BOOLEAN_DIALOG_H
#define LOGDOCTOR__DIALOGS__BOOLEAN_DIALOG_H


#include <QDialog>


namespace Ui {
    class DialogBool;
}

//! DialogBool
/*!
    YES/NO choice dialog
*/
class DialogBool final : public QDialog
{
    Q_OBJECT

public:

    //! Dialog constructor
    /*!
        \param title The title
        \param text The message
        \param parent The parent Widget
    */
    explicit DialogBool( const QString& title, const QString& text, QWidget *parent=nullptr );

private slots:

    //! Called when the NO button gets clicked
    void on_button_NO_clicked();

    //! Called when the YES button gets clicked
    void on_button_YES_clicked();

private:
    QSharedPointer<Ui::DialogBool> ui;
};


#endif // LOGDOCTOR__DIALOGS__BOOLEAN_DIALOG_H

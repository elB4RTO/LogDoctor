#ifndef DIALOGBOOL_H
#define DIALOGBOOL_H

#include <QDialog>


namespace Ui {
    class DialogBool;
}

//! DialogBool
/*!
    YES/NO choice dialog
*/
class DialogBool : public QDialog
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
    ~DialogBool();

private slots:

    //! Called when the NO button gets clicked
    void on_button_NO_clicked();

    //! Called when the YES button gets clicked
    void on_button_YES_clicked();

private:
    Ui::DialogBool* ui;
};

#endif // DIALOGBOOL_H

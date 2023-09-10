#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>


namespace Ui {
    class GameDialog;
}

//! GameDialog
/*!
    A dialog message to provide informations to the user
*/
class GameDialog : public QDialog
{
    Q_OBJECT

public:

    //! Dialog constructor
    /*!
        \param title The title
        \param text The message
        \param parent The parent Widget
    */
    explicit GameDialog( const QString& title, const QString& text, QWidget *parent=nullptr );

private slots:

    //! Called when the OK button gets clicked
    void on_button_Ok_clicked();

private:
    QSharedPointer<Ui::GameDialog> ui;
};

#endif // GAMEDIALOG_H

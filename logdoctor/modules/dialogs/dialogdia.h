#ifndef DIALOGDIA_H
#define DIALOGDIA_H

#include <QDialog>


namespace Ui {
    class DialogDia;
}

class DialogDia : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDia( const QString& title, const QString& text, const bool& ignore=true, const bool& discard=true, const bool& abort=true, QWidget *parent=nullptr );
    ~DialogDia();

private slots:
    void on_button_IGNORE_clicked();

    void on_button_DISCARD_clicked();

    void on_button_ABORT_clicked();

private:
    Ui::DialogDia *ui;
};

#endif // DIALOGDIA_H

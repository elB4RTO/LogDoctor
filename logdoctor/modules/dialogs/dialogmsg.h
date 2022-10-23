#ifndef DIALOGMSG_H
#define DIALOGMSG_H

#include <QDialog>


namespace Ui {
    class DialogMsg;
}

class DialogMsg : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMsg( const QString& title, const QString& text, const QString& additional, const int& type, QWidget *parent=nullptr );
    ~DialogMsg();

private slots:
    void on_button_ShowAdditional_clicked();

    void on_button_Ok_clicked();

private:
    Ui::DialogMsg *ui;

    bool additional_shown = false;

    int initial_height    = 0;
    int additional_height = 0;
};

#endif // DIALOGMSG_H

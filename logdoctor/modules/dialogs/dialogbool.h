#ifndef DIALOGBOOL_H
#define DIALOGBOOL_H

#include <QDialog>


namespace Ui {
    class DialogBool;
}

class DialogBool : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBool( const QString& title, const QString& text, QWidget *parent=nullptr );
    ~DialogBool();

private slots:
    void on_button_NO_clicked();

    void on_button_YES_clicked();

private:
    Ui::DialogBool *ui;
};

#endif // DIALOGBOOL_H

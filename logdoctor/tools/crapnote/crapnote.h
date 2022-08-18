#ifndef CRAPNOTE_H
#define CRAPNOTE_H

#include <QWidget>

namespace Ui {
class Crapnote;
}

class Crapnote : public QWidget
{
    Q_OBJECT

public:
    explicit Crapnote(QWidget *parent = nullptr);
    ~Crapnote();

    void setTextFont( const QFont& font );
    void setColorScheme( const int& color_scheme_id );

private slots:
    void on_spinBox_FontSize_valueChanged(int arg1);

    void on_button_FontSize_Minus_clicked();

    void on_button_FontSize_Plus_clicked();

private:
    Ui::Crapnote *ui;

    int font_size = 17;
};

#endif // CRAPNOTE_H

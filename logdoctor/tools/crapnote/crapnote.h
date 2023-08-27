#ifndef CRAPNOTE_H
#define CRAPNOTE_H

#include <QWidget>


namespace Ui {
class Crapnote;
}

//! Crapnote
/*!
    A simple block-note like widget
*/
class Crapnote : public QWidget
{
    Q_OBJECT

public:
    explicit Crapnote( QWidget* parent=nullptr );

    //! Sets the given font
    void setTextFont( const QFont& font );

    //! Sets the given color-scheme
    void setColorScheme( const int& color_scheme_id );


private slots:

    //! Sets the given font size
    void on_spinBox_FontSize_valueChanged(int arg1);

    //! Decrease the font size of 1 point
    void on_button_FontSize_Minus_clicked();

    //! Increase the font size of 1 point
    void on_button_FontSize_Plus_clicked();


private:
    QSharedPointer<Ui::Crapnote> ui;

    int font_size{ 17 };
};

#endif // CRAPNOTE_H

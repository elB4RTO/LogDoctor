#ifndef LOGDOCTOR__CRAPNOTE_H
#define LOGDOCTOR__CRAPNOTE_H


#include <QWidget>


enum class ColorsScheme : unsigned char;


namespace Ui {
    class Crapnote;
}


//! Crapnote
/*!
    A simple block-note like widget
*/
class Crapnote final : public QWidget
{
    Q_OBJECT

public:
    explicit Crapnote( const ColorsScheme colors_scheme, QFont font, QWidget* parent=nullptr );

    //! Sets the given font
    void setTextFont( QFont font ) noexcept;

    //! Sets the given color-scheme
    void setColorScheme( const ColorsScheme colors_scheme );


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


#endif // LOGDOCTOR__CRAPNOTE_H

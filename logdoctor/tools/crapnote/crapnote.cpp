
#include "crapnote.h"
#include "ui_crapnote.h"

#include "globals/global_configs.h"

#include "modules/stylesheets.h"

#include "modules/exceptions.h"


Crapnote::Crapnote( const int color_scheme_id, QFont font, QWidget* parent )
    : QWidget{ parent }
    , ui{ new Ui::Crapnote }
{
    ui->setupUi(this);

    this->setColorScheme( color_scheme_id );
    this->setTextFont( font );
}


void Crapnote::setTextFont( QFont font ) noexcept
{
    font.setPointSize( this->font_size );
    this->ui->text_Note->setFont( font );
}

void Crapnote::setColorScheme( const int color_scheme_id )
{
    if ( GlobalConfigs::window_theme != WindowTheme::Native ) {
        this->setStyleSheet( StyleSec::Crapnote::getStyleSheet( color_scheme_id ) );
    } else {
        this->setStyleSheet("");
        QPalette p;
        // update the colors palette
        switch ( color_scheme_id ) {
            case 0:
                break;
            case 1:
                // breeze
                p.setColor( QPalette::Base, QColor( 255, 198, 102 ) );
                p.setColor( QPalette::Text, QColor(  31,  28,  27 ) );
                break;
            case 2:
                // monokai
                p.setColor( QPalette::Base, QColor( 166, 226,  46 ) );
                p.setColor( QPalette::Text, QColor(  39,  40,  34 ) );
                break;
            case 3:
                // radical
                p.setColor( QPalette::Base, QColor(  20,  19,  34 ) );
                p.setColor( QPalette::Text, QColor( 213,  53, 143 ) );
                break;
            default:
                // wrong
                throw GenericException( "Unexpected ColorScheme ID for Crapnote: "+std::to_string( color_scheme_id ), true ); // leave un-catched
        }
        this->ui->text_Note->setPalette( p );
    }
}


void Crapnote::on_spinBox_FontSize_valueChanged(int arg1)
{
    this->font_size = arg1;
    this->setTextFont( this->ui->text_Note->font() );
}


void Crapnote::on_button_FontSize_Minus_clicked()
{
    this->ui->spinBox_FontSize->setValue( this->font_size-1 );
}


void Crapnote::on_button_FontSize_Plus_clicked()
{
    this->ui->spinBox_FontSize->setValue( this->font_size+1 );
}

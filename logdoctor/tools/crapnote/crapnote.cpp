
#include "crapnote.h"
#include "ui_crapnote.h"

#include "modules/exceptions.h"


Crapnote::Crapnote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Crapnote)
{
    ui->setupUi(this);
}

Crapnote::~Crapnote()
{
    delete ui;
}


void Crapnote::setTextFont( const QFont& font )
{
    QFont f = QFont( font );
    f.setPointSize( this->font_size );
    this->ui->textEdit->setFont( f );
}

void Crapnote::setColorScheme( const int& color_scheme_id )
{
    QColor b, t;
    // update the colors palette
    switch ( color_scheme_id ) {
        case 0:
            this->ui->textEdit->setPalette( QPalette() );
            break;
        case 1:
            // breeze
            b = QColor(255,198,102);
            t = QColor(31,28,27);
            this->ui->textEdit->setPalette( QPalette(t,b,b,b,b,t,b) );
            break;
        case 2:
            // monokai
            b = QColor(166,226,46);
            t = QColor(39,40,34);
            this->ui->textEdit->setPalette( QPalette(t,b,b,b,b,t,b) );
            break;
        case 3:
            // radical
            b = QColor(20,19,34);
            t = QColor(213,53,143);
            this->ui->textEdit->setPalette( QPalette(t,b,b,b,b,t,b) );
            break;
        default:
            // wrong
            throw GenericException( "Unexpected ColorScheme ID for Crapnote: "+std::to_string( scheme_id ), true ); // leave un-catched
    }
}


void Crapnote::on_spinBox_FontSize_valueChanged(int arg1)
{
    this->font_size = arg1;
    this->setTextFont( this->ui->textEdit->font() );
}


void Crapnote::on_button_FontSize_Minus_clicked()
{
    this->ui->spinBox_FontSize->setValue( this->font_size-1 );
}


void Crapnote::on_button_FontSize_Plus_clicked()
{
    this->ui->spinBox_FontSize->setValue( this->font_size+1 );
}

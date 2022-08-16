
#include "craphelp.h"
#include "ui_craphelp.h"

#include "modules/exceptions.h"

#include "utilities/io.h"


Craphelp::Craphelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Craphelp)
{
    ui->setupUi(this);
}

Craphelp::~Craphelp()
{
    delete ui;
}


const std::unordered_map<std::string, QString> Craphelp::getColorScheme( const int& scheme_id )
{
    switch ( scheme_id ) {
        case 0:
            // none
            return {
                {"background", ""},
                      {"text", ""},
                        {"h1", ""},
                        {"h3", ""},
                      {"code", ""},
                      {"link", ""} };
        case 1:
            // breeze
            return {
                {"background", "#ffffff"},
                      {"text", "#1f1c1b"},
                    {"italic", "#9c9c9b"},
                        {"h1", "#006e28"},
                        {"h3", "#54b8ff"},
                      {"code", "#644a9b"},
                      {"link", "#d24f4f"} };

        case 2:
            // monokai
            return {
                {"background", "#272822"},
                      {"text", "#d1d1cb"},
                    {"italic", "#c1b864"},
                        {"h1", "#a6e22e"},
                        {"h3", "#9773db"},
                      {"code", "#57adbc"},
                      {"link", "#f92672"} };

        case 3:
            // radical
            return {
                {"background", "#141322"},
                      {"text", "#a8c0c2"},
                    {"italic", "#fda8bc"},
                        {"h1", "#d5358f"},
                        {"h3", "#56e8e4"},
                      {"code", "#7c9c9e"},
                      {"link", "#ff85a1"} };

        default:
            // wrong
            throw GenericException( "Unexpected ColorScheme ID: "+std::to_string( scheme_id ) );
    }
}


void Craphelp::helpLogsFormat( const std::string& path, const QFont& font, const int &color_scheme_id )
{
    std::unordered_map<std::string, QString> color_scheme = this->getColorScheme( color_scheme_id );
    std::string aux;
    IOutils::readFile( path, aux );
    QString content;
    if ( color_scheme_id == 0 ) {
        // remove the style for the colors
        while (true) {
            const int start = aux.find( "background-color:" );
            if ( start < 0 || start > aux.size() ) {
                break;
            }
            const int stop  = aux.find( ";\n", start ) + 2;
            aux = aux.substr( 0, start ) + aux.substr( stop );
        }
        while (true) {
            const int start = aux.find( "color:" );
            if ( start < 0 || start > aux.size() ) {
                break;
            }
            const int stop  = aux.find( ";\n", start ) + 2;
            aux = aux.substr( 0, start ) + aux.substr( stop );
        }
        content = QString::fromStdString( aux );

    } else {
        // replace with colors
        content = QString::fromStdString( aux )
            .replace( "$BG$",   color_scheme.at("background") )
            .replace( "$TEXT$", color_scheme.at("text") )
            .replace( "$IT$",   color_scheme.at("italic") )
            .replace( "$H1$",   color_scheme.at("h1") )
            .replace( "$H3$",   color_scheme.at("h3") )
            .replace( "$CODE$", color_scheme.at("code") )
            .replace( "$LINK$", color_scheme.at("link") );
    }
    // show the content
    this->ui->helpBrowser->setText( content );
    // apply the font
    this->ui->helpBrowser->setFont( font );
}

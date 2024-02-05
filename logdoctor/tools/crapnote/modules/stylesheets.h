#ifndef LOGDOCTOR__CRAPNOTE__STYLESHEETS_H
#define LOGDOCTOR__CRAPNOTE__STYLESHEETS_H


enum class ColorsScheme : unsigned char;

class QString;


namespace StyleSec::Crapnote
{

QString getStyleSheet( const ColorsScheme colors_scheme );

} // namespacce StyleSec::Crapnote


#endif // LOGDOCTOR__CRAPNOTE__STYLESHEETS_H

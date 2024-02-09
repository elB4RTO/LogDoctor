#ifndef LOGDOCTOR__CRAPINFO__STYLESHEETS_H
#define LOGDOCTOR__CRAPINFO__STYLESHEETS_H


class QString;


namespace StyleSec::Crapinfo
{

//! Returns the proper style sheet
/*!
    \throw DoNotCatchException
*/
QString getStyleSheet();

} // namespace StyleSec::Crapinfo


#endif // LOGDOCTOR__CRAPINFO__STYLESHEETS_H

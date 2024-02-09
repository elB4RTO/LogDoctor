#ifndef LOGDOCTOR__CRAPUP__STYLESHEETS_H
#define LOGDOCTOR__CRAPUP__STYLESHEETS_H


class QString;


namespace StyleSec::Crapup
{

//! Returns the proper style sheet
/*!
    \throw DoNotCatchException
*/
QString getStyleSheet();

} // namespacce StyleSec::Crapup


#endif // LOGDOCTOR__CRAPUP__STYLESHEETS_H

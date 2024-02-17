#ifndef LOGDOCTOR__UTILITIES__STYLESHEETS_H
#define LOGDOCTOR__UTILITIES__STYLESHEETS_H


class QString;


//! StyleSec
/*!
    Utilities for the stylesheets
*/
namespace StyleSec
{

//! Returns the proper style sheet
/*!
    \throw DoNotCatchException
*/
QString getStyleSheet();

} // namespace StyleSec


#endif // LOGDOCTOR__UTILITIES__STYLESHEETS_H

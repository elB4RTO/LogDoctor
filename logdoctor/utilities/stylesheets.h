#ifndef LOGDOCTOR__UTILITIES__STYLESHEETS_H
#define LOGDOCTOR__UTILITIES__STYLESHEETS_H


class QString;


//! StyleSec
/*!
    Utilities for the stylesheets
*/
namespace StyleSec
{

//! Provides the requested stylesheet
/*!
    \param stylesheet Will hold the stylesheet
*/
void getStyleSheet( QString& stylesheet );

} // namespace StyleSec


#endif // LOGDOCTOR__UTILITIES__STYLESHEETS_H

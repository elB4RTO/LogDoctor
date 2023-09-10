#ifndef STYLESHEETS_H
#define STYLESHEETS_H

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

#endif // STYLESHEETS_H

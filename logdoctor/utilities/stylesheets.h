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
    \param icons_theme The theme selected for the Icons
*/
void getStyleSheet( QString& stylesheet, const QString& icons_theme );

} // namespace StyleSec

#endif // STYLESHEETS_H

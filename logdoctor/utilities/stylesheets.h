#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <unordered_map>

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
    \param theme_id The theme selected for the Window
*/
void getStyleSheet( QString& stylesheet, const QString& icons_theme, const int theme_id );

} // namespace StyleSec

#endif // STYLESHEETS_H

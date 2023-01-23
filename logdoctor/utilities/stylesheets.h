#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>

#include <unordered_map>


//! StyleSec
/*!
    Utilities for the stylesheets
*/
class StyleSec
{
public:
    explicit StyleSec();

    //! Provides the requested stylesheet
    /*!
        \param stylesheet Will hold the stylesheet
        \param icons_theme The theme selected for the Icons
        \param theme_id The theme selected for the Window
    */
    static void getStyleSheet( QString& stylesheet, const QString& icons_theme, const int theme_id );

private:

    //! Builds the final stylesheet upon the generic model
    /*!
        \param ss Will hold the stylesheet
        \param style The key-value pairs to be applied at the generic
        \param theme The theme selected for the Icons
    */
    static void makeStyleSheet( QString& ss, const std::unordered_map<std::string, QString> &style, const QString& theme );
};

#endif // STYLESHEETS_H

#ifndef LOGDOCTOR__CHANGELOG__STYLESHEETS_H
#define LOGDOCTOR__CHANGELOG__STYLESHEETS_H


class QString;


namespace StyleSec::Changelog
{

//! Returns the proper style sheet
/*!
    \throw DoNotCatchException
*/
QString getStyleSheet();

} // namespace StyleSec::Changelog


#endif // LOGDOCTOR__CHANGELOG__STYLESHEETS_H

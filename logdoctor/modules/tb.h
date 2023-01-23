#ifndef LOGFILESTB_H
#define LOGFILESTB_H

#include <QFont>

#include <string>
#include <unordered_map>


//! TextBrowser
/*!
    Perform operations for files visualization
*/
class TextBrowser
{
public:
    explicit TextBrowser();


    /////////////////
    //// GETTERS ////

    //! Returns whether the wide lines option is set to be used or not
    const bool& getWideLinesUsage() const;

    //! Returns the ID of the color scheme in use
    const int& getColorSchemeID() const;

    //! Returns the color scheme in use
    const std::unordered_map<std::string, QString>& getColorScheme() const;

    /*const int& getFontSize();*/

    //! Returns the family of the font in use
    const QString& getFontFamily() const;

    //! Returns the font in use
    const QFont& getFont() const;


    /////////////////
    //// SETTERS ////

    //! Sets whether to use wide lines or not
    void setWideLinesUsage( const bool& use_wide_lines );

    //! Stes the given color scheme as the one in use
    void setColorScheme( const int& color_scheme_id, const std::unordered_map<std::string, QString>& color_scheme );

    /*void setFontSize( const int& font_size );
    void setFontFamily( const QString& font_family );*/

    //! Sets the given font as the one in use
    void setFont( const QFont& font );


    /////////////////
    //// PREVIEW ////

    //! Builds the preview which will be shown in the configs tab
    /*!
        \param content Will hold the preview string
    */
    void makePreview( QString& content ) const;


private:

    bool wide_lines = false;
    int  color_scheme_id = 1;
    std::unordered_map<std::string, QString> color_scheme;
    /*int  font_size = 13;*/
    QString font_family;
    QFont   font;
};

#endif // LOGFILESTB_H

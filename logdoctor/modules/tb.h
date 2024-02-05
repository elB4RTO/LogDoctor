#ifndef LOGDOCTOR__MODULES__TB_H
#define LOGDOCTOR__MODULES__TB_H


#include <QFont>

#include <string>
#include <unordered_map>


enum class ColorsScheme : unsigned char;


//! TextBrowser
/*!
    Perform operations for files visualization
*/
class TextBrowser final
{
public:

    /////////////////
    //// GETTERS ////

    //! Returns whether the wide lines option is set to be used or not
    bool getWideLinesUsage() const noexcept;

    //! Returns the ID of the color scheme in use
    ColorsScheme getColorSchemeID() const noexcept;

    //! Returns the color scheme in use
    const std::unordered_map<std::string, QString>& getColorScheme() const noexcept;

    /*const int& getFontSize() noexcept;*/

    //! Returns the family of the font in use
    const QString& getFontFamily() const noexcept;

    //! Returns the font in use
    const QFont& getFont() const noexcept;


    /////////////////
    //// SETTERS ////

    //! Sets whether to use wide lines or not
    void setWideLinesUsage( const bool use_wide_lines ) noexcept;

    //! Stes the given color scheme as the one in use
    void setColorScheme( const ColorsScheme colors_scheme_id, const std::unordered_map<std::string, QString>& colors_scheme ) noexcept;

    /*void setFontSize( const int& font_size ) noexcept;
    void setFontFamily( const QString& font_family ) noexcept;*/

    //! Sets the given font as the one in use
    void setFont( const QFont& font ) noexcept;


    /////////////////
    //// PREVIEW ////

    //! Builds the preview which will be shown in the configs tab
    /*!
        \param content Will hold the preview string
    */
    void makePreview( QString& content ) const noexcept;


private:

    bool wide_lines{ false };
    ColorsScheme  colors_scheme_id;
    std::unordered_map<std::string, QString> colors_scheme;
    /*int  font_size = 13;*/
    QString font_family;
    QFont   font;
};


#endif // LOGDOCTOR__MODULES__TB_H

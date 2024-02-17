#ifndef LOGDOCTOR__CRAPHELP_H
#define LOGDOCTOR__CRAPHELP_H


#include <QWidget>

#include <unordered_map>


enum class ColorsScheme : unsigned char;


namespace Ui {
    class Craphelp;
}


//! Craphelp
/*!
    Displays an help window
*/
class Craphelp final : public QWidget
{
    Q_OBJECT

public:
    explicit Craphelp( QWidget* parent=nullptr );
    Q_DISABLE_COPY_MOVE(Craphelp)

    //! Provides help about log formats
    /*!
        \param path The path of the file resource to be displayed
        \param font The font to be used
        \param colors_scheme_id The ID of the color-scheme to be used
        \throw DoNotCatchException
    */
    void helpLogsFormat( const std::string& path, const QFont& font, const ColorsScheme colors_scheme_id ) const noexcept;

    //! Provides help about log formats
    /*!
        Shows the built-in default version in case the file
        for the currently selected locale was not found or unreadable
        \param file_name The file that was supposed to be shown
        \param font The font to be used
        \param colors_scheme_id The ID of the color-scheme to be used
        \throw DoNotCatchException
    */
    void helpLogsFormatDefault( std::string_view file_name, const QFont& font, const ColorsScheme colors_scheme_id ) const noexcept;

private:
    QSharedPointer<Ui::Craphelp> ui;

    // \throw DoNotCatchException
    std::unordered_map<std::string, QString> getColorScheme( const ColorsScheme scheme_id ) const;

    void defaultApacheFormat( std::string& str ) const noexcept;
    void defaultNginxFormat( std::string& str ) const noexcept;
    void defaultIisFormat( std::string& str ) const noexcept;
};


#endif // LOGDOCTOR__CRAPHELP_H

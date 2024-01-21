#ifndef LOGDOCTOR__CRAPHELP_H
#define LOGDOCTOR__CRAPHELP_H


#include <QWidget>

#include <unordered_map>


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

    //! Provides help about log formats
    /*!
        \param path The path of the file resource to be displayed
        \param font The font to be used
        \param color_scheme_id The ID of the color-scheme to be used
    */
    void helpLogsFormat( const std::string& path, const QFont& font, const int color_scheme_id ) const noexcept;

    //! Provides help about log formats
    /*!
        Shows the built-in default version in case the file
        for the currently selected locale was not found or unreadable
        \param file_name The file that was supposed to be shown
        \param font The font to be used
        \param color_scheme_id The ID of the color-scheme to be used
    */
    void helpLogsFormatDefault( std::string_view file_name, const QFont& font, const int color_scheme_id ) const noexcept;

private:
    QSharedPointer<Ui::Craphelp> ui;

    std::unordered_map<std::string, QString> getColorScheme( const int scheme_id ) const;

    void defaultApacheFormat( std::string& str ) const noexcept;
    void defaultNginxFormat( std::string& str ) const noexcept;
    void defaultIisFormat( std::string& str ) const noexcept;
};


#endif // LOGDOCTOR__CRAPHELP_H

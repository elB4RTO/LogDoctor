#ifndef CRAPHELP_H
#define CRAPHELP_H

#include <QWidget>

#include <unordered_map>


namespace Ui {
    class Craphelp;
}

//! Craphelp
/*!
    Displays an help window
*/
class Craphelp : public QWidget
{
    Q_OBJECT

public:
    explicit Craphelp( QWidget* parent=nullptr );
    ~Craphelp();

    //! Provides help about log formats
    /*!
        \param path The path of the file resource to be displayed
        \param font The font to be used
        \param color_scheme_id The ID of the color-scheme to be used
    */
    void helpLogsFormat( const std::string& path, const QFont& font, const int& color_scheme_id );

private:
    Ui::Craphelp *ui;

    const std::unordered_map<std::string, QString> getColorScheme( const int& scheme_id );
};

#endif // CRAPHELP_H

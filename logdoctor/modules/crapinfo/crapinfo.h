#ifndef CRAPINFO_H
#define CRAPINFO_H

#include <QWidget>


namespace Ui {
    class Crapinfo;
}

//! Crapinfo
/*!
    Displays informations
*/
class Crapinfo : public QWidget
{
    Q_OBJECT

public:

    //! Class constructor
    /*!
        \param window_theme_id The ID of the theme in use on the main window
        \param version The version of LogDoctor actually running
        \param exec_path The path of the executable
        \param conf_path The path of the configuration file
        \param logdoc_path The path of the additional resources folder
        \param parent The parent Widget
    */
    explicit Crapinfo( const int& window_theme_id, const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget* parent=nullptr );
    ~Crapinfo();

private:
    Ui::Crapinfo *ui;

    void getStyleSheet( QString& stylesheet, const int& theme_id ) const;
};

#endif // CRAPINFO_H

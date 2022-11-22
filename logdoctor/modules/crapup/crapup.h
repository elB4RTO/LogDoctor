#ifndef CRAPUP_H
#define CRAPUP_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QNetworkReply>


namespace Ui {
    class Crapup;
}


//! Crapup
/*!

*/
class Crapup : public QWidget
{
    Q_OBJECT
public:

    //! Class constructor
    /*!
        \param window_theme_id The ID of the window theme
        \param icons theme The theme of the icons
        \param parent The parent Widget
    */
    Crapup( const int& window_theme_id, const QString& icons_theme, QWidget* parent=nullptr );
    ~Crapup();


    //! Performs a version check
    /*!
        \param current_version The running version of LogDoctor
        \throw GenericException
    */
    void versionCheck( const float& current_version );


signals:

    //! Emitted to abort an on-going request
    void abortRequest();


private slots:

    //! Override
    void closeEvent( QCloseEvent* event );

    //! Called when a request's times out
    /*!
        \see deleteReply(), abortRequest()
    */
    void requestTimeout();

    //! Rotates the 'checking' image
    void rotateImg();


private:
    Ui::Crapup *ui;

    QPixmap img_checking;

    float img_orientation = 0.0;

    QTimer* img_timer = nullptr;

    void getStyleSheet( QString& stylesheet, const int& theme_id );

    bool request_aborted;

    QTimer* request_timer = nullptr;

    const int timeout_msec = 15000;

    QNetworkReply* reply = nullptr;

    //! Deletes the reply in a safe way
    void deleteReply();

    bool quitting = false;
};

#endif // CRAPUP_H

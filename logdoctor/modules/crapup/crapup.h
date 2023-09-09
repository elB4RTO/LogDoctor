#ifndef CRAPUP_H
#define CRAPUP_H

#include <QWidget>

class QNetworkReply;
class QPixmap;
class QTimer;


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
        \param icons_theme The theme of the icons
        \param parent The parent Widget
    */
    explicit Crapup( const QString& icons_theme, QWidget* parent=nullptr );
    ~Crapup();


    //! Performs a version check
    /*!
        \param current_version The running version of LogDoctor
        \throw GenericException
    */
    void versionCheck( const float current_version );


signals:

    //! Emitted to abort an on-going request
    void abortRequest();


private slots:

    //! Override
    void closeEvent( QCloseEvent* event ) override;

    //! Called when a request's times out
    /*!
        \see deleteReply(), abortRequest()
    */
    void requestTimeout();

    //! Rotates the 'checking' image
    void rotateImg();


private:
    QSharedPointer<Ui::Crapup> ui;

    QPixmap img_checking;

    float img_orientation{ 0.0 };

    QScopedPointer<QTimer> img_timer;

    bool request_aborted;

    QScopedPointer<QTimer> request_timer;

    const int timeout_msec{ 15000 };

    QScopedPointer<QNetworkReply> reply;

    //! Deletes the reply in a safe way
    void deleteReply();

    bool quitting{ false };
};

#endif // CRAPUP_H

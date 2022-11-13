#ifndef CRAPUP_H
#define CRAPUP_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QNetworkReply>


namespace Ui {
class Crapup;
}

class Crapup : public QWidget
{
    Q_OBJECT
public:
    Crapup( const int& window_theme_id, const QString& icons_theme, QWidget* parent=nullptr );
    ~Crapup();

    void versionCheck( const float& current_version );

signals:
    void abortRequest();

private slots:
    void closeEvent( QCloseEvent* event );

    void requestTimeout();

    void rotateImg();

private:
    Ui::Crapup *ui;

    QPixmap img_checking;
    float img_orientation = 0.0;
    QTimer* img_timer = nullptr;

    void getStyleSheet( QString& stylesheet, const int& theme_id );

    bool request_aborted;
    QTimer* request_timer = nullptr;
    QNetworkReply* reply = nullptr;
    void deleteReply();

    bool quitting = false;
};

#endif // CRAPUP_H
